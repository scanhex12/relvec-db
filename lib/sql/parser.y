%skeleton "lalr1.cc"
%require "2.5"
%defines
%define api.namespace {shdb}
%define api.value.type variant
%define api.parser.class {Parser}

%code requires {
    #include <memory>
    #include <iostream>
    #include "ast.h"
    #include "schema.h"
    namespace shdb {class Lexer;}
}

%parse-param {shdb::Lexer& lexer} {std::shared_ptr<IAST>& result} {std::string& message}

%code {
    #include "lexer.h"
    #define yylex lexer.lex
}

%token END 0 "end of file"
%token ERROR
%token <std::string> NAME
%type <std::vector<std::string>> NAME_LIST


%type <std::shared_ptr<IAST>> expr, value
%type <Schema> params
%type <ASTListPtr> arguments
%type <ASTSortingListPtr> params_sorting

%token <int> NUM

%token CREATE "CREATE"
%token TABLE "TABLE"
%token DROP "DROP"
%token VARCHAR "varchar"
%token INSERT "INSERT"
%token VALUES "VALUES"
%token FROM "FROM"
%token WHERE "WHERE"
%token ORDER "ORDER"
%token BY "BY"
%token DESC "DESC"



%token PLUS "+"
%token MINUS "-"
%token MUL "*"
%token DIV "/"
%token LPAR "("
%token RPAR ")"
%token EOL "\n"
%token COMA ","
%token LE "<="
%token GE ">="
%token LT "<"
%token GT ">"
%token QUOTES "\""
%token SELECT "SELECT"


%left "<=" ">=" "<" ">"
%left "+" "-"
%left "*" "/"
%nonassoc UMINUS
%nonassoc UNOT


%%

input: expr "\n" { result = $1; }
    | expr {result = $1; };

params : NAME NAME { 
        ColumnSchema elem;
        if ($2 == "string") {
            elem = ColumnSchema{$1, shdb::Type::string, 0};
        } else if ($2 == "uint64") {
            std::cout << "incorrect type!";
            elem = ColumnSchema{$1, shdb::Type::uint64, 0};
        } else if ($2 == "int64") {
            std::cout << "correct type!";
            elem = ColumnSchema{$1, shdb::Type::int64, 0};
        } else if ($2 == "boolean") {
            elem = ColumnSchema{$1, shdb::Type::boolean, 0};
        }
        $$ = Schema{elem};
    }
    | params "," NAME NAME {
        if ($4 == "string") {
            $1.push_back(ColumnSchema{$3, shdb::Type::string, 0});
        } else if ($4 == "uint64") {
            std::cout << "incorrect type!";
            $1.push_back(ColumnSchema{$3, shdb::Type::uint64, 0});
        } else if ($4 == "int64") {
            std::cout << "correct type!";
            $1.push_back(ColumnSchema{$3, shdb::Type::int64, 0});
        } else if ($4 == "boolean") {
            $1.push_back(ColumnSchema{$3, shdb::Type::boolean, 0});
        }
        $$ = std::move($1); 
    }
    | NAME "varchar" "(" NUM ")" {
        ColumnSchema elem{$1, shdb::Type::varchar, $4};
        $$ = Schema{elem};
    }
    | params "," NAME "varchar" "(" NUM ")" {
        $1.push_back(ColumnSchema{$3, shdb::Type::varchar, $6});
        $$ = std::move($1); 
    }


params_sorting : value { $$ = newSortingList($1, false); }
    | value "DESC" { $$ = newSortingList($1, true); }
    | params_sorting "," value { $1->append($3, false); $$ = std::move($1); }
    | params_sorting "," value "DESC" { $1->append($3, true); $$ = std::move($1); }



value: NAME { $$ = newStringLiteral($1); }
    | NUM { $$ = newNumberLiteral($1); }
    | "-" NUM { $$ = newNumberLiteral(-$2); }
    | "\"" NAME "\"" { $$ = newStringLiteral($2); }
    | value "+" value {$$ = newBinaryOperator(BinaryOperatorCode::plus, $1, $3); }
    | value "-" value {$$ = newBinaryOperator(BinaryOperatorCode::minus, $1, $3); }
    | value "*" value {$$ = newBinaryOperator(BinaryOperatorCode::mul, $1, $3); }
    | value "/" value {$$ = newBinaryOperator(BinaryOperatorCode::div, $1, $3); }
    | value "<=" value {$$ = newBinaryOperator(BinaryOperatorCode::le, $1, $3); }
    | value ">=" value {$$ = newBinaryOperator(BinaryOperatorCode::ge, $1, $3); }
    | value ">" value {$$ = newBinaryOperator(BinaryOperatorCode::gt, $1, $3); }
    | value "<" value {$$ = newBinaryOperator(BinaryOperatorCode::lt, $1, $3); }
    | value "==" value {$$ = newBinaryOperator(BinaryOperatorCode::eq, $1, $3); }
    | "(" value ")" { $$ = $2; }


NAME_LIST: NAME { $$ = std::vector<std::string>{$1}; }
    | NAME_LIST "," NAME { ($1).push_back($3); $$ = std::move($1); }


arguments: value { $$ = newList($1); }
    | arguments "," value { $1->append($3); $$ = std::move($1); }

expr: "CREATE" "TABLE" NAME "(" params ")" {$$ = newCreateQuery($3, $5); }
    | "DROP" "TABLE" NAME { $$ = newDropQuery($3);}
    | NAME { $$ = newStringLiteral($1); }
    | "INSERT" NAME "VALUES" "(" arguments ")" { $$ = newInsertQuery($2, $5); }
    | "SELECT" arguments { std::cout << "HERE\n"; $$ = newSelectQuery($2, {}, nullptr, nullptr, nullptr, nullptr); }
    | "SELECT" arguments { std::cout << "HERE\n"; $$ = newSelectQuery($2, {}, nullptr, nullptr, nullptr, nullptr); }
    | "SELECT" "*" "FROM" NAME_LIST { $$ = newSelectQuery(nullptr, $4, nullptr, nullptr, nullptr, nullptr); }
    | "SELECT" "*" "FROM" NAME_LIST "WHERE" expr { $$ = newSelectQuery(nullptr, $4, $6, nullptr, nullptr, nullptr); }
    | "SELECT" arguments "FROM" NAME_LIST {$$ = newSelectQuery($2, $4, nullptr, nullptr, nullptr, nullptr); }
    | "SELECT" arguments "FROM" NAME_LIST  "WHERE" expr {$$ = newSelectQuery($2, $4, $6, nullptr, nullptr, nullptr); }
    | "SELECT" arguments "FROM" NAME_LIST  "WHERE" expr {$$ = newSelectQuery($2, $4, $6, nullptr, nullptr, nullptr); }
    | "SELECT" "*" "FROM" NAME_LIST "ORDER" "BY" params_sorting { $$ = newSelectQuery(nullptr, $4, nullptr, nullptr, nullptr, $7); }
    | "SELECT" arguments "FROM" NAME_LIST "ORDER" "BY" params_sorting { $$ = newSelectQuery($2, $4, nullptr, nullptr, nullptr, $7); }
    | "SELECT" arguments "FROM" NAME_LIST  "WHERE" expr "ORDER" "BY" params_sorting {$$ = newSelectQuery($2, $4, $6, nullptr, nullptr, $9); }
    | value

%%

void shdb::Parser::error(const std::string& err)
{
	message = err;
}
