#ifndef ARUAB_AST_TOKEN_H__
#define ARUAB_AST_TOKEN_H__
#pragma once

#include <list>
#include <memory>
#include <string>

#include "filesystem/path.h"

namespace arua {

enum TokenType {
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

struct Token {
	TokenType type;
	unsigned int line;
	unsigned int columnStart;
	unsigned int columnEnd;
	filesystem::path source;
	std::weak_ptr<std::list<std::shared_ptr<const Token>>> tokenList;
	std::string value;
};

}

#endif
