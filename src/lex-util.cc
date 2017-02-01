#include "lex-util.h"

using namespace std;

string arua::formatToken(const shared_ptr<const Token> tkn, bool human) {
	switch (tkn->type) {
	case ABT_EOF: return (human ? "end of input" : "");
	case ABT_WS: return (human ? "whitespace" : " ");
	case ABT_TAB: return (human ? "tab" : "\t");
	case ABT_NL: return (human ? "new line" : "\n");
	case ABT_FN: return "\x1b[36;1mfn\x1b[m";
	case ABT_ID: return tkn->value;
	case ABT_OPAREN: return "\x1b[2m(\x1b[m";
	case ABT_CPAREN: return "\x1b[2m)\x1b[m";
	case ABT_OBRACKET: return "\x1b[2m[\x1b[m";
	case ABT_CBRACKET: return "\x1b[2m]\x1b[m";
	case ABT_RANGLE: return "\x1b[2m>\x1b[m";
	case ABT_LANGLE: return "\x1b[2m<\x1b[m";
	case ABT_AMP: return "\x1b[2m&\x1b[m";
	case ABT_RET: return "\x1b[36;1mret\x1b[m";
	case ABT_NUM:
	case ABT_NUM_RADIX:
	case ABT_NUM_EX: return "\x1b[31m" + tkn->value + "\x1b[m";
	case ABT_SCALAR: return "\x1b[38;2;199m" + tkn->value + "\x1b[m";
	case ABT_DOT: return "\x1b[2m.\x1b[m";
	case ABT_USE: return "\x1b[36;1muse\x1b[m";
	case ABT_STR_BEGIN: return "\x1b[38;5;208m\"";
	case ABT_STR_END: return "\"\x1b[m";
	case ABT_STR_LITERAL: return tkn->value;
	case ABT_STR_ESCAPE: return "\x1b[38;5;214;1m" + tkn->value + "\x1b[0;38;5;208m";
	case ABT_AS: return "\x1b[36;1mas\x1b[m";
	case ABT_PUB: return "\x1b[36;1mpub\x1b[m";
	case ABT_STRUCT: return "\x1b[36;1mstruct\x1b[m";
	case ABT_ON: return "\x1b[36;1mon\x1b[m";
	case ABT_BE: return "\x1b[36;1mbe\x1b[m";
	case ABT_TRAIT: return "\x1b[36;1mtrait\x1b[m";
	case ABT_TYPEDEF: return "\x1b[36;1mtype\x1b[m";
	case ABT_ALIAS: return "\x1b[36;1malias\x1b[m";
	case ABT_COMMA: return "\x1b[2m,\x1b[m";
	case ABT_PLUS: return "\x1b[2m+\x1b[m";
	case ABT_MINUS: return "\x1b[2m-\x1b[m";
	case ABT_DIVIDE: return "\x1b[2m/\x1b[m";
	case ABT_MULTIPLY: return "\x1b[2m*\x1b[m";
	case ABT_EQUALS: return "\x1b[2m=\x1b[m";
	case ABT_COMMENT:
		if (human) {
			return "comment";
		} else {
			return "\x1b[38;5;242;2m" + tkn->value + "\x1b[m";
		}
		break;
	case ABT_COMMENT_DOC:
		if (human) {
			return "documentation comment";
		} else {
			return "\x1b[38;5;61;2m" + tkn->value + "\x1b[m";
		}
		break;
	case ABT_COMMENT_HEADER:
		if (human) {
			return "header comment";
		} else {
			return "\x1b[38;5;50;2m" + tkn->value + "\x1b[m";
		}
		break;
	}
}

void arua::renderTokens(std::list<std::shared_ptr<const Token>> &tokens, std::ostream &stream) {
	arua::renderTokens(tokens.cbegin(), tokens.cend(), stream);
}

void arua::renderTokens(std::list<std::shared_ptr<const Token>>::const_iterator begin, std::list<std::shared_ptr<const Token>>::const_iterator end, std::ostream &stream) {
	for (; begin != end; begin++) {
		stream << formatToken(*begin);
	}
}
