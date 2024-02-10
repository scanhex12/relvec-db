#include "lexer.h"

namespace shdb {

%%{
    machine lexer;
    ident = (alpha | '_') (alpha | digit | '_')*;

    main := |*
    
        digit+ => {
            ret = Parser::token::NUM;
            Parser::semantic_type num(strtol(std::string(ts, te).c_str(), 0, 10));
            val->move<int>(num);
            fbreak;
        };

        (ident)* => {
            ret = Parser::token::NAME;
            Parser::semantic_type str(std::string(ts, te));
            std::string identifier(ts, te);
            if (identifier == "CREATE")
                ret = Parser::token::CREATE;
            else if (identifier == "TABLE")
                ret = Parser::token::TABLE;
            else if (identifier == "DROP")
                ret = Parser::token::DROP;
            else if (identifier == "varchar")
                ret = Parser::token::VARCHAR;
            else if (identifier == "INSERT")
                ret = Parser::token::INSERT;
            else if (identifier == "VALUES")
                ret = Parser::token::VALUES;
            else if (identifier == "SELECT")
                ret = Parser::token::SELECT;
            else if (identifier == "FROM")
                ret = Parser::token::FROM;
            else if (identifier == "WHERE")
                ret = Parser::token::WHERE;
            else if (identifier == "ORDER")
                ret = Parser::token::ORDER;
            else if (identifier == "BY")
                ret = Parser::token::BY;
            else if (identifier == "DESC")
                ret = Parser::token::DESC;
            val->move<std::string>(str);
            fbreak;
        };

        ',' => { ret = Parser::token::COMA; fbreak;};
        '"' => { ret = Parser::token::QUOTES; fbreak;};
        '(' => { ret = Parser::token::LPAR; fbreak; };
        ')' => { ret = Parser::token::RPAR; fbreak; };
        '\n' => { ret = Parser::token::EOL; fbreak; };
        '+' => { ret = Parser::token::PLUS; fbreak; };
        '-' => { ret = Parser::token::MINUS; fbreak; };
        '*' => { ret = Parser::token::MUL; fbreak; };
        '/' => { ret = Parser::token::DIV; fbreak; };
        '<=' => { ret = Parser::token::LE; fbreak; };
        '<' => { ret = Parser::token::LT; fbreak; };
        '>=' => { ret = Parser::token::GE; fbreak; };
        '>' => { ret = Parser::token::GT; fbreak; };
        '==' => { ret = Parser::token::GT; fbreak; };


        'CREATE' => { ret = Parser::token::CREATE; fbreak; };
        'TABLE' => { ret = Parser::token::TABLE; fbreak; };
        'DROP' => { ret = Parser::token::DROP; fbreak; };
        'varchar' => { ret = Parser::token::VARCHAR; fbreak; };
        'INSERT' => { ret = Parser::token::INSERT; fbreak; };
        'VALUES' => { ret = Parser::token::VALUES; fbreak; };
        'SELECT' => { ret = Parser::token::SELECT; fbreak; };
        'ORDER' => { ret = Parser::token::ORDER; fbreak; };
        'BY' => { ret = Parser::token::BY; fbreak; };
        'DESC' => { ret = Parser::token::DESC; fbreak; };

        space;
    *|;
    write data;
}%%


Lexer::Lexer(const char *p, const char *pe)
    : p(p), pe(pe), eof(pe)
{
    %%write init;
}

Parser::token_type Lexer::lex(Parser::semantic_type* val)
{
    Parser::token_type ret = Parser::token::END;
    %%write exec;

    if (ret == Parser::token::END && p != pe && te != pe) {
        std::cerr << "Unexpected input: \"" << std::string(te, pe) << "\"" << std::endl;
        ret = Parser::token::ERROR;
    }

    return ret;
}

}    // namespace shdb
