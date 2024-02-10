#pragma once

#include "aggregate_function.h"
#include "expression.h"

namespace shdb
{

template <typename Derived>
class ASTVisitor
{
public:
    bool needChildVisit(const ASTPtr & parent, const ASTPtr & child)
    {
        (void)(parent);
        (void)(child);
        return true;
    }

    void visit(const ASTPtr & query_tree_node)
    {
        getDerived().visitImpl(query_tree_node);
        visitChildren(query_tree_node);
    }

private:
    Derived & getDerived() { return *static_cast<Derived *>(this); }

    const Derived & getDerived() const { return *static_cast<Derived *>(this); }

    void visitChildren(const ASTPtr & expression)
    {
        for (const auto & child : expression->getChildren())
        {
            if (!child)
                continue;

            bool need_visit_child = getDerived().needChildVisit(expression, child);
            if (need_visit_child)
                visit(child);
        }
    }
};

ASTs collectAggregateFunctions(const ASTs & expressions, AggregateFunctionFactory & factory);

}
