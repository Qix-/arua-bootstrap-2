#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream> // XXX DEBUG
#include <memory>
#include <vector>

#include "ast/module.h"
#include "ast/symbol-direct.h"
#include "ast/symbol-indirect.h"
#include "ast/type.h"
#include "ast/type-array.h"
#include "ast/type-scalar.h"
#include "ast/type-symbol.h"
#include "lexer.h"

// just used in the column math.
// in the final compiler, we will be able to specify this to get accurate columns.
// this will allow IDEs to automatically plug in the correct tab widths, too.
#define TAB_WIDTH 8

using namespace arua;
using namespace std;

enum arua_abt {
	ABT_EOF,
	ABT_WS,
	ABT_TAB,
	ABT_NL,
	ABT_OPAREN,
	ABT_CPAREN,
	ABT_OBRACKET,
	ABT_CBRACKET,
	ABT_RANGLE,
	ABT_LANGLE,
	ABT_ID,
	ABT_FN,
	ABT_RET,
	ABT_NUM,
	ABT_NUM_EXTENDED,
	ABT_DOT,
	ABT_USE,
	ABT_STR_BEGIN,
	ABT_STR_END,
	ABT_STR_LITERAL,
	ABT_STR_ESCAPE,
	ABT_AS,
	ABT_COMMA,
	ABT_PLUS,
	ABT_MINUS,
	ABT_DIVIDE,
	ABT_MULTIPLY,
	ABT_PUB,
	ABT_COMMENT,
	ABT_COMMENT_DOC,
	ABT_COMMENT_HEADER,
	ABT_STRUCT,
	ABT_EQUALS,
	ABT_ON,
	ABT_BE,
	ABT_TRAIT,
	ABT_TYPEDEF,
	ABT_ALIAS,
};

struct token {
	token(arua_abt type) : type(type) {}
	arua_abt type;
	unsigned int line;
	unsigned int col_start;
	unsigned int col_end;
};

struct token_val : public token {
	token_val(arua_abt type, string val) : token(type), val(val) {}
	string val;
};

typedef vector<shared_ptr<token>>::const_iterator tokenitr;

/*!re2c
	re2c:define:YYCTYPE = "unsigned char";
*/

/*!max:re2c*/
static const size_t SIZE = 64 * 1024;

// shamelessly copied from http://re2c.org/examples/example_07.html
// modified to be a bit more contained.
// I chose to keep the old cstdio fopen()/fread()/etc functions here
// instead of streams since they seem to be lacking a "read up to this many
// bytes and return the number of bytes you /actually/ read" function.
struct input {
	unsigned char buf[SIZE + YYMAXFILL];
	unsigned char *lim;
	unsigned char *cur;
	unsigned char *mar;
	unsigned char *tok;
	bool eof;
	vector<shared_ptr<token>> tokens;
	unsigned int line;
	unsigned int col;

	FILE *const file;

	input(string &filename)
			: buf()
			, lim(buf + SIZE)
			, cur(lim)
			, mar(lim)
			, tok(lim)
			, eof(false)
			, line(1)
			, col(1)
			, file(fopen(filename.c_str(), "rb")) {
		if (!file) {
			throw runtime_error("could not open file for reading: " + filename + "(" + strerror(errno) + ")");
		}
	}

	bool fill(size_t need) {
		if (eof) {
			return false;
		}

		const size_t free = tok - buf;
		if (free < need) {
			return false;
		}

		memmove(buf, tok, lim - tok);

		lim -= free;
		cur -= free;
		mar -= free;
		tok -= free;

		lim += fread(lim, 1, free, file);

		if (lim < buf + SIZE) {
			eof = true;
			memset(lim, 0, YYMAXFILL);
			lim += YYMAXFILL;
		}

		return true;
	}

	void push(token *tkn) {
		tkn->line = this->line;
		tkn->col_start = this->col;
		if (tkn->type == ABT_NL) {
			this->col = 1;
			++this->line;
			tkn->col_end = tkn->col_start;
		} else {
			this->col += this->cur - this->tok;
			tkn->col_end = this->col - 1;
		}
		this->tokens.push_back(shared_ptr<token>(tkn));
	}

	void push(arua_abt type) {
		this->push(new token(type));
	}

	void pushv(arua_abt type) {
		this->push(new token_val(type, string((const char *) this->tok, (size_t) (this->cur - this->tok))));
	}
};

