#pragma once

#include "aggregate_function.h"
#include "ast.h"
#include "database.h"
#include "row.h"
#include "rowset.h"
#include <unordered_map>

namespace shdb
{

class Interpreter
{
public:
    explicit Interpreter(std::shared_ptr<Database> db_);

    RowSet execute(const std::string & query);

private:
    RowSet executeSelect(const ASTPtr & select_query);
    void executeInsert(const ASTInsertQueryPtr & insert_query);
    void executeCreate(const ASTCreateQueryPtr & create_query);
    void executeDrop(const ASTDropQueryPtr & drop_query);
    Row executeSomeAST(const ASTPtr & ast, bool load_var);
    RowSet executeJoin(std::vector<std::string>& table_names);

    std::shared_ptr<Database> db;
    AggregateFunctionFactory aggregate_function_factory;
    std::unordered_map<std::string, Value> variables;
};

}
