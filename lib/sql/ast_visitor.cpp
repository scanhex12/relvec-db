#include "ast_visitor.h"

namespace shdb
{

namespace
{

class CollectAggregateFunctionsVisitor : public ASTVisitor<CollectAggregateFunctionsVisitor>
{
public:
    explicit CollectAggregateFunctionsVisitor(AggregateFunctionFactory & factory_) : factory(factory_) { }

    void visitImpl(const ASTPtr & node)
    {
        (void)(node);
        throw std::runtime_error("Not implemented");
    }

    AggregateFunctionFactory & factory;
};

}

ASTs collectAggregateFunctions(const ASTs & expressions, AggregateFunctionFactory & factory)
{
    (void)(expressions);
    (void)(factory);
    throw std::runtime_error("Not implemented");
}

}