void lex_string(input &in) {
	in.push(ABT_STR_BEGIN);
	for (;;) {
		in.tok = in.cur;
/*!re2c
		re2c:define:YYCURSOR = in.cur;
		re2c:define:YYMARKER = in.mar;
		re2c:define:YYLIMIT = in.lim;
		re2c:yyfill:enable = 1;
		re2c:define:YYFILL = "if (!in.fill(@@)) return;";
		re2c:define:YYFILL:naked = 1;

		* { return; } // TODO error message
		["] { break; }

		[^\r\n\\"]+ { in.pushv(ABT_STR_LITERAL); continue; }

		"\\"[\\"nrftavb] { in.pushv(ABT_STR_ESCAPE); continue; }
		"\\x"[^"]{0,2} { in.pushv(ABT_STR_ESCAPE); continue; }
		"\\u"[^"]{0,4} { in.pushv(ABT_STR_ESCAPE); continue; }
		"\\U"[^"]{0,8} { in.pushv(ABT_STR_ESCAPE); continue; }
*/
	}
	in.push(ABT_STR_END);
}

void lex_input(input &in) {
	for (;;) {
		in.tok = in.cur;
/*!re2c
		re2c:define:YYCURSOR = in.cur;
		re2c:define:YYMARKER = in.mar;
		re2c:define:YYLIMIT = in.lim;
		re2c:yyfill:enable = 1;
		re2c:define:YYFILL = "if (!in.fill(@@)) return;";
		re2c:define:YYFILL:naked = 1;

		* { in.push(ABT_EOF); return; } // TODO error message

		"#:" [^\r\n\f]* { in.pushv(ABT_COMMENT_DOC); continue; }
		"##" [^\r\n\f]* { in.pushv(ABT_COMMENT_HEADER); continue; }
		"#" [^\r\n\f]* { in.pushv(ABT_COMMENT); continue; }

		" " { in.push(ABT_WS); continue; }
		"\t" { in.push(ABT_TAB); continue; }
		"\r"?"\n" { in.push(ABT_NL); continue; }

		extended_form = ("0" | [1-9][0-9]*) "x" [a-zA-Z0-9]+;
		extended_form { in.pushv(ABT_NUM_EXTENDED); continue; }

		["] { lex_string(in); continue; }

		([0-9]+ "." [0-9]+) | ("." [0-9]+) | ([0-9]+ "."?) { in.pushv(ABT_NUM); continue; }

		"fn" { in.push(ABT_FN); continue; }
		"ret" { in.push(ABT_RET); continue; }
		"use" { in.push(ABT_USE); continue; }
		"as" { in.push(ABT_AS); continue; }
		"pub" { in.push(ABT_PUB); continue; }
		"struct" { in.push(ABT_STRUCT); continue; }
		"on" { in.push(ABT_ON); continue; }
		"be" { in.push(ABT_BE); continue; }
		"trait" { in.push(ABT_TRAIT); continue; }
		"type" { in.push(ABT_TYPEDEF); continue; }
		"alias" { in.push(ABT_ALIAS); continue; }

		"(" { in.push(ABT_OPAREN); continue; }
		")" { in.push(ABT_CPAREN); continue; }
		"[" { in.push(ABT_OBRACKET); continue; }
		"]" { in.push(ABT_CBRACKET); continue; }
		"<" { in.push(ABT_LANGLE); continue; }
		">" { in.push(ABT_RANGLE); continue; }
		"." { in.push(ABT_DOT); continue; }
		"," { in.push(ABT_COMMA); continue; }
		"+" { in.push(ABT_PLUS); continue; }
		"-" { in.push(ABT_MINUS); continue; }
		"/" { in.push(ABT_DIVIDE); continue; }
		"*" { in.push(ABT_MULTIPLY); continue; }
		"=" { in.push(ABT_EQUALS); continue; }

		id = [a-zA-Z_][a-zA-Z_0-9]*;
		id { in.pushv(ABT_ID); continue; }
*/
	}
}

