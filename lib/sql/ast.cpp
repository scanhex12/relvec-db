#include "ast.h"
#include <iostream>

namespace shdb
{

ASTPtr newIdentifier(std::string value)
{
    return std::make_shared<ASTIdentifier>(std::move(value));
}

ASTPtr newStringLiteral(std::string value)
{
    return std::make_shared<ASTLiteral>(std::move(value));
}

ASTPtr newNumberLiteral(int64_t value)
{
    return std::make_shared<ASTLiteral>(value);
}

ASTPtr newBinaryOperator(BinaryOperatorCode operator_code, ASTPtr lhs, ASTPtr rhs)
{
    return std::make_shared<ASTBinaryOperator>(operator_code, std::move(lhs), std::move(rhs));
}

ASTPtr newUnaryOperator(UnaryOperatorCode operator_code, ASTPtr operand)
{
    return std::make_shared<ASTUnaryOperator>(operator_code, std::move(operand));
}

ASTListPtr newList()
{
    return std::make_shared<ASTList>();
}

ASTListPtr newList(ASTPtr element)
{
    return std::make_shared<ASTList>(std::move(element));
}

ASTSortingListPtr newSortingList()
{
    return std::make_shared<ASTSortingList>();
}

ASTSortingListPtr newSortingList(ASTPtr element, bool desc)
{
    return std::make_shared<ASTSortingList>(std::move(element), desc);
}


ASTPtr newFunction(std::string name, ASTListPtr args)
{
    return std::make_shared<ASTFunction>(std::move(name), std::move(args));
}

ASTPtr newOrder(ASTPtr expr, bool desc)
{
    return std::make_shared<ASTOrder>(std::move(expr), desc);
}

ASTPtr newSelectQuery(
    ASTListPtr list, std::vector<std::string> from, ASTPtr where, ASTListPtr group_by, std::shared_ptr<IAST> having, ASTSortingListPtr order)
{
    return std::make_shared<ASTSelectQuery>(
        std::move(list), std::move(from), std::move(where), std::move(group_by), std::move(having), std::move(order));
}

ASTPtr newInsertQuery(std::string table, ASTListPtr values)
{
    //std::cout << "newInsertQuery " << table << '\n';
    auto op = std::make_shared<ASTInsertQuery>(std::move(table), std::move(values));
    return op;
}

ASTPtr newCreateQuery(std::string table, Schema schema)
{
    return std::make_shared<ASTCreateQuery>(std::move(table), std::move(schema));
}

ASTPtr newDropQuery(std::string table)
{
    return std::make_shared<ASTDropQuery>(std::move(table));
}

std::string toString(const IAST & ast)
{
    switch (ast.type)
    {
        case ASTType::identifier:
            return static_cast<const ASTIdentifier &>(ast).name;
        case ASTType::literal: {
            const auto & literal = static_cast<const ASTLiteral &>(ast);
            switch (literal.literal_type)
            {
                case ASTLiteralType::number:
                    return std::to_string(literal.integer_value);
                case ASTLiteralType::string:
                    return std::string("\"") + literal.string_value + "\"";
            }
        }
        case ASTType::binaryOperator: {
            auto binary_operator = static_cast<const ASTBinaryOperator &>(ast);
            auto get_op = [&]() -> std::string
            {
                switch (binary_operator.operator_code)
                {
                    case BinaryOperatorCode::plus:
                        return "+";
                    case BinaryOperatorCode::minus:
                        return "-";
                    case BinaryOperatorCode::mul:
                        return "*";
                    case BinaryOperatorCode::div:
                        return "/";
                    case BinaryOperatorCode::land:
                        return "AND";
                    case BinaryOperatorCode::lor:
                        return "OR";
                    case BinaryOperatorCode::eq:
                        return "=";
                    case BinaryOperatorCode::ne:
                        return "<>";
                    case BinaryOperatorCode::lt:
                        return "<";
                    case BinaryOperatorCode::le:
                        return "<=";
                    case BinaryOperatorCode::gt:
                        return ">";
                    case BinaryOperatorCode::ge:
                        return ">=";
                }

                return {};
            };
            return std::string("(") + toString(*binary_operator.getLHS()) + ") " + get_op() + " (" + toString(*binary_operator.getRHS())
                + ")";
        }
        case ASTType::unaryOperator: {
            auto unary_operator = static_cast<const ASTUnaryOperator &>(ast);
            auto get_op = [&]() -> std::string
            {
                switch (unary_operator.operator_code)
                {
                    case UnaryOperatorCode::lnot:
                        return "!";
                    case UnaryOperatorCode::uminus:
                        return "-";
                }

                return {};
            };
            return get_op() + " (" + toString(*unary_operator.getOperand()) + ")";
        }
        case ASTType::order: {
            auto order = static_cast<const ASTOrder &>(ast);
            return toString(*order.getExpr()) + (order.desc ? " DESC" : "");
        }
        case ASTType::list: {
            auto op = static_cast<const ASTList &>(ast);
            if (op.getChildren().empty())
                return {};

            std::string result = toString(*op.getChildren()[0]);
            for (size_t index = 1; index < op.getChildren().size(); ++index)
                result += ", " + toString(*op.getChildren()[index]);
            return result;
        }
        case ASTType::function: {
            auto function = static_cast<const ASTFunction &>(ast);
            return function.name + "(" + toString(function.getArgumentsList()) + ")";
        }
        case ASTType::selectQuery: {
            auto select_query = static_cast<const ASTSelectQuery &>(ast);
            std::string result = "SELECT ";

            if (select_query.getProjectionList().getChildren().empty())
                result += "*";
            else
                result += toString(select_query.getProjectionList());

            if (!select_query.from.empty())
            {
                result += " FROM " + select_query.from[0];
                for (size_t i = 1; i < select_query.from.size(); ++i)
                    result += ", " + select_query.from[i];
            }
            if (select_query.getWhere())
                result += " WHERE " + toString(*select_query.getWhere());
            if (select_query.hasGroupBy())
                result += " GROUP BY " + toString(select_query.getGroupByList());
            if (select_query.getHaving())
                result += " HAVING " + toString(*select_query.getHaving());
            if (select_query.getOrder())
                result += " ORDER BY " + toString(*select_query.getOrder());
            return result;
        }
        case ASTType::insertQuery: {
            auto insert_query = static_cast<const ASTInsertQuery &>(ast);
            return "INSERT " + insert_query.table + " VALUES (" + toString(insert_query.getValuesList()) + ")";
        }
        case ASTType::createQuery: {
            auto create_query = static_cast<const ASTCreateQuery &>(ast);
            auto col_to_string = [&](const auto & column)
            {
                auto result = column.name + " ";
                switch (column.type)
                {
                    case Type::uint64:
                        result += "UINT64";
                        break;
                    case Type::int64:
                        result += "INT64";
                        break;
                    case Type::boolean:
                        result += "BOOLEAN";
                        break;
                    case Type::string:
                        result += "STRING";
                        break;
                    case Type::varchar:
                        result += "VARCHAR(" + std::to_string(column.length) + ")";
                        break;
                }

                return result;
            };
            auto & schema = *create_query.schema;
            auto result = "CREATE TABLE " + create_query.table + " (" + col_to_string(schema[0]);
            for (size_t index = 1; index < schema.size(); ++index)
                result += ", " + col_to_string(schema[index]);
            return result + ")";
        }
        case ASTType::dropQuery: {
            auto drop_query = static_cast<const ASTDropQuery &>(ast);
            return "DROP TABLE " + drop_query.table;
        }
    }

    return {};
}

std::ostream & operator<<(std::ostream & stream, const IAST & ast)
{
    stream << toString(ast);
    return stream;
}

}
