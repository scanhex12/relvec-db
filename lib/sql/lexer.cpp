
#line 1 "lexer.l"
#include "lexer.h"

namespace shdb {


#line 9 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/lexer.cpp"
static const char _lexer_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 1, 17, 1, 18
};

static const short _lexer_key_offsets[] = {
	0, 0, 1, 32, 34, 35, 36, 43, 
	50, 58, 66, 74, 82, 89, 98, 106, 
	113, 121, 128, 136, 144, 152, 160, 167, 
	175, 183, 191, 198, 206, 214, 222, 230, 
	237, 245, 253, 261, 268, 276, 284, 292, 
	300, 307, 315, 323, 331, 339, 347
};

static const char _lexer_trans_keys[] = {
	61, 10, 32, 34, 40, 41, 42, 43, 
	44, 45, 47, 60, 61, 62, 66, 67, 
	68, 73, 79, 83, 84, 86, 95, 118, 
	9, 13, 48, 57, 65, 90, 97, 122, 
	48, 57, 61, 61, 95, 48, 57, 65, 
	90, 97, 122, 95, 48, 57, 65, 90, 
	97, 122, 82, 95, 48, 57, 65, 90, 
	97, 122, 69, 95, 48, 57, 65, 90, 
	97, 122, 65, 95, 48, 57, 66, 90, 
	97, 122, 84, 95, 48, 57, 65, 90, 
	97, 122, 95, 48, 57, 65, 90, 97, 
	122, 69, 82, 95, 48, 57, 65, 90, 
	97, 122, 83, 95, 48, 57, 65, 90, 
	97, 122, 95, 48, 57, 65, 90, 97, 
	122, 79, 95, 48, 57, 65, 90, 97, 
	122, 95, 48, 57, 65, 90, 97, 122, 
	78, 95, 48, 57, 65, 90, 97, 122, 
	83, 95, 48, 57, 65, 90, 97, 122, 
	69, 95, 48, 57, 65, 90, 97, 122, 
	82, 95, 48, 57, 65, 90, 97, 122, 
	95, 48, 57, 65, 90, 97, 122, 82, 
	95, 48, 57, 65, 90, 97, 122, 68, 
	95, 48, 57, 65, 90, 97, 122, 69, 
	95, 48, 57, 65, 90, 97, 122, 95, 
	48, 57, 65, 90, 97, 122, 69, 95, 
	48, 57, 65, 90, 97, 122, 76, 95, 
	48, 57, 65, 90, 97, 122, 69, 95, 
	48, 57, 65, 90, 97, 122, 67, 95, 
	48, 57, 65, 90, 97, 122, 95, 48, 
	57, 65, 90, 97, 122, 65, 95, 48, 
	57, 66, 90, 97, 122, 66, 95, 48, 
	57, 65, 90, 97, 122, 76, 95, 48, 
	57, 65, 90, 97, 122, 95, 48, 57, 
	65, 90, 97, 122, 65, 95, 48, 57, 
	66, 90, 97, 122, 76, 95, 48, 57, 
	65, 90, 97, 122, 85, 95, 48, 57, 
	65, 90, 97, 122, 69, 95, 48, 57, 
	65, 90, 97, 122, 95, 48, 57, 65, 
	90, 97, 122, 95, 97, 48, 57, 65, 
	90, 98, 122, 95, 114, 48, 57, 65, 
	90, 97, 122, 95, 99, 48, 57, 65, 
	90, 97, 122, 95, 104, 48, 57, 65, 
	90, 97, 122, 95, 97, 48, 57, 65, 
	90, 98, 122, 95, 48, 57, 65, 90, 
	97, 122, 0
};

static const char _lexer_single_lengths[] = {
	0, 1, 23, 0, 1, 1, 1, 1, 
	2, 2, 2, 2, 1, 3, 2, 1, 
	2, 1, 2, 2, 2, 2, 1, 2, 
	2, 2, 1, 2, 2, 2, 2, 1, 
	2, 2, 2, 1, 2, 2, 2, 2, 
	1, 2, 2, 2, 2, 2, 1
};

