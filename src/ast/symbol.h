#ifndef ARUAB_AST_SYMBOL_H__
#define ARUAB_AST_SYMBOL_H__
#pragma once

#include <string>
#include <memory>

#include "symbol-context.h"
#include "token.h"

namespace arua {

class SymbolContext;

class Symbol {
public:
	Symbol(std::string identifier, std::shared_ptr<SymbolContext> symCtx);

	std::string getIdentifier() const throw();
	std::shared_ptr<SymbolContext> getSymbolContext() const throw();

	// Note that certain declarations, namely in structs and modules, do not have to be in order (similar to Java)
	// which means this might return INVALID even for valid symbols that haven't been lexed yet.
	//
	// Make sure to only call this after lexing is finished.
	SymbolType getSymbolType() const throw();

	const std::shared_ptr<Type> asType() const throw();
	const std::shared_ptr<SymbolContext> asContext() const throw();

private:
	std::string identifier;
	std::shared_ptr<SymbolContext> symCtx;
};

}

#endif
