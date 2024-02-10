#include "executor.h"

namespace shdb
{

namespace
{

class ReadFromRowsExecutor : public IExecutor
{
public:
    explicit ReadFromRowsExecutor(Rows rows_, std::shared_ptr<Schema> rows_schema_)
        : rows(std::move(rows_)), rows_schema(std::move(rows_schema_))
    {
        throw std::runtime_error("Not implemented");
    }

    std::optional<Row> next() override { throw std::runtime_error("Not implemented"); }

    std::shared_ptr<Schema> getOutputSchema() override { throw std::runtime_error("Not implemented"); }

private:
    Rows rows;
    std::shared_ptr<Schema> rows_schema;
};

class ReadFromTableExecutor : public IExecutor
{
public:
    explicit ReadFromTableExecutor(std::shared_ptr<ITable> table_, std::shared_ptr<Schema> table_schema_)
        : table(std::move(table_)), table_schema(std::move(table_schema_))
    {
        throw std::runtime_error("Not implemented");
    }

    std::optional<Row> next() override { throw std::runtime_error("Not implemented"); }

    std::shared_ptr<Schema> getOutputSchema() override { throw std::runtime_error("Not implemented"); }

private:
    std::shared_ptr<ITable> table;
    std::shared_ptr<Schema> table_schema;
};

class ExpressionsExecutor : public IExecutor
{
public:
    explicit ExpressionsExecutor(ExecutorPtr input_executor_, Expressions expressions_)
        : input_executor(std::move(input_executor_)), expressions(std::move(expressions_))
    {
        throw std::runtime_error("Not implemented");
    }

    std::optional<Row> next() override { throw std::runtime_error("Not implemented"); }

    std::shared_ptr<Schema> getOutputSchema() override { throw std::runtime_error("Not implemented"); }

private:
    ExecutorPtr input_executor;
    Expressions expressions;
};

class FilterExecutor : public IExecutor
{
public:
    explicit FilterExecutor(ExecutorPtr input_executor_, ExpressionPtr filter_expression_)
        : input_executor(std::move(input_executor_)), filter_expression(std::move(filter_expression_))
    {
    }

    std::optional<Row> next() override { throw std::runtime_error("Not implemented"); }

    std::shared_ptr<Schema> getOutputSchema() override { throw std::runtime_error("Not implemented"); }

private:
    ExecutorPtr input_executor;
    ExpressionPtr filter_expression;
};

class SortExecutor : public IExecutor
{
public:
    explicit SortExecutor(ExecutorPtr input_executor_, SortExpressions sort_expressions_)
        : input_executor(std::move(input_executor_)), sort_expressions(std::move(sort_expressions_))
    {
        throw std::runtime_error("Not implemented");
    }

    std::optional<Row> next() override { throw std::runtime_error("Not implemented"); }

    std::shared_ptr<Schema> getOutputSchema() override { throw std::runtime_error("Not implemented"); }

private:
    ExecutorPtr input_executor;
    SortExpressions sort_expressions;
};

class JoinExecutor : public IExecutor
{
public:
    explicit JoinExecutor(ExecutorPtr left_input_executor_, ExecutorPtr right_input_executor_)
        : left_input_executor(std::move(left_input_executor_)), right_input_executor(std::move(right_input_executor_))
    {
        throw std::runtime_error("Not implemented");
    }

    std::optional<Row> next() override { throw std::runtime_error("Not implemented"); }

    std::shared_ptr<Schema> getOutputSchema() override { throw std::runtime_error("Not implemented"); }

private:
    ExecutorPtr left_input_executor;
    ExecutorPtr right_input_executor;
};

class GroupByExecutor : public IExecutor
{
public:
    explicit GroupByExecutor(ExecutorPtr input_executor_, GroupByKeys group_by_keys_, GroupByExpressions group_by_expressions_)
        : input_executor(std::move(input_executor_))
        , group_by_keys(std::move(group_by_keys_))
        , group_by_expressions(std::move(group_by_expressions_))
    {
        throw std::runtime_error("Not implemented");
    }

    std::optional<Row> next() override { throw std::runtime_error("Not implemented"); }

    std::shared_ptr<Schema> getOutputSchema() override { throw std::runtime_error("Not implemented"); }

private:
    ExecutorPtr input_executor;
    GroupByKeys group_by_keys;
    GroupByExpressions group_by_expressions;
};

}

ExecutorPtr createReadFromRowsExecutor(Rows rows, std::shared_ptr<Schema> rows_schema)
{
    (void)(rows);
    (void)(rows_schema);
    throw std::runtime_error("Not implemented");
}

ExecutorPtr createReadFromTableExecutor(std::shared_ptr<ITable> table, std::shared_ptr<Schema> table_schema)
{
    (void)(table);
    (void)(table_schema);
    throw std::runtime_error("Not implemented");
}

ExecutorPtr createExpressionsExecutor(ExecutorPtr input_executor, Expressions expressions)
{
    (void)(input_executor);
    (void)(expressions);
    throw std::runtime_error("Not implemented");
}

ExecutorPtr createFilterExecutor(ExecutorPtr input_executor, ExpressionPtr filter_expression)
{
    (void)(input_executor);
    (void)(filter_expression);
    throw std::runtime_error("Not implemented");
}

ExecutorPtr createSortExecutor(ExecutorPtr input_executor, SortExpressions sort_expressions)
{
    (void)(input_executor);
    (void)(sort_expressions);
    throw std::runtime_error("Not implemented");
}

ExecutorPtr createJoinExecutor(ExecutorPtr left_input_executor, ExecutorPtr right_input_executor)
{
    (void)(left_input_executor);
    (void)(right_input_executor);
    throw std::runtime_error("Not implemented");
}

ExecutorPtr createGroupByExecutor(ExecutorPtr input_executor, GroupByKeys group_by_keys, GroupByExpressions group_by_expressions)
{
    (void)(input_executor);
    (void)(group_by_keys);
    (void)(group_by_expressions);
    throw std::runtime_error("Not implemented");
}

RowSet execute(ExecutorPtr executor)
{
    (void)(executor);
    throw std::runtime_error("Not implemented");
}

}
