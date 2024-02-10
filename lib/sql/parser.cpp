// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "parser.hpp"


// Unqualified %code blocks.
#line 18 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"

    #include "lexer.h"
    #define yylex lexer.lex

#line 50 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 4 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
namespace shdb {
#line 123 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"


  /// Build a parser object.
  Parser::Parser (shdb::Lexer& lexer_yyarg, std::shared_ptr<IAST>& result_yyarg, std::string& message_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      result (result_yyarg),
      message (message_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
  {
    switch (this->type_get ())
    {
      case 40: // arguments
        value.move< ASTListPtr > (std::move (that.value));
        break;

      case 37: // params_sorting
        value.move< ASTSortingListPtr > (std::move (that.value));
        break;

      case 36: // params
        value.move< Schema > (std::move (that.value));
        break;

      case 5: // NUM
        value.move< int > (std::move (that.value));
        break;

      case 38: // value
      case 41: // expr
        value.move< std::shared_ptr<IAST> > (std::move (that.value));
        break;

      case 4: // NAME
        value.move< std::string > (std::move (that.value));
        break;

      case 39: // NAME_LIST
        value.move< std::vector<std::string> > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->type_get ())
    {
      case 40: // arguments
        value.copy< ASTListPtr > (YY_MOVE (that.value));
        break;

      case 37: // params_sorting
        value.copy< ASTSortingListPtr > (YY_MOVE (that.value));
        break;

      case 36: // params
        value.copy< Schema > (YY_MOVE (that.value));
        break;

      case 5: // NUM
        value.copy< int > (YY_MOVE (that.value));
        break;

      case 38: // value
      case 41: // expr
        value.copy< std::shared_ptr<IAST> > (YY_MOVE (that.value));
        break;

      case 4: // NAME
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case 39: // NAME_LIST
        value.copy< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 40: // arguments
        value.move< ASTListPtr > (YY_MOVE (s.value));
        break;

      case 37: // params_sorting
        value.move< ASTSortingListPtr > (YY_MOVE (s.value));
        break;

      case 36: // params
        value.move< Schema > (YY_MOVE (s.value));
        break;

      case 5: // NUM
        value.move< int > (YY_MOVE (s.value));
        break;

      case 38: // value
      case 41: // expr
        value.move< std::shared_ptr<IAST> > (YY_MOVE (s.value));
        break;

      case 4: // NAME
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case 39: // NAME_LIST
        value.move< std::vector<std::string> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_type.
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  Parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  Parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_number_type
  Parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 40: // arguments
        value.YY_MOVE_OR_COPY< ASTListPtr > (YY_MOVE (that.value));
        break;

      case 37: // params_sorting
        value.YY_MOVE_OR_COPY< ASTSortingListPtr > (YY_MOVE (that.value));
        break;

      case 36: // params
        value.YY_MOVE_OR_COPY< Schema > (YY_MOVE (that.value));
        break;

      case 5: // NUM
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 38: // value
      case 41: // expr
        value.YY_MOVE_OR_COPY< std::shared_ptr<IAST> > (YY_MOVE (that.value));
        break;

      case 4: // NAME
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 39: // NAME_LIST
        value.YY_MOVE_OR_COPY< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 40: // arguments
        value.move< ASTListPtr > (YY_MOVE (that.value));
        break;

      case 37: // params_sorting
        value.move< ASTSortingListPtr > (YY_MOVE (that.value));
        break;

      case 36: // params
        value.move< Schema > (YY_MOVE (that.value));
        break;

      case 5: // NUM
        value.move< int > (YY_MOVE (that.value));
        break;

      case 38: // value
      case 41: // expr
        value.move< std::shared_ptr<IAST> > (YY_MOVE (that.value));
        break;

      case 4: // NAME
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 39: // NAME_LIST
        value.move< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 40: // arguments
        value.copy< ASTListPtr > (that.value);
        break;

      case 37: // params_sorting
        value.copy< ASTSortingListPtr > (that.value);
        break;

      case 36: // params
        value.copy< Schema > (that.value);
        break;

      case 5: // NUM
        value.copy< int > (that.value);
        break;

      case 38: // value
      case 41: // expr
        value.copy< std::shared_ptr<IAST> > (that.value);
        break;

      case 4: // NAME
        value.copy< std::string > (that.value);
        break;

      case 39: // NAME_LIST
        value.copy< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 40: // arguments
        value.move< ASTListPtr > (that.value);
        break;

      case 37: // params_sorting
        value.move< ASTSortingListPtr > (that.value);
        break;

      case 36: // params
        value.move< Schema > (that.value);
        break;

      case 5: // NUM
        value.move< int > (that.value);
        break;

      case 38: // value
      case 41: // expr
        value.move< std::shared_ptr<IAST> > (that.value);
        break;

      case 4: // NAME
        value.move< std::string > (that.value);
        break;

      case 39: // NAME_LIST
        value.move< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 40: // arguments
        yylhs.value.emplace< ASTListPtr > ();
        break;

      case 37: // params_sorting
        yylhs.value.emplace< ASTSortingListPtr > ();
        break;

      case 36: // params
        yylhs.value.emplace< Schema > ();
        break;

      case 5: // NUM
        yylhs.value.emplace< int > ();
        break;

      case 38: // value
      case 41: // expr
        yylhs.value.emplace< std::shared_ptr<IAST> > ();
        break;

      case 4: // NAME
        yylhs.value.emplace< std::string > ();
        break;

      case 39: // NAME_LIST
        yylhs.value.emplace< std::vector<std::string> > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 75 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                 { result = yystack_[1].value.as < std::shared_ptr<IAST> > (); }
#line 820 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 3:
#line 76 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
           {result = yystack_[0].value.as < std::shared_ptr<IAST> > (); }
#line 826 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 4:
#line 78 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                   { 
        ColumnSchema elem;
        if (yystack_[0].value.as < std::string > () == "string") {
            elem = ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::string, 0};
        } else if (yystack_[0].value.as < std::string > () == "uint64") {
            std::cout << "incorrect type!";
            elem = ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::uint64, 0};
        } else if (yystack_[0].value.as < std::string > () == "int64") {
            std::cout << "correct type!";
            elem = ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::int64, 0};
        } else if (yystack_[0].value.as < std::string > () == "boolean") {
            elem = ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::boolean, 0};
        }
        yylhs.value.as < Schema > () = Schema{elem};
    }