static const char _lexer_range_lengths[] = {
	0, 0, 4, 1, 0, 0, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3
};

static const short _lexer_index_offsets[] = {
	0, 0, 2, 30, 32, 34, 36, 41, 
	46, 52, 58, 64, 70, 75, 82, 88, 
	93, 99, 104, 110, 116, 122, 128, 133, 
	139, 145, 151, 156, 162, 168, 174, 180, 
	185, 191, 197, 203, 208, 214, 220, 226, 
	232, 237, 243, 249, 255, 261, 267
};

static const char _lexer_indicies[] = {
	0, 1, 3, 2, 4, 5, 6, 7, 
	8, 9, 10, 11, 13, 14, 15, 17, 
	18, 19, 20, 21, 22, 23, 24, 16, 
	25, 2, 12, 16, 16, 1, 12, 26, 
	28, 27, 30, 29, 16, 16, 16, 16, 
	31, 16, 16, 16, 16, 31, 32, 16, 
	16, 16, 16, 31, 33, 16, 16, 16, 
	16, 31, 34, 16, 16, 16, 16, 31, 
	35, 16, 16, 16, 16, 31, 16, 16, 
	16, 16, 31, 36, 37, 16, 16, 16, 
	16, 31, 38, 16, 16, 16, 16, 31, 
	16, 16, 16, 16, 31, 39, 16, 16, 
	16, 16, 31, 16, 16, 16, 16, 31, 
	40, 16, 16, 16, 16, 31, 41, 16, 
	16, 16, 16, 31, 42, 16, 16, 16, 
	16, 31, 43, 16, 16, 16, 16, 31, 
	16, 16, 16, 16, 31, 44, 16, 16, 
	16, 16, 31, 45, 16, 16, 16, 16, 
	31, 46, 16, 16, 16, 16, 31, 16, 
	16, 16, 16, 31, 47, 16, 16, 16, 
	16, 31, 48, 16, 16, 16, 16, 31, 
	49, 16, 16, 16, 16, 31, 50, 16, 
	16, 16, 16, 31, 16, 16, 16, 16, 
	31, 51, 16, 16, 16, 16, 31, 52, 
	16, 16, 16, 16, 31, 53, 16, 16, 
	16, 16, 31, 16, 16, 16, 16, 31, 
	54, 16, 16, 16, 16, 31, 55, 16, 
	16, 16, 16, 31, 56, 16, 16, 16, 
	16, 31, 57, 16, 16, 16, 16, 31, 
	16, 16, 16, 16, 31, 16, 58, 16, 
	16, 16, 31, 16, 59, 16, 16, 16, 
	31, 16, 60, 16, 16, 16, 31, 16, 
	61, 16, 16, 16, 31, 16, 62, 16, 
	16, 16, 31, 16, 16, 16, 16, 31, 
	0
};

static const char _lexer_trans_targs[] = {
	2, 0, 2, 2, 2, 2, 2, 2, 
	2, 2, 2, 2, 3, 4, 1, 5, 
	6, 7, 8, 13, 18, 23, 27, 32, 
	36, 41, 2, 2, 2, 2, 2, 2, 
	9, 10, 11, 12, 14, 16, 15, 17, 
	19, 20, 21, 22, 24, 25, 26, 28, 
	29, 30, 31, 33, 34, 35, 37, 38, 
	39, 40, 42, 43, 44, 45, 46
};

