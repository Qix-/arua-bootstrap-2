#include <cstdio>
#include <cerrno>
#include <cstring>

#include "parser.h"

using namespace std;

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

	FILE *const file;

	input(string &filename)
			: buf()
			, lim(buf + SIZE)
			, cur(lim)
			, mar(lim)
			, tok(lim)
			, eof(false)
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
};

void parse_input(input &in) {
	(void) in;
/*!re2c
	re2c:define:YYCURSOR = in.cur;
	re2c:define:YYMARKER = in.mar;
	re2c:define:YYLIMIT = in.lim;
	re2c:yyfill:enable = 1;
	re2c:define:YYFILL = "if (!in.fill(@@)) return false;";
	re2c:define:YYFILL:naked = 1;
*/
}

// TODO change void to an AST tree result.
void arua::parse_file(string &filename) {
	input in(filename);
	parse_input(in);
}