#line 846 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 5:
#line 93 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                           {
        if (yystack_[0].value.as < std::string > () == "string") {
            yystack_[3].value.as < Schema > ().push_back(ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::string, 0});
        } else if (yystack_[0].value.as < std::string > () == "uint64") {
            std::cout << "incorrect type!";
            yystack_[3].value.as < Schema > ().push_back(ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::uint64, 0});
        } else if (yystack_[0].value.as < std::string > () == "int64") {
            std::cout << "correct type!";
            yystack_[3].value.as < Schema > ().push_back(ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::int64, 0});
        } else if (yystack_[0].value.as < std::string > () == "boolean") {
            yystack_[3].value.as < Schema > ().push_back(ColumnSchema{yystack_[1].value.as < std::string > (), shdb::Type::boolean, 0});
        }
        yylhs.value.as < Schema > () = std::move(yystack_[3].value.as < Schema > ()); 
    }
#line 865 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 6:
#line 107 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                 {
        ColumnSchema elem{yystack_[4].value.as < std::string > (), shdb::Type::varchar, yystack_[1].value.as < int > ()};
        yylhs.value.as < Schema > () = Schema{elem};
    }
#line 874 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 7:
#line 111 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                            {
        yystack_[6].value.as < Schema > ().push_back(ColumnSchema{yystack_[4].value.as < std::string > (), shdb::Type::varchar, yystack_[1].value.as < int > ()});
        yylhs.value.as < Schema > () = std::move(yystack_[6].value.as < Schema > ()); 
    }