static const char _lexer_trans_actions[] = {
	27, 0, 29, 13, 7, 9, 11, 19, 
	15, 5, 17, 21, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 31, 35, 23, 37, 25, 33, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _lexer_to_state_actions[] = {
	0, 0, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _lexer_from_state_actions[] = {
	0, 0, 3, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const short _lexer_eof_trans[] = {
	0, 0, 0, 27, 28, 30, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32, 32, 
	32, 32, 32, 32, 32, 32, 32
};

static const int lexer_start = 2;
static const int lexer_first_final = 2;
static const int lexer_error = 0;

static const int lexer_en_main = 2;


#line 80 "lexer.l"



Lexer::Lexer(const char *p, const char *pe)
    : p(p), pe(pe), eof(pe)
{
    
#line 204 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/lexer.cpp"
	{
	cs = lexer_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 87 "lexer.l"
}

Parser::token_type Lexer::lex(Parser::semantic_type* val)
{
    Parser::token_type ret = Parser::token::END;
    
#line 219 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/lexer.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _lexer_actions + _lexer_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 1:
#line 1 "NONE"
	{ts = p;}
	break;
#line 240 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/lexer.cpp"
		}
	}

	_keys = _lexer_trans_keys + _lexer_key_offsets[cs];
	_trans = _lexer_index_offsets[cs];

	_klen = _lexer_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _lexer_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _lexer_indicies[_trans];
_eof_trans:
	cs = _lexer_trans_targs[_trans];

	if ( _lexer_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _lexer_actions + _lexer_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 2:
#line 50 "lexer.l"
	{te = p+1;{ ret = Parser::token::COMA; {p++; goto _out; }}}
	break;
	case 3:
#line 51 "lexer.l"
	{te = p+1;{ ret = Parser::token::QUOTES; {p++; goto _out; }}}
	break;
	case 4:
#line 52 "lexer.l"
	{te = p+1;{ ret = Parser::token::LPAR; {p++; goto _out; } }}
	break;
	case 5:
#line 53 "lexer.l"
	{te = p+1;{ ret = Parser::token::RPAR; {p++; goto _out; } }}
	break;
	case 6:
#line 54 "lexer.l"
	{te = p+1;{ ret = Parser::token::EOL; {p++; goto _out; } }}
	break;
	case 7:
#line 55 "lexer.l"
	{te = p+1;{ ret = Parser::token::PLUS; {p++; goto _out; } }}
	break;
	case 8:
#line 56 "lexer.l"
	{te = p+1;{ ret = Parser::token::MINUS; {p++; goto _out; } }}
	break;
	case 9:
#line 57 "lexer.l"
	{te = p+1;{ ret = Parser::token::MUL; {p++; goto _out; } }}
	break;
	case 10:
#line 58 "lexer.l"
	{te = p+1;{ ret = Parser::token::DIV; {p++; goto _out; } }}
	break;
	case 11:
#line 59 "lexer.l"
	{te = p+1;{ ret = Parser::token::LE; {p++; goto _out; } }}
	break;
	case 12:
#line 61 "lexer.l"
	{te = p+1;{ ret = Parser::token::GE; {p++; goto _out; } }}
	break;
	case 13:
#line 63 "lexer.l"
	{te = p+1;{ ret = Parser::token::GT; {p++; goto _out; } }}
	break;
	case 14:
#line 77 "lexer.l"
	{te = p+1;}
	break;
	case 15:
#line 11 "lexer.l"
	{te = p;p--;{
            ret = Parser::token::NUM;
            Parser::semantic_type num(strtol(std::string(ts, te).c_str(), 0, 10));
            val->move<int>(num);
            {p++; goto _out; }
        }}
	break;
	case 16:
#line 18 "lexer.l"
	{te = p;p--;{
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
            {p++; goto _out; }
        }}
	break;
	case 17:
#line 60 "lexer.l"
	{te = p;p--;{ ret = Parser::token::LT; {p++; goto _out; } }}
	break;
	case 18:
#line 62 "lexer.l"
	{te = p;p--;{ ret = Parser::token::GT; {p++; goto _out; } }}
	break;
#line 409 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/lexer.cpp"
		}
	}

_again:
	_acts = _lexer_actions + _lexer_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 0:
#line 1 "NONE"
	{ts = 0;}
	break;
#line 422 "/home/robertj/ysda/shad-db-real/shad-db1/shdb/src/lexer.cpp"
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _lexer_eof_trans[cs] > 0 ) {
		_trans = _lexer_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}

#line 93 "lexer.l"

    if (ret == Parser::token::END && p != pe && te != pe) {
        std::cerr << "Unexpected input: \"" << std::string(te, pe) << "\"" << std::endl;
        ret = Parser::token::ERROR;
    }

    return ret;
}

}    // namespace shdb
