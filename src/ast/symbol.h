#ifndef ARUAB_AST_SYMBOL_H__
#define ARUAB_AST_SYMBOL_H__
#pragma once

#include <string>

#include "token.h"

namespace arua {

class Symbol : public Token {
public:
	Symbol(unsigned int line, unsigned int col, std::string label);

	std::string getLabel() const throw();

private:
	std::string label;
};

}

#endif
