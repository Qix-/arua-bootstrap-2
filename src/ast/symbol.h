#ifndef ARUAB_AST_SYMBOL_H__
#define ARUAB_AST_SYMBOL_H__
#pragma once

#include <memory>

#include "token.h"

namespace arua {

class SymbolContext;
class SymbolDirect;

class Symbol : public Token {
public:
	Symbol(std::shared_ptr<SymbolContext> symCtx);

	std::shared_ptr<SymbolContext> getContext() const throw();

	virtual std::shared_ptr<SymbolDirect> resolve() const throw() = 0;

private:
	std::shared_ptr<SymbolContext> symCtx;
};

}

#endif
