#include "expression.h"

namespace shdb
{

namespace
{

class IdentifierExpression : public IExpression
{
public:
    explicit IdentifierExpression(const std::string & identifier_name, const std::shared_ptr<SchemaAccessor> & input_schema_accessor)
    {
        (void)(identifier_name);
        (void)(input_schema_accessor);
        throw std::runtime_error("Not implemented");
    }

    Type getResultType() override { throw std::runtime_error("Not implemented"); }

    Value evaluate(const Row & input_row) override
    {
        (void)(input_row);
        throw std::runtime_error("Not implemented");
    }
};

class NumberConstantExpression : public IExpression
{
public:
    explicit NumberConstantExpression(int64_t value_) : value(value_) { }

    Type getResultType() override { return Type::int64; }

    Value evaluate(const Row &) override { return value; }

    Value value;
};

class StringConstantExpression : public IExpression
{
public:
    explicit StringConstantExpression(std::string value_) : value(value_) { }

    Type getResultType() override { return Type::string; }

    Value evaluate(const Row &) override { return value; }

    Value value;
};

class BinaryOperatorExpression : public IExpression
{
public:
    explicit BinaryOperatorExpression(
        BinaryOperatorCode binary_operator_code_, ExpressionPtr lhs_expression_, ExpressionPtr rhs_expression_)
        : binary_operator_code(binary_operator_code_)
        , lhs_expression(std::move(lhs_expression_))
        , rhs_expression(std::move(rhs_expression_))
        , lhs_type(lhs_expression->getResultType())
        , rhs_type(rhs_expression->getResultType())
    {
        throw std::runtime_error("Not implemented");
    }

    Type getResultType() override { throw std::runtime_error("Not implemented"); }

    Value evaluate(const Row & input_row) override
    {
        (void)(input_row);
        throw std::runtime_error("Not implemented");
    }

    const BinaryOperatorCode binary_operator_code;
    ExpressionPtr lhs_expression;
    ExpressionPtr rhs_expression;
    Type lhs_type;
    Type rhs_type;
};

class UnaryOperatorExpression : public IExpression
{
public:
    explicit UnaryOperatorExpression(UnaryOperatorCode unary_operator_code_, ExpressionPtr expression_)
        : unary_operator_code(unary_operator_code_), expression(std::move(expression_)), expression_type(expression->getResultType())
    {
        throw std::runtime_error("Not implemented");
    }

    Type getResultType() override { throw std::runtime_error("Not implemented"); }

    Value evaluate(const Row & input_row) override
    {
        (void)(input_row);
        throw std::runtime_error("Not implemented");
    }

    const UnaryOperatorCode unary_operator_code;
    ExpressionPtr expression;
    Type expression_type;
};

}

ExpressionPtr buildExpression(const ASTPtr & expression, const std::shared_ptr<SchemaAccessor> & input_schema_accessor)
{
    (void)(expression);
    (void)(input_schema_accessor);
    throw std::runtime_error("Not implemented");
}

Expressions buildExpressions(const ASTs & expressions, const std::shared_ptr<SchemaAccessor> & input_schema_accessor)
{
    (void)(expressions);
    (void)(input_schema_accessor);
    throw std::runtime_error("Not implemented");
}

}
