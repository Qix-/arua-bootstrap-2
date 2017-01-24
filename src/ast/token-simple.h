#ifndef ARUAB_AST_TOKEN_SIMPLE_H__
#define ARUAB_AST_TOKEN_SIMPLE_H__
#pragma once

#include "token.h"

namespace arua {

class TokenSimple : public virtual Token {
public:
	TokenSimple(unsigned int line, unsigned int columnStart, unsigned int columnEnd);
	virtual ~TokenSimple() = default;

	virtual unsigned int getLine() const throw();
	virtual unsigned int getColumnStart() const throw();
	virtual unsigned int getColumnEnd() const throw();

private:
	const unsigned int line;
	const unsigned int columnStart;
	const unsigned int columnEnd;
};

}

#endif
