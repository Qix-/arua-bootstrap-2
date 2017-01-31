#include <cstdio>
#include <cerrno>
#include <cstring>
#include <memory>
#include <list>

#include "token.h"
#include "lexer.h"

using namespace arua;
using namespace std;

/*!re2c
	re2c:define:YYCTYPE = "unsigned char";
*/

/*!max:re2c*/
static const size_t SIZE = 64 * 1024;

// shamelessly copied from http://re2c.org/examples/example_07.html
// modified to be a bit more contained.lex
// I chose to keep the old cstdio fopen()/fread()/etc functions here
// instead of streams since they seem to be lacking a "read up to this many
// bytes and return the number of bytes you /actually/ read" function.
struct Input {
	unsigned char buf[SIZE + YYMAXFILL];
	unsigned char *lim;
	unsigned char *cur;
	unsigned char *mar;
	unsigned char *tok;
	bool eof;
	shared_ptr<list<shared_ptr<const Token>>> tokens;
	unsigned int line;
	unsigned int col;
	unsigned int tabWidth;

	Input(unsigned int tabWidth)
			: buf()
			, lim(buf + SIZE)
			, cur(lim)
			, mar(lim)
			, tok(lim)
			, eof(false)
			, tokens(new list<shared_ptr<const Token>>())
			, line(1)
			, col(1)
			, tabWidth(tabWidth) {
	}

	void push(TokenType type, string val = "") {
		shared_ptr<Token> tkn(new Token());

		tkn->type = type;
		tkn->value = val;
		tkn->line = this->line;
		tkn->columnStart = this->col;

		if (type == ABT_NL) {
			this->col = 1;
			++this->line;
			tkn->columnEnd = tkn->columnStart;
		} else {
			this->col += this->cur - this->tok;
			tkn->columnEnd = this->col - 1;
		}

		if (type == ABT_TAB) {
			tkn->columnStart += tabWidth;
			tkn->columnEnd += tabWidth;
		}

		tkn->source = this->getSource();
		tkn->tokenList = this->tokens;

		this->tokens->push_back(tkn);
		tkn->iterator = --this->tokens->cend();
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

		lim += this->read(lim, free);

		if (lim < buf + SIZE) {
			eof = true;
			memset(lim, 0, YYMAXFILL);
			lim += YYMAXFILL;
		}

		return true;
	}

	virtual size_t read(unsigned char *dst, size_t count) = 0;

	virtual filesystem::path getSource() const = 0;

	void pushv(TokenType type) {
		this->push(type, string((const char *) this->tok, (size_t) (this->cur - this->tok)));
	}
};

struct InputFile : public Input {
	filesystem::path source;
	FILE *const file;

	InputFile(filesystem::path filename, unsigned int tabWidth)
			: Input(tabWidth)
			, source(filename)
			, file(fopen(filename.str().c_str(), "rb")) {
		if (!file) {
			throw runtime_error("could not open file for reading: " + filename.str() + " (" + strerror(errno) + ")");
		}
	}

	virtual size_t read(unsigned char *dst, size_t count) {
		return fread(dst, 1, count, file);
	}

	virtual filesystem::path getSource() const {
		return this->source;
	}
};

struct InputString : public Input {
	string src;
	size_t srcLen;
	size_t curPos;

	InputString(string srcStr, unsigned int tabWidth)
			: Input(tabWidth)
			, src(srcStr)
			, srcLen(srcStr.length())
			, curPos(0) {
	}

	virtual size_t read(unsigned char *dst, size_t count) {
		size_t i = 0;
		for (; i < count && curPos < srcLen; ++i, ++curPos) {
			dst[i] = src[curPos];
		}
		return i;
	}

	virtual filesystem::path getSource() const {
		return "stdin";
	}
};

void lex_string(Input &in) {
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

void lex_input(Input &in) {
	for (;;) {
		in.tok = in.cur;
/*!re2c
		re2c:define:YYCURSOR = in.cur;
		re2c:define:YYMARKER = in.mar;
		re2c:define:YYLIMIT = in.lim;
		re2c:yyfill:enable = 1;
		re2c:define:YYFILL = "if (!in.fill(@@)) return;";
		re2c:define:YYFILL:naked = 1;

		* { in.push(ABT_NL); in.push(ABT_EOF); return; } // TODO error message

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
		"&" { in.push(ABT_AMP); continue; }

		id = [a-zA-Z_][a-zA-Z_0-9]*;
		id { in.pushv(ABT_ID); continue; }
*/
	}
}

shared_ptr<list<shared_ptr<const Token>>> arua::lexFile(filesystem::path path, unsigned int tabWidth) {
	InputFile in(path, tabWidth);
	lex_input(in);
	return in.tokens;
}

shared_ptr<list<shared_ptr<const Token>>> arua::lexString(string source, unsigned int tabWidth) {
	InputString in(source, tabWidth);
	lex_input(in);
	return in.tokens;
}