#line 883 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 8:
#line 117 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                       { yylhs.value.as < ASTSortingListPtr > () = newSortingList(yystack_[0].value.as < std::shared_ptr<IAST> > (), false); }
#line 889 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 9:
#line 118 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                   { yylhs.value.as < ASTSortingListPtr > () = newSortingList(yystack_[1].value.as < std::shared_ptr<IAST> > (), true); }
#line 895 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 10:
#line 119 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                               { yystack_[2].value.as < ASTSortingListPtr > ()->append(yystack_[0].value.as < std::shared_ptr<IAST> > (), false); yylhs.value.as < ASTSortingListPtr > () = std::move(yystack_[2].value.as < ASTSortingListPtr > ()); }
#line 901 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 11:
#line 120 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                      { yystack_[3].value.as < ASTSortingListPtr > ()->append(yystack_[1].value.as < std::shared_ptr<IAST> > (), true); yylhs.value.as < ASTSortingListPtr > () = std::move(yystack_[3].value.as < ASTSortingListPtr > ()); }
#line 907 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 12:
#line 124 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
            { yylhs.value.as < std::shared_ptr<IAST> > () = newStringLiteral(yystack_[0].value.as < std::string > ()); }
#line 913 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 13:
#line 125 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
          { yylhs.value.as < std::shared_ptr<IAST> > () = newNumberLiteral(yystack_[0].value.as < int > ()); }
#line 919 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 14:
#line 126 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
              { yylhs.value.as < std::shared_ptr<IAST> > () = newNumberLiteral(-yystack_[0].value.as < int > ()); }
#line 925 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 15:
#line 127 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                     { yylhs.value.as < std::shared_ptr<IAST> > () = newStringLiteral(yystack_[1].value.as < std::string > ()); }
#line 931 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 16:
#line 128 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                      {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::plus, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 937 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 17:
#line 129 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                      {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::minus, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 943 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 18:
#line 130 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                      {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::mul, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 949 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 19:
#line 131 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                      {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::div, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 955 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 20:
#line 132 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                       {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::le, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 961 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 21:
#line 133 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                       {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::ge, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 967 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 22:
#line 134 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                      {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::gt, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 973 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 23:
#line 135 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                      {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::lt, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 979 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 24:
#line 136 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                       {yylhs.value.as < std::shared_ptr<IAST> > () = newBinaryOperator(BinaryOperatorCode::eq, yystack_[2].value.as < std::shared_ptr<IAST> > (), yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 985 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 25:
#line 137 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                    { yylhs.value.as < std::shared_ptr<IAST> > () = yystack_[1].value.as < std::shared_ptr<IAST> > (); }
#line 991 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 26:
#line 140 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                { yylhs.value.as < std::vector<std::string> > () = std::vector<std::string>{yystack_[0].value.as < std::string > ()}; }
#line 997 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 27:
#line 141 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                         { (yystack_[2].value.as < std::vector<std::string> > ()).push_back(yystack_[0].value.as < std::string > ()); yylhs.value.as < std::vector<std::string> > () = std::move(yystack_[2].value.as < std::vector<std::string> > ()); }
#line 1003 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 28:
#line 144 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                 { yylhs.value.as < ASTListPtr > () = newList(yystack_[0].value.as < std::shared_ptr<IAST> > ()); }
#line 1009 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 29:
#line 145 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                          { yystack_[2].value.as < ASTListPtr > ()->append(yystack_[0].value.as < std::shared_ptr<IAST> > ()); yylhs.value.as < ASTListPtr > () = std::move(yystack_[2].value.as < ASTListPtr > ()); }
#line 1015 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 30:
#line 147 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                           {yylhs.value.as < std::shared_ptr<IAST> > () = newCreateQuery(yystack_[3].value.as < std::string > (), yystack_[1].value.as < Schema > ()); }
#line 1021 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 31:
#line 148 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                          { yylhs.value.as < std::shared_ptr<IAST> > () = newDropQuery(yystack_[0].value.as < std::string > ());}
#line 1027 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 32:
#line 149 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
           { yylhs.value.as < std::shared_ptr<IAST> > () = newStringLiteral(yystack_[0].value.as < std::string > ()); }
