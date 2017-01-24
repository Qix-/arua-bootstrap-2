#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream> // XXX DEBUG
#include <vector>

#include "lexer.h"

using namespace arua;
using namespace std;

enum arua_abt {
	ABT_WS,
	ABT_TAB,
	ABT_NL,
	ABT_OPAREN,
	ABT_CPAREN,
	ABT_OBRACKET,
	ABT_CBRACKET,
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
};

struct token {
	token(arua_abt type) : type(type) {}
	arua_abt type;
	unsigned int line;
};

struct token_val : public token {
	token_val(arua_abt type, string val) : token(type), val(val) {}
	string val;
};

/*!re2c
	re2c:define:YYCTYPE = "unsigned char";
*/

/*!max:re2c*/
static const size_t SIZE = 64 * 1024;

#define LAST_TOKEN

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

	FILE *const file;

	input(string &filename)
			: buf()
			, lim(buf + SIZE)
			, cur(lim)
			, mar(lim)
			, tok(lim)
			, eof(false)
			, line(1)
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
		if (tkn->type == ABT_NL) {
			++this->line;
		}
		this->tokens.push_back(shared_ptr<token>(tkn));
	}

	void push(arua_abt type) {
		this->push(new token(type));
	}

	void push(arua_abt type, input &in) {
		this->push(new token_val(type, string((const char *) in.tok, (size_t) (in.cur - in.tok))));
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

		[^\r\n\\"]+ { in.push(ABT_STR_LITERAL, in); continue; }

		"\\"[\\"nrftavb] { in.push(ABT_STR_ESCAPE, in); continue; }
		"\\x"[^"]{0,2} { in.push(ABT_STR_ESCAPE, in); continue; }
		"\\u"[^"]{0,4} { in.push(ABT_STR_ESCAPE, in); continue; }
		"\\U"[^"]{0,8} { in.push(ABT_STR_ESCAPE, in); continue; }
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

		* { return; } // TODO error message

		"#:" [^\r\n\f]* { in.push(ABT_COMMENT_DOC, in); continue; }
		"##" [^\r\n\f]* { in.push(ABT_COMMENT_HEADER, in); continue; }
		"#" [^\r\n\f]* { in.push(ABT_COMMENT, in); continue; }

		" " { in.push(ABT_WS); continue; }
		"\t" { in.push(ABT_TAB); continue; }
		"\r"?"\n" { in.push(ABT_NL); continue; }

		extended_form = ("0" | [1-9][0-9]*) "x" [a-zA-Z0-9]+;
		extended_form { in.push(ABT_NUM_EXTENDED, in); continue; }

		["] { lex_string(in); continue; }

		([0-9]+ "." [0-9]+) | ("." [0-9]+) | ([0-9]+ "."?) { in.push(ABT_NUM, in); continue; }

		"fn" { in.push(ABT_FN); continue; }
		"ret" { in.push(ABT_RET); continue; }
		"use" { in.push(ABT_USE); continue; }
		"as" { in.push(ABT_AS); continue; }
		"pub" { in.push(ABT_PUB); continue; }
		"struct" { in.push(ABT_STRUCT); continue; }
		"on" { in.push(ABT_ON); continue; }
		"be" { in.push(ABT_BE); continue; }
		"trait" { in.push(ABT_TRAIT); continue; }

		"(" { in.push(ABT_OPAREN); continue; }
		")" { in.push(ABT_CPAREN); continue; }
		"[" { in.push(ABT_OBRACKET); continue; }
		"]" { in.push(ABT_CBRACKET); continue; }
		"." { in.push(ABT_DOT); continue; }
		"," { in.push(ABT_COMMA); continue; }
		"+" { in.push(ABT_PLUS); continue; }
		"-" { in.push(ABT_MINUS); continue; }
		"/" { in.push(ABT_DIVIDE); continue; }
		"*" { in.push(ABT_MULTIPLY); continue; }
		"=" { in.push(ABT_EQUALS); continue; }

		id = [a-zA-Z_][a-zA-Z_0-9]*;
		id { in.push(ABT_ID, in); continue; }
*/
	}
}

void print_highlighted(input &in) {
	for (const shared_ptr<token> &tkn : in.tokens) {
		switch (tkn->type) {
		case ABT_WS: cerr << " "; continue;
		case ABT_TAB: cerr << "\t"; continue;
		case ABT_NL: cerr << "\n"; continue;
		case ABT_FN: cerr << "\x1b[36;1mfn\x1b[m"; continue;
		case ABT_ID: cerr << ((token_val *)tkn.get())->val; continue;
		case ABT_OPAREN: cerr << "\x1b[2m(\x1b[m"; continue;
		case ABT_CPAREN: cerr << "\x1b[2m)\x1b[m"; continue;
		case ABT_OBRACKET: cerr << "\x1b[2m[\x1b[m"; continue;
		case ABT_CBRACKET: cerr << "\x1b[2m]\x1b[m"; continue;
		case ABT_RET: cerr << "\x1b[36;1mret\x1b[m"; continue;
		case ABT_NUM:
		case ABT_NUM_EXTENDED: cerr << "\x1b[31m" << ((token_val *)tkn.get())->val << "\x1b[m"; continue;
		case ABT_DOT: cerr << "\x1b[2m.\x1b[m"; continue;
		case ABT_USE: cerr << "\x1b[36;1muse\x1b[m"; continue;
		case ABT_STR_BEGIN: cerr << "\x1b[38;5;208m\""; continue;
		case ABT_STR_END: cerr << "\"\x1b[m"; continue;
		case ABT_STR_LITERAL: cerr << ((token_val *)tkn.get())->val; continue;
		case ABT_STR_ESCAPE: cerr << "\x1b[38;5;214;1m" << ((token_val *)tkn.get())->val << "\x1b[0;38;5;208m"; continue;
		case ABT_AS: cerr << "\x1b[36;1mas\x1b[m"; continue;
		case ABT_PUB: cerr << "\x1b[36;1mpub\x1b[m"; continue;
		case ABT_STRUCT: cerr << "\x1b[36;1mstruct\x1b[m"; continue;
		case ABT_ON: cerr << "\x1b[36;1mon\x1b[m"; continue;
		case ABT_BE: cerr << "\x1b[36;1mbe\x1b[m"; continue;
		case ABT_TRAIT: cerr << "\x1b[36;1mtrait\x1b[m"; continue;
		case ABT_COMMA: cerr << "\x1b[2m,\x1b[m"; continue;
		case ABT_PLUS: cerr << "\x1b[2m+\x1b[m"; continue;
		case ABT_MINUS: cerr << "\x1b[2m-\x1b[m"; continue;
		case ABT_DIVIDE: cerr << "\x1b[2m/\x1b[m"; continue;
		case ABT_MULTIPLY: cerr << "\x1b[2m*\x1b[m"; continue;
		case ABT_EQUALS: cerr << "\x1b[2m=\x1b[m"; continue;
		case ABT_COMMENT: cerr << "\x1b[38;5;242;2m" << ((token_val *)tkn.get())->val << "\x1b[m"; continue;
		case ABT_COMMENT_DOC: cerr << "\x1b[38;5;61;2m" << ((token_val *)tkn.get())->val << "\x1b[m"; continue;
		case ABT_COMMENT_HEADER: cerr << "\x1b[38;5;50;2m" << ((token_val *)tkn.get())->val << "\x1b[m"; continue;
		}
	}

	cerr << "\x1b[m";
}

shared_ptr<Module> arua::lex_file(string &filename) {
	input in(filename);
	lex_input(in);
	print_highlighted(in);

	shared_ptr<Module> module(new Module(filename));
	return module;
}
