#ifndef ARUAB_AST_SYMBOL_H__
#define ARUAB_AST_SYMBOL_H__
#pragma once

#include <string>
#include <memory>

#include "token.h"

namespace arua {

class SymbolContext;

class Symbol : public Token {
public:
	Symbol(unsigned int line, unsigned int col, std::string label, std::shared_ptr<SymbolContext> symCtx);

	std::string getLabel() const throw();
	std::shared_ptr<SymbolContext> getSymbolContext() const throw();

private:
	std::string label;
	std::shared_ptr<SymbolContext> symCtx;
};

}

#endif