void print_token(const shared_ptr<token> &tkn, bool human = false) {
	switch (tkn->type) {
	case ABT_EOF: cerr << (human ? "end of input" : ""); break;
	case ABT_WS: cerr << (human ? "whitespace" : " "); break;
	case ABT_TAB: cerr << (human ? "tab" : "\t"); break;
	case ABT_NL: cerr << (human ? "new line" : "\n"); break;
	case ABT_FN: cerr << "\x1b[36;1mfn\x1b[m"; break;
	case ABT_ID: cerr << ((token_val *)tkn.get())->val; break;
	case ABT_OPAREN: cerr << "\x1b[2m(\x1b[m"; break;
	case ABT_CPAREN: cerr << "\x1b[2m)\x1b[m"; break;
	case ABT_OBRACKET: cerr << "\x1b[2m[\x1b[m"; break;
	case ABT_CBRACKET: cerr << "\x1b[2m]\x1b[m"; break;
	case ABT_RANGLE: cerr << "\x1b[2m>\x1b[m"; break;
	case ABT_LANGLE: cerr << "\x1b[2m<\x1b[m"; break;
	case ABT_RET: cerr << "\x1b[36;1mret\x1b[m"; break;
	case ABT_NUM:
	case ABT_NUM_EXTENDED: cerr << "\x1b[31m" << ((token_val *)tkn.get())->val << "\x1b[m"; break;
	case ABT_DOT: cerr << "\x1b[2m.\x1b[m"; break;
	case ABT_USE: cerr << "\x1b[36;1muse\x1b[m"; break;
	case ABT_STR_BEGIN: cerr << "\x1b[38;5;208m\""; break;
	case ABT_STR_END: cerr << "\"\x1b[m"; break;
	case ABT_STR_LITERAL: cerr << ((token_val *)tkn.get())->val; break;
	case ABT_STR_ESCAPE: cerr << "\x1b[38;5;214;1m" << ((token_val *)tkn.get())->val << "\x1b[0;38;5;208m"; break;
	case ABT_AS: cerr << "\x1b[36;1mas\x1b[m"; break;
	case ABT_PUB: cerr << "\x1b[36;1mpub\x1b[m"; break;
	case ABT_STRUCT: cerr << "\x1b[36;1mstruct\x1b[m"; break;
	case ABT_ON: cerr << "\x1b[36;1mon\x1b[m"; break;
	case ABT_BE: cerr << "\x1b[36;1mbe\x1b[m"; break;
	case ABT_TRAIT: cerr << "\x1b[36;1mtrait\x1b[m"; break;
	case ABT_TYPEDEF: cerr << "\x1b[36;1mtype\x1b[m"; break;
	case ABT_ALIAS: cerr << "\x1b[36;1malias\x1b[m"; break;
	case ABT_COMMA: cerr << "\x1b[2m,\x1b[m"; break;
	case ABT_PLUS: cerr << "\x1b[2m+\x1b[m"; break;
	case ABT_MINUS: cerr << "\x1b[2m-\x1b[m"; break;
	case ABT_DIVIDE: cerr << "\x1b[2m/\x1b[m"; break;
	case ABT_MULTIPLY: cerr << "\x1b[2m*\x1b[m"; break;
	case ABT_EQUALS: cerr << "\x1b[2m=\x1b[m"; break;
	case ABT_COMMENT:
		if (human) {
			cerr << "comment";
		} else {
			cerr << "\x1b[38;5;242;2m" << ((token_val *)tkn.get())->val << "\x1b[m";
		}
		break;
	case ABT_COMMENT_DOC:
		if (human) {
			cerr << "documentation comment";
		} else {
			cerr << "\x1b[38;5;61;2m" << ((token_val *)tkn.get())->val << "\x1b[m";
		}
		break;
	case ABT_COMMENT_HEADER:
		if (human) {
			cerr << "header comment";
		} else {
			cerr << "\x1b[38;5;50;2m" << ((token_val *)tkn.get())->val << "\x1b[m";
		}
		break;
	}
}

void print_highlighted(input &in) {
	for (const shared_ptr<token> &tkn : in.tokens) {
		print_token(tkn);
	}

	cerr << "\x1b[m";
}

void print_location(const shared_ptr<token> &tkn) {
	cerr << "\x1b[7m{" << tkn->line << ":" << tkn->col_start << "-" << tkn->col_end << "}\x1b[m";
	print_token(tkn);
	cerr << "\x1b[m";
}

void print_locations(input &in) {
	for (const shared_ptr<token> &tkn : in.tokens) {
		print_location(tkn);
	}
}

bool unexpected(tokenitr &titr) {
	// TODO give to error handler
	// TODO maybe skip to next line on error here instead of blowing up on the first encountered parsing error?
	cerr << "ERROR: unexpected token ";
	print_token(*titr, true);
	cerr << " at " << (*titr)->line << ":" << (*titr)->col_start << endl;

	// always return false.
	return false;
}

bool burn(tokenitr &titr) {
	for (;; ++titr) {
		switch ((*titr)->type) {
		case ABT_COMMENT:
		case ABT_COMMENT_DOC:
		case ABT_COMMENT_HEADER:
		case ABT_NL:
			break;
		default:
			return true;
		}
	}
}

bool parse_whitespace(tokenitr &titr) {
	bool found = false;
	while ((*titr)->type == ABT_WS) {
		found = true;
		++titr;
	}
	return found ? true : unexpected(titr);
}

inline bool expect(tokenitr &titr, arua_abt type) {
	if ((*titr)->type == type) {
		++titr;
		return true;
	} else {
		return false;
	}
}