#line 1033 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 33:
#line 150 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                               { yylhs.value.as < std::shared_ptr<IAST> > () = newInsertQuery(yystack_[4].value.as < std::string > (), yystack_[1].value.as < ASTListPtr > ()); }
#line 1039 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 34:
#line 151 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                         { std::cout << "HERE\n"; yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(yystack_[0].value.as < ASTListPtr > (), {}, nullptr, nullptr, nullptr, nullptr); }
#line 1045 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 35:
#line 152 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                         { std::cout << "HERE\n"; yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(yystack_[0].value.as < ASTListPtr > (), {}, nullptr, nullptr, nullptr, nullptr); }
#line 1051 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 36:
#line 153 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                    { yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(nullptr, yystack_[0].value.as < std::vector<std::string> > (), nullptr, nullptr, nullptr, nullptr); }
#line 1057 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 37:
#line 154 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                                 { yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(nullptr, yystack_[2].value.as < std::vector<std::string> > (), yystack_[0].value.as < std::shared_ptr<IAST> > (), nullptr, nullptr, nullptr); }
#line 1063 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 38:
#line 155 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                          {yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(yystack_[2].value.as < ASTListPtr > (), yystack_[0].value.as < std::vector<std::string> > (), nullptr, nullptr, nullptr, nullptr); }
#line 1069 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 39:
#line 156 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                                        {yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(yystack_[4].value.as < ASTListPtr > (), yystack_[2].value.as < std::vector<std::string> > (), yystack_[0].value.as < std::shared_ptr<IAST> > (), nullptr, nullptr, nullptr); }
#line 1075 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 40:
#line 157 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                                        {yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(yystack_[4].value.as < ASTListPtr > (), yystack_[2].value.as < std::vector<std::string> > (), yystack_[0].value.as < std::shared_ptr<IAST> > (), nullptr, nullptr, nullptr); }
#line 1081 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 41:
#line 158 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                                                { yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(nullptr, yystack_[3].value.as < std::vector<std::string> > (), nullptr, nullptr, nullptr, yystack_[0].value.as < ASTSortingListPtr > ()); }
#line 1087 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 42:
#line 159 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                                                      { yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(yystack_[5].value.as < ASTListPtr > (), yystack_[3].value.as < std::vector<std::string> > (), nullptr, nullptr, nullptr, yystack_[0].value.as < ASTSortingListPtr > ()); }
#line 1093 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 43:
#line 160 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
                                                                                    {yylhs.value.as < std::shared_ptr<IAST> > () = newSelectQuery(yystack_[7].value.as < ASTListPtr > (), yystack_[5].value.as < std::vector<std::string> > (), yystack_[3].value.as < std::shared_ptr<IAST> > (), nullptr, nullptr, yystack_[0].value.as < ASTSortingListPtr > ()); }
#line 1099 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;

  case 44:
#line 161 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
      { yylhs.value.as < std::shared_ptr<IAST> > () = yystack_[0].value.as < std::shared_ptr<IAST> > (); }
#line 1105 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"
    break;


