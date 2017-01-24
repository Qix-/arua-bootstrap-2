#ifndef ARUAB_AST_TOKEN_H__
#define ARUAB_AST_TOKEN_H__
#pragma once

namespace arua {

class Token {
public:
	Token(unsigned int line, unsigned int column);

	unsigned int getLine() const throw();
	unsigned int getColumn() const throw();

private:
	unsigned int line;
	unsigned int col;
};

}

#endif
