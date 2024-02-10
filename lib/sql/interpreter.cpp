#include "interpreter.h"

#include "accessors.h"
#include "ast.h"
#include "ast_visitor.h"
#include "comparator.h"
#include "executor.h"
#include "expression.h"
#include "lexer.h"
#include "parser.hpp"
#include "row.h"
#include "rowset.h"
#include "schema.h"
#include <iostream>
#include <stdexcept>
#include <variant>
#include <unordered_map>
#include <unordered_set>
#include <optional>

namespace shdb
{

Interpreter::Interpreter(std::shared_ptr<Database> db_) : db(std::move(db_))
{
    registerAggregateFunctions(aggregate_function_factory);
}

RowSet Interpreter::execute(const std::string & query)
{
    std::string processed_query = query + "\n"; 
    Lexer lexer(processed_query.c_str(), processed_query.c_str() + processed_query.size());
    ASTPtr result;
    std::string error;
    shdb::Parser parser(lexer, result, error);
    parser.parse();
    if (!result || !error.empty())
        throw std::runtime_error("Bad input: " + error);
    std::cout << "HERE\n";

    switch (result->type)
    {
        case ASTType::selectQuery:
            return executeSelect(std::static_pointer_cast<ASTSelectQuery>(result));
        case ASTType::insertQuery:
            executeInsert(std::static_pointer_cast<ASTInsertQuery>(result));
            break;
        case ASTType::createQuery:
            std::cout << "CREATE\n";

            executeCreate(std::static_pointer_cast<ASTCreateQuery>(result));
            break;
        case ASTType::dropQuery:
            std::cout << "DROP OK\n";
            executeDrop(std::static_pointer_cast<ASTDropQuery>(result));
            break;
        default:
            throw std::runtime_error("Invalid AST. Expected SELECT, INSERT, CREATE or DROP query");
    }

    return RowSet{};
}

Row Interpreter::executeSomeAST(const ASTPtr & ast, bool load_var) {
    if (!ast) {
        throw std::runtime_error("nullptr in execute select");
    }
    switch (ast->type) {
        case ASTType::literal: {
            auto op = std::static_pointer_cast<ASTLiteral>(ast);
            if (op->literal_type == ASTLiteralType::string) {
                if (!variables.count(op->string_value) || load_var) {
                    std::cout << "FOUNDED " << op->string_value << '\n';
                    return {op->string_value};
                } else {
                    return {variables[op->string_value]};
                }
            } else {
                return {op->integer_value};
            }
        }
        case ASTType::binaryOperator: {
            auto op = std::static_pointer_cast<ASTBinaryOperator>(ast);
            auto left_val = executeSomeAST(op->getLHS(), load_var)[0];
            auto right_val = executeSomeAST(op->getRHS(), load_var)[0];
            switch (op->operator_code) {
            case BinaryOperatorCode::plus: {
                return {std::get<int64_t>(left_val) + std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::minus: {
                return {std::get<int64_t>(left_val) - std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::mul: {
                return {std::get<int64_t>(left_val) * std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::div: {
                return {std::get<int64_t>(left_val) / std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::eq: {
                return {std::get<int64_t>(left_val) == std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::ge: {
                return {std::get<int64_t>(left_val) >= std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::gt: {
                return {std::get<int64_t>(left_val) > std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::lt: {
                return {std::get<int64_t>(left_val) < std::get<int64_t>(right_val)};
            }
            case BinaryOperatorCode::le: {
                return {std::get<int64_t>(left_val) <= std::get<int64_t>(right_val)};
            }
            default: assert(0);
            }
        }
        default: assert(0);
    }
}

std::optional<Value> extractValue(const Row& row, const Schema& schema, const ColumnSchema& column) {
    for (size_t i = 0; i < schema.size(); ++i) {
        if (schema[i] == column) {
            return row[i];
        }
    }
    return std::nullopt;
}

RowSet Interpreter::executeJoin(std::vector<std::string>& table_names) {
    assert(!table_names.empty());
    if (table_names.size() == 1) {
        auto table_name = table_names[0];
        auto table = db->getTable(table_name);
        auto scan = shdb::Scan(table);
        auto schema = db->findTableSchema(table_name);
        RowSet answer(schema);
        for (auto it = scan.begin(), end = scan.end(); it != end; ++it) {
            auto row = it.getRow();
            if (row.empty()) {
                continue;
            }
            answer.addRow(row);
        }
        return answer;
    } else {
        auto table_name = table_names.back();
        table_names.pop_back();
        RowSet left = executeJoin(table_names);
        std::vector<std::string> right_table_names = {table_name};
        RowSet right = executeJoin(right_table_names);
        Schema intersect_schema = Schema();
        Schema result_schema = *left.getSchema();

        for (size_t i = 0; i < right.getSchema()->size(); ++i) {
            bool count = false;
            for (size_t j = 0; j < left.getSchema()->size(); ++j) {
                if (right.getSchema()->at(i) == left.getSchema()->at(j)) {
                    count = true;
                }
            }
            if (count) {
                intersect_schema.push_back(right.getSchema()->at(i));
            } else {
                result_schema.push_back(right.getSchema()->at(i));
            }
        }

        auto comporator = Comparator();
        RowSet result_rowset(std::make_shared<Schema>(result_schema));
        for (auto left_row : left.getRows()) {
            for (auto right_row : right.getRows()) {
                bool matched = true;
                for (auto intersected_col : intersect_schema) {
                    auto left_intersected_val = *extractValue(left_row, *left.getSchema(), intersected_col);
                    auto right_intersected_val = *extractValue(right_row, *right.getSchema(), intersected_col);
                    matched &= (comporator({left_intersected_val}, {right_intersected_val}) == 0);
                }
                if (matched) {
                    Row join_row;
                    for (size_t i = 0; i < result_schema.size(); ++i) {
                        auto left_intersected_val = extractValue(left_row, *left.getSchema(), result_schema[i]);
                        if (left_intersected_val) {
                            join_row.push_back(*left_intersected_val);
                        } else {
                            auto right_intersected_val = extractValue(right_row, *right.getSchema(), result_schema[i]);
                            join_row.push_back(*right_intersected_val);
                        }
                    }
                    result_rowset.addRow(join_row);
                }
            }
        }
        return result_rowset;
    }
}


RowSet Interpreter::executeSelect(const ASTPtr & select_query_ptr)
{
    auto ast = select_query_ptr;
    if (!ast) {
        throw std::runtime_error("nullptr in execute select");
    }
    switch (ast->type) {
        case ASTType::selectQuery: {
            auto op = std::static_pointer_cast<ASTSelectQuery>(ast);
            {
                /*
                auto table_name = op->from[0];
                auto table = db->getTable(table_name);
                auto scan = shdb::Scan(table);
                auto filter = op->getWhere();
                auto schema = db->findTableSchema(table_name);*/
                auto table_name =op->from; 
                RowSet start = executeJoin(table_name);
                auto filter = op->getWhere();
                auto schema = start.getSchema();
                auto new_schema = Schema{};
                if (!op->getProjection()) {
                    new_schema = *schema;
                }
                RowSet answer(schema);
                for (auto row : start.getRows()) {
                    if (row.empty()) {
                        continue;
                    }
                    for (size_t i = 0; i < schema->size(); ++i) {
                        variables[schema->at(i).name] = row[i];
                    }

                    auto modifiedRow = Row{};
                    bool should_update_schema = new_schema.empty();
                    if (op->getProjection()) {
                        for (size_t i = 0; i < op->getProjection()->getChildren().size(); ++i) {
                            auto expr = op->getProjection()->getChildren()[i];
                            modifiedRow.push_back(executeSomeAST(expr, false)[0]);
                            if (should_update_schema) {
                                try {
                                    auto var_name = std::get<std::string>(executeSomeAST(expr, true)[0]);
                                    for (auto col: *schema) {
                                        if (col.name == var_name) {
                                            new_schema.push_back(col);
                                        }
                                    }
                                } catch (...) {
                                    new_schema.push_back(ColumnSchema());
                                } 
                            }
                        }
                    } else {
                        modifiedRow = row;
                    }
                    if (!row.empty()) {
                        if (!filter || std::get<bool>(executeSomeAST(filter, false)[0])) {
                            std::cout << "ADD ROW\n";
                            answer.addRow(modifiedRow);
                            answer.getRows();
                        }
                    }
                }
                
                auto sorting = op->getOrder();
                auto res_rows = answer.getRows();

                if (sorting) {
                    std::cout << "APPLY SORTING START\n";
                    std::cout << "APPLY SORTING " << res_rows.size() << '\n';
                    auto sorting_order = sorting->sorting_args;
                    auto comporator = Comparator();

                    std::vector<std::pair<Row, size_t>> sorted_args;
                    for (size_t i = 0; i < res_rows.size(); ++i) {
                        auto row = res_rows[i];
                        for (size_t j = 0; j < new_schema.size(); ++j) {
                            std::cout << "SETTED VALUE " << new_schema[j].name << '\n';
                            variables[new_schema[j].name] = row[j];
                            if (std::holds_alternative<std::string>(row[j])) {
                                std::cout << "WHAT SETTED " << i << ' ' << j << ' ' << std::get<std::string>(row[j])<< ' ' << new_schema[j].name  << '\n';
                            } else if (std::holds_alternative<int64_t>(row[j])) {
                                std::cout << "WHAT SETTED " << i << ' ' << j << ' ' << std::get<int64_t>(row[j])<< ' ' << new_schema[j].name  << '\n';
                            } else if (std::holds_alternative<uint64_t>(row[j])) {
                                std::cout << "WHAT SETTED " << i << ' ' << j << ' ' << std::get<uint64_t>(row[j]) << ' ' << new_schema[j].name << '\n';
                            } else if (std::holds_alternative<Null>(row[j])) {
                                std::cout << "WHAT SETTED " << i << ' ' << j << " NULL\n";
                            } else if (std::holds_alternative<bool>(row[j])) {
                                std::cout << "WHAT SETTED " << i << ' ' << j << " bool\n";
                            }
                        }

                        sorted_args.push_back({{}, i});
                        for (size_t j = 0; j < sorting->getChildren().size(); ++j) {
                            auto current_val = executeSomeAST(sorting->getChildren()[j], false);
                            sorted_args.back().first.push_back(current_val[0]);
                        }
                    }
                    std::cout << "DONE PREPARE SORTING\n";

                    std::sort(sorted_args.begin(), sorted_args.end(),  [&](const std::pair<Row, size_t> &x, const std::pair<Row, size_t> &y) {
                        int answer_cur = comporator(x.first, y.first, sorting_order);
                        std::cout << "sort iter " << answer_cur << ' ' << x.second << ' ' << y.second << '\n';
                        if (std::holds_alternative<std::string>(x.first[0])) {
                            std::cout << "add values " << std::get<std::string>(x.first[0]) << ' ' << std::get<std::string>(y.first[0]) << '\n';
                        } else if (std::holds_alternative<int64_t>(x.first[0])) {
                            std::cout << "add values " << std::get<int64_t>(x.first[0]) << ' ' << std::get<int64_t>(y.first[0]) << '\n';
                        } else if (std::holds_alternative<uint64_t>(x.first[0])) {
                            std::cout << "add values " << std::get<uint64_t>(x.first[0]) << ' ' << std::get<uint64_t>(y.first[0]) << '\n';
                        } else if (std::holds_alternative<bool>(x.first[0])) {
                            std::cout << "add values " << std::get<bool>(x.first[0]) << ' ' << std::get<bool>(y.first[0]) << '\n';
                        } else if (std::holds_alternative<Null>(x.first[0])) {
                            std::cout << "add values None None" << '\n';
                        }
                        return answer_cur == -1;
                    });
                    RowSet answer_sorted;
                    for (size_t i = 0; i < res_rows.size(); ++i) {
                        answer_sorted.addRow(res_rows[sorted_args[i].second]);
                    }
                    std::cout << "=== Sort info ===\n";
                    for (size_t i = 0; i < res_rows.size(); ++i) {
                        std::cout << sorted_args[i].second << ' ';
                        if (std::holds_alternative<int64_t>(sorted_args[i].first[0])) {
                            std::cout << std::get<int64_t>(sorted_args[i].first[0]) << '\n';
                        } else {
                            std::cout << '\n';
                        }
                    }
                    std::cout << "DONE SORTING\n";

                    return answer_sorted;
                }

                return answer;
            } /*else {
                std::vector<Value> answer;
                for (size_t i = 0; i < op->getProjection()->getChildren().size(); ++i) {
                    auto expr = op->getProjection()->getChildren()[i];
                    answer.push_back(executeSomeAST(expr, false)[0]);
                }
                std::cout << "RETURNED\n";
                return RowSet(nullptr, {answer});
            }*/
        }
        default: assert(0);
    }
}

void Interpreter::executeInsert(const std::shared_ptr<ASTInsertQuery> & insert_query)
{
    std::cout << "insert!\n";
    Row insert_values;
    for (size_t i = 0; i < insert_query->getValuesList().getChildren().size(); ++i) {
        std::cout<< "execute " << i <<'\n';
        insert_values.push_back(executeSomeAST(insert_query->getValuesList().getChildren()[i], false)[0]);
        std::cout<< "done execute " << i <<'\n';
        std::cout << toString(*insert_query->getValuesList().getChildren()[i]) << '\n';
        if (std::holds_alternative<bool>(insert_values.back())) {
            std::cout << std::get<bool>(insert_values.back()) << '\n';
        } else if (std::holds_alternative<int64_t>(insert_values.back())) {
            std::cout << std::get<int64_t>(insert_values.back()) << '\n'; 
        } else if (std::holds_alternative<std::string>(insert_values.back())) {
            std::cout << std::get<std::string>(insert_values.back()) << '\n'; 
        }        
    }
    auto table = db->getTable(insert_query->table);
    std::cout << "OK GET TABLE\n";
    table->insertRow(insert_values);
    std::cout << "OK INSERTION\n";

}

void Interpreter::executeCreate(const ASTCreateQueryPtr & create_query)
{
    std::cout << "CREATE\n";
    db->createTable(create_query->table, create_query->schema);
}

void Interpreter::executeDrop(const ASTDropQueryPtr & drop_query)
{
    db->dropTable(drop_query->table);

}

}
    