#ifndef ARUAB_AST_SYMBOL_REF_H__
#define ARUAB_AST_SYMBOL_REF_H__
#pragma once

#include <memory>

#include "symbol-context-base.h"
#include "token.h"
#include "type.h"

namespace arua {

class SymbolRef : public Token {
public:
	SymbolRef(unsigned int line, unsigned int col, std::shared_ptr<SymbolContextBase> symCtx, std::shared_ptr<Symbol> symbol);

	const std::shared_ptr<SymbolContextBase> getSymbolContext() const throw();
	const std::shared_ptr<Symbol> getSymbol() const throw();

	// Note that certain declarations, namely in structs and modules, do not have to be in order (similar to Java)
	// which means this might return INVALID even for valid symbols that haven't been lexed yet.
	//
	// Make sure to only call this after lexing is finished.
	SymbolType getSymbolType() const throw();

	const std::shared_ptr<Type> resolveType() const throw();

private:
	std::shared_ptr<SymbolContextBase> symCtx;
	std::shared_ptr<Symbol> symbol;
};

}

#endif
