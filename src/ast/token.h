#ifndef ARUAB_AST_TOKEN_H__
#define ARUAB_AST_TOKEN_H__
#pragma once

namespace arua {

class Token {
public:
	virtual unsigned int getLine() const throw() = 0;
	virtual unsigned int getColumnStart() const throw() = 0;
	virtual unsigned int getColumnEnd() const throw() = 0;
};

}

#endif
