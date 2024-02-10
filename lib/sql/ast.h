#pragma once

#include <memory>
#include <string>
#include <vector>

#include "schema.h"

namespace shdb
{

enum class ASTType
{
    identifier,
    literal,
    binaryOperator,
    unaryOperator,
    list,
    function,
    order,
    selectQuery,
    insertQuery,
    createQuery,
    dropQuery,
};

class IAST;
using ASTPtr = std::shared_ptr<IAST>;
using ASTs = std::vector<ASTPtr>;

std::string toString(const IAST & ast);

class IAST
{
public:
    explicit IAST(ASTType type_) : type(type_) { }

    virtual ~IAST() = default;

    std::string getName() const { return toString(*this); };

    const ASTs & getChildren() const { return children; }

    const ASTType type;

protected:
    ASTs children;
};

class ASTIdentifier : public IAST
{
public:
    explicit ASTIdentifier(std::string name_) : IAST(ASTType::identifier), name(std::move(name_)) { }
    const std::string name;
};

enum class ASTLiteralType
{
    number,
    string
};

class ASTLiteral : public IAST
{
public:
    explicit ASTLiteral(std::string literal_value)
        : IAST(ASTType::literal), literal_type(ASTLiteralType::string), string_value(std::move(literal_value))
    {
    }

    explicit ASTLiteral(int64_t literal_value) : IAST(ASTType::literal), literal_type(ASTLiteralType::number), integer_value(literal_value)
    {
    }

    const ASTLiteralType literal_type;
    const int64_t integer_value{};
    const std::string string_value{};
};

enum class BinaryOperatorCode
{
    plus,
    minus,
    mul,
    div,
    eq,
    ne,
    lt,
    le,
    gt,
    ge,
    land,
    lor,

};

class ASTBinaryOperator : public IAST
{
public:
    ASTBinaryOperator(BinaryOperatorCode operator_code_, ASTPtr lhs, ASTPtr rhs)
        : IAST(ASTType::binaryOperator), operator_code(operator_code_)
    {
        children.resize(children_size);
        children[lhs_child_index] = std::move(lhs);
        children[rhs_child_index] = std::move(rhs);
    }

    const ASTPtr & getLHS() const { return children[lhs_child_index]; }

    const ASTPtr & getRHS() const { return children[rhs_child_index]; }

    const BinaryOperatorCode operator_code;

private:
    static constexpr size_t lhs_child_index = 0;
    static constexpr size_t rhs_child_index = 1;
    static constexpr size_t children_size = rhs_child_index + 1;
};

enum class UnaryOperatorCode
{
    lnot,
    uminus,
};

class ASTUnaryOperator : public IAST
{
public:
    ASTUnaryOperator(UnaryOperatorCode operator_code_, ASTPtr operand) : IAST(ASTType::unaryOperator), operator_code(operator_code_)
    {
        children.resize(children_size);
        children[operand_child_index] = std::move(operand);
    }

    const ASTPtr & getOperand() const { return children[operand_child_index]; }

    const UnaryOperatorCode operator_code;

private:
    static constexpr size_t operand_child_index = 0;
    static constexpr size_t children_size = operand_child_index + 1;
};

class ASTList : public IAST
{
public:
    explicit ASTList(ASTs elements) : IAST(ASTType::list) { children = std::move(elements); }

    explicit ASTList(ASTPtr element) : ASTList() { children = {std::move(element)}; }

    explicit ASTList() : ASTList(ASTs{}) { }

    void append(ASTPtr element) { children.emplace_back(std::move(element)); }
};

class ASTSortingList : public IAST
{
public:
    explicit ASTSortingList(ASTs elements, std::vector<bool> sorting_rules) : IAST(ASTType::list) { 
        children = std::move(elements);
        sorting_args = std::move(sorting_rules);
    }

    explicit ASTSortingList(ASTPtr element, bool value) : ASTSortingList() { 
        children = {std::move(element)};
        sorting_args = {std::move(value)};
    }

    explicit ASTSortingList() : ASTSortingList(ASTs{}, {}) { }

    void append(ASTPtr element, bool sort_arg) { 
        children.emplace_back(std::move(element));
        sorting_args.emplace_back(std::move(sort_arg));    
    }
    
    std::vector<bool> sorting_args;
};


using ASTListPtr = std::shared_ptr<ASTList>;
using ASTSortingListPtr = std::shared_ptr<ASTSortingList>;


class ASTFunction : public IAST
{
public:
    ASTFunction(std::string name_, ASTListPtr arguments_) : IAST(ASTType::function), name(std::move(name_))
    {
        children.resize(children_size);
        children[arguments_child_index] = std::move(arguments_);
    }

    const ASTPtr & getArguments() const { return children[arguments_child_index]; }

    const ASTList & getArgumentsList() const { return static_cast<const ASTList &>(*children[arguments_child_index]); }

