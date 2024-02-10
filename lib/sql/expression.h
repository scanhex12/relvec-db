#pragma once

#include "accessors.h"
#include "ast.h"
#include <stdexcept>

namespace shdb
{

class IExpression
{
public:
    virtual ~IExpression() = default;

    virtual Type getResultType() = 0;

    virtual Value evaluate(const Row & input_row) = 0;
};

using ExpressionPtr = std::shared_ptr<IExpression>;
using Expressions = std::vector<ExpressionPtr>;

ExpressionPtr buildExpression(const ASTPtr & expression, const std::shared_ptr<SchemaAccessor> & input_schema_accessor);

Expressions buildExpressions(const ASTs & expressions, const std::shared_ptr<SchemaAccessor> & input_schema_accessor);

}