#line 1109 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char Parser::yypact_ninf_ = -57;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
      22,   -57,   -57,    -2,    24,    29,    30,    32,    40,    37,
      48,    97,    31,    58,    59,    54,   -57,   -57,    85,    39,
      47,    97,    45,   -57,    32,    32,    32,    32,    32,    32,
      32,    32,    32,   -57,    49,   -57,    57,   -57,   -57,    72,
      72,    32,    27,    27,    52,    52,    -4,    -4,    -4,    -4,
      97,    82,    32,   -57,    25,    95,    97,    -3,   -13,   -12,
      22,    79,    83,    22,    80,   -57,    76,   -57,   116,   -57,
     -57,    32,   -57,   107,    32,   121,    -1,   103,    55,   113,
     103,   109,   -57,   108,    32,   -57,    32,   -57,   127,    73,
     103,   111,   -57,   -57
  };

  const signed char
  Parser::yydefact_[] =
  {
       0,    12,    13,     0,     0,     0,     0,     0,     0,     0,
       0,    44,     3,     0,     0,     0,    14,    12,     0,     0,
       0,    28,    34,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     0,    31,     0,    25,    15,     0,
       0,     0,    16,    17,    18,    19,    20,    21,    23,    22,
      24,     0,     0,    26,    36,    38,    29,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     0,    30,     0,    33,
      37,     0,    27,    39,     0,     0,     0,    41,     8,     0,
      42,     0,     5,     0,     0,     9,     0,     6,     0,    10,
      43,     0,    11,     7
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -57,   -57,   -57,    10,    -7,    94,    84,   -56
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,    10,    58,    77,    11,    54,    22,    12
  };

  const signed char
  Parser::yytable_[] =
  {
      18,    65,    21,    82,    70,    13,    66,    73,    83,    67,
      69,    68,    41,    24,    25,    26,    27,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     1,     2,     3,    32,
       4,    14,     5,    15,    56,    16,    17,     2,    60,    61,
       6,    17,     2,     7,    19,    21,    26,    27,    23,    62,
       6,     8,     9,     7,    33,     6,    20,    40,     7,    39,
      32,     8,    34,    35,    78,    36,     8,    78,    38,    41,
      51,    85,    24,    25,    26,    27,    53,    89,    52,    78,
      28,    29,    30,    31,    80,    32,    57,    72,    32,    92,
      24,    25,    26,    27,    71,    74,    90,    75,    28,    29,
      30,    31,    24,    25,    26,    27,    32,    37,    63,    64,
      28,    29,    30,    31,    24,    25,    26,    27,    32,    62,
      76,    79,    28,    29,    30,    31,    81,    84,    86,    88,
      32,    87,    91,    93,    55,     0,    59
  };

  const signed char
  Parser::yycheck_[] =
  {
       7,     4,     9,     4,    60,     7,     9,    63,     9,    22,
      22,    24,    24,    17,    18,    19,    20,    24,    25,    26,
      27,    28,    29,    30,    31,    32,     4,     5,     6,    33,
       8,     7,    10,     4,    41,     5,     4,     5,    13,    14,
      18,     4,     5,    21,     4,    52,    19,    20,     0,    24,
      18,    29,    30,    21,    23,    18,    19,    12,    21,    12,
      33,    29,     4,     4,    71,    11,    29,    74,    29,    24,
      21,    16,    17,    18,    19,    20,     4,    84,    21,    86,
      25,    26,    27,    28,    74,    33,     4,     4,    33,    16,
      17,    18,    19,    20,    15,    15,    86,    21,    25,    26,
      27,    28,    17,    18,    19,    20,    33,    22,    13,    14,
      25,    26,    27,    28,    17,    18,    19,    20,    33,    24,
       4,    14,    25,    26,    27,    28,     5,    24,    15,    21,
      33,    22,     5,    22,    40,    -1,    52
  };

  const signed char
  Parser::yystos_[] =
  {
       0,     4,     5,     6,     8,    10,    18,    21,    29,    30,
      35,    38,    41,     7,     7,     4,     5,     4,    38,     4,
      19,    38,    40,     0,    17,    18,    19,    20,    25,    26,
      27,    28,    33,    23,     4,     4,    11,    22,    29,    12,
      12,    24,    38,    38,    38,    38,    38,    38,    38,    38,
      38,    21,    21,     4,    39,    39,    38,     4,    36,    40,
      13,    14,    24,    13,    14,     4,     9,    22,    24,    22,
      41,    15,     4,    41,    15,    21,     4,    37,    38,    14,
      37,     5,     4,     9,    24,    16,    15,    22,    21,    38,
      37,     5,    16,    22
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    34,    35,    35,    36,    36,    36,    36,    37,    37,
      37,    37,    38,    38,    38,    38,    38,    38,    38,    38,
      38,    38,    38,    38,    38,    38,    39,    39,    40,    40,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    41
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     2,     1,     2,     4,     5,     7,     1,     2,
       3,     4,     1,     1,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       6,     3,     1,     6,     2,     2,     4,     6,     4,     6,
       6,     7,     7,     9,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "ERROR", "NAME", "NUM",
  "\"CREATE\"", "\"TABLE\"", "\"DROP\"", "\"varchar\"", "\"INSERT\"",
  "\"VALUES\"", "\"FROM\"", "\"WHERE\"", "\"ORDER\"", "\"BY\"", "\"DESC\"",
  "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"(\"", "\")\"", "\"\\n\"", "\",\"",
  "\"<=\"", "\">=\"", "\"<\"", "\">\"", "\"\\\"\"", "\"SELECT\"", "UMINUS",
  "UNOT", "\"==\"", "$accept", "input", "params", "params_sorting",
  "value", "NAME_LIST", "arguments", "expr", YY_NULLPTR
  };


  const unsigned char
  Parser::yyrline_[] =
  {
       0,    75,    75,    76,    78,    93,   107,   111,   117,   118,
     119,   120,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   140,   141,   144,   145,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
    };
    const int user_token_number_max_ = 288;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

#line 4 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"
} // shdb
#line 1512 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.cpp"

#line 163 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/parser.y"


void shdb::Parser::error(const std::string& err)
{
	message = err;
}