    const std::string name;

private:
    static constexpr size_t arguments_child_index = 0;
    static constexpr size_t children_size = arguments_child_index + 1;
};

class ASTOrder : public IAST
{
public:
    ASTOrder(ASTPtr expr, bool desc_) : IAST(ASTType::order), desc(desc_)
    {
        children.resize(children_size);
        children[expr_child_index] = std::move(expr);
    }

    const ASTPtr & getExpr() const { return children[expr_child_index]; }

    const bool desc = false;

private:
    static constexpr size_t expr_child_index = 0;
    static constexpr size_t children_size = expr_child_index + 1;
};

class ASTSelectQuery : public IAST
{
    ASTSortingListPtr order_;
public:
    ASTSelectQuery(
        ASTListPtr projection, std::vector<std::string> from_, ASTPtr where, ASTListPtr group_by, ASTPtr having, ASTSortingListPtr order)
        : IAST(ASTType::selectQuery), from(std::move(from_))
    {
        children.resize(children_size);
        children[projection_child_index] = std::move(projection);
        children[where_child_index] = std::move(where);
        children[group_by_child_index] = std::move(group_by);
        children[having_child_index] = std::move(having);
        order_ = std::move(order);
    }

    const ASTPtr & getProjection() const { return children[projection_child_index]; }

    const ASTList & getProjectionList() const { return static_cast<const ASTList &>(*children[projection_child_index]); }

    const ASTPtr & getWhere() const { return children[where_child_index]; }

    bool hasGroupBy() const { return !getGroupByList().getChildren().empty(); }

    const ASTPtr & getGroupBy() const { return children[group_by_child_index]; }

    const ASTList & getGroupByList() const { return static_cast<const ASTList &>(*children[group_by_child_index]); }

    const ASTPtr & getHaving() const { return children[having_child_index]; }

    const ASTSortingListPtr & getOrder() const { return order_; }

    const std::vector<std::string> from;

private:
    static constexpr size_t projection_child_index = 0;
    static constexpr size_t where_child_index = 1;
    static constexpr size_t group_by_child_index = 2;
    static constexpr size_t having_child_index = 3;
    static constexpr size_t order_child_index = 4;
    static constexpr size_t children_size = order_child_index + 1;
};

using ASTSelectQueryPtr = std::shared_ptr<ASTSelectQuery>;

class ASTInsertQuery : public IAST
{
public:
    ASTInsertQuery(std::string table_, ASTListPtr values) : IAST(ASTType::insertQuery), table(std::move(table_))
    {
        children.resize(children_size);
        children[values_child_index] = std::move(values);
    }

    const ASTPtr & getValues() { return children[values_child_index]; }

    const ASTList & getValuesList() { return static_cast<const ASTList &>(*children[values_child_index]); }

    const std::string table;

private:
    static constexpr size_t values_child_index = 0;
    static constexpr size_t children_size = values_child_index + 1;
};

using ASTInsertQueryPtr = std::shared_ptr<ASTInsertQuery>;

class ASTCreateQuery : public IAST
{
public:
    ASTCreateQuery(std::string table_, Schema schema_)
        : IAST(ASTType::createQuery), table(std::move(table_)), schema(std::make_shared<Schema>(std::move(schema_)))
    {
    }

    const std::string table;
    const std::shared_ptr<Schema> schema;
};

using ASTCreateQueryPtr = std::shared_ptr<ASTCreateQuery>;

class ASTDropQuery : public IAST
{
public:
    explicit ASTDropQuery(std::string table_) : IAST(ASTType::dropQuery), table(std::move(table_)) { }

    const std::string table;
};

using ASTDropQueryPtr = std::shared_ptr<ASTDropQuery>;

ASTPtr newIdentifier(std::string value);

ASTPtr newStringLiteral(std::string value);

ASTPtr newNumberLiteral(int64_t value);

ASTPtr newBinaryOperator(BinaryOperatorCode operator_code, ASTPtr lhs, ASTPtr rhs);

ASTPtr newUnaryOperator(UnaryOperatorCode operator_code, ASTPtr operand);

ASTListPtr newList();

ASTListPtr newList(ASTPtr element);

ASTSortingListPtr newSortingList();

ASTSortingListPtr newSortingList(ASTPtr element, bool desc);


ASTPtr newFunction(std::string name, ASTListPtr args);

ASTPtr newOrder(ASTPtr expr, bool desc);

ASTPtr newSelectQuery(ASTListPtr list, std::vector<std::string> from, ASTPtr where, ASTListPtr group_by, ASTPtr having, ASTSortingListPtr order);

ASTPtr newInsertQuery(std::string table, ASTListPtr values);

ASTPtr newCreateQuery(std::string table, Schema schema);

ASTPtr newDropQuery(std::string table);

std::string toString(const IAST & ast);

std::ostream & operator<<(std::ostream & stream, const IAST & ast);

}