bool parse_identifier(tokenitr &titr, shared_ptr<Identifier> &id) {
	if ((*titr)->type != ABT_ID) return unexpected(titr);

	id.reset(new Identifier((*titr)->line, (*titr)->col_start, ((token_val *)&*titr)->val));
	++titr;
	return true;
}

bool parse_type(tokenitr &titr, shared_ptr<Type> &type, shared_ptr<SymbolContext> symCtx);

bool parse_type_array(tokenitr &titr, shared_ptr<Type> &type, shared_ptr<SymbolContext> symCtx) {
	unsigned int depth = 0;

	while (expect(titr, ABT_OBRACKET)) {
		++depth;
	}

	shared_ptr<Type> innerType;
	if (!parse_type(titr, innerType, symCtx)) return false;

	for (unsigned int i = 0; i < depth; i++) {
		if (!expect(titr, ABT_CBRACKET)) return unexpected(titr);
	}

	type.reset(new TypeArray(innerType, depth));
	return true;
}

bool parse_type_symbol_or_scalar(tokenitr &titr, shared_ptr<Type> &type, shared_ptr<SymbolContext> symCtx) {
	shared_ptr<SymbolIndirect> indirect(new SymbolIndirect(symCtx));

	// get first identifier
	shared_ptr<Identifier> firstId;
	if (!parse_identifier(titr, firstId)) return false;
	indirect->addIdentifier(firstId);

	// get more
	bool single = true;
	while (expect(titr, ABT_DOT)) {
		shared_ptr<Identifier> nextId;
		if (!parse_identifier(titr, nextId)) return unexpected(titr);
		single = false;
		indirect->addIdentifier(nextId);
	}

	if (!single) {
		type.reset(new TypeSymbol(indirect));
		return true;
	}

	// scalar?
	string firstStr = firstId->getIdentifier();
	ScalarClass scalarClass;
	switch (firstStr[0]) {
	case 'u':
		scalarClass = ScalarClass::UINT;
		break;
	case 'i':
		scalarClass = ScalarClass::INT;
		break;
	case 'f':
		scalarClass = ScalarClass::FLOAT;
		break;
	default:
		goto notAScalar;
	}

	{
		string widthStr = firstStr.substr(1);
		if (firstStr.length() > 1 && all_of(widthStr.begin(), widthStr.end(), ::isdigit)) {
			type.reset(new TypeScalar((*titr)->line, (*titr)->col_start, (*titr)->col_end, scalarClass, ::atoi(widthStr.c_str())));
		}
	}

	notAScalar:
	type.reset(new TypeSymbol(shared_ptr<Symbol>(new SymbolDirect(symCtx, firstId))));
	return true;
}

bool parse_type(tokenitr &titr, shared_ptr<Type> &type, shared_ptr<SymbolContext> symCtx) {
	// parse arrays
	if ((*titr)->type == ABT_OBRACKET) {
		return parse_type_array(titr, type, symCtx);
	}

	// parse functions
	if ((*titr)->type == ABT_FN) {
		// TODO not entirely sure about this yet.
		cerr << "NOTICE (fatal): function type (outside of declarations) are not yes supported (at "
			<< (*titr)->line << ":" << (*titr)->col_start << ")" << endl;
		return false;
	}

	return parse_type_symbol_or_scalar(titr, type, symCtx);
}

bool parse_typedef(tokenitr &titr, shared_ptr<Module> module) {
	auto beginToken = *titr;

	if (!expect(titr, ABT_TYPEDEF)) return unexpected(titr);
	if (!parse_whitespace(titr)) return false;

	shared_ptr<Type> baseType;
	if (!parse_type(titr, baseType, module)) {
		return false;
	}

	if (!parse_whitespace(titr)) return false;
	if (!expect(titr, ABT_AS)) return unexpected(titr);
	if (!parse_whitespace(titr)) return false;

	shared_ptr<Identifier> name;
	if (!parse_identifier(titr, name)) return false;

	if (!expect(titr, ABT_NL)) return false;

	module->addType(name, baseType);

	return true;
}

bool parse_module(tokenitr &titr, shared_ptr<Module> module) {
	while ((*titr)->type != ABT_EOF) {
		burn(titr);
		switch ((*titr)->type) {
		case ABT_TYPEDEF:
			if (!parse_typedef(titr, module)) return false;
			break;
		default:
			return unexpected(titr);
		}
	}

	return true;
}

shared_ptr<Module> arua::lex_file(string &filename) {
	input in(filename);
	lex_input(in);
	print_highlighted(in);
	cerr << endl << "-----------------------------" << endl << endl;
	print_locations(in);
	cerr << endl << endl;

	shared_ptr<Module> module(new Module(filename));
	tokenitr titr = in.tokens.cbegin();
	return parse_module(titr, module) ? module : shared_ptr<Module>(nullptr);
}
