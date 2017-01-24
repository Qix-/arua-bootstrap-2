#ifndef ARUAB_AST_SYMBOL_H__
#define ARUAB_AST_SYMBOL_H__
#pragma once

#include <memory>

#include "token.h"

namespace arua {

class SymbolContext;

class Symbol : public Token {
public:
	Symbol(std::shared_ptr<SymbolContext> symCtx);

	std::shared_ptr<SymbolContext> getContext() const throw();

protected:
	virtual std::shared_ptr<Symbol> resolve() const throw() = 0;

private:
	std::shared_ptr<SymbolContext> symCtx;
};

}

#endif
