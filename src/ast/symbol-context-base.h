#ifndef ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#define ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "symbol.h"

namespace arua {

class TypeDef;

enum class SymbolType {
	INVALID,
	TYPEDEF,
};

template <typename TSymbol>
struct SymbolEntry {
	typedef TSymbol Type;

	// TODO include originating declaration statment reference
	const std::shared_ptr<TSymbol> symbol;
};

class SymbolContextBase {
public:
	// returns SymbolType::INVALID if the given symbol isn't found.
	SymbolType getSymbolType(const Symbol &name) const throw();

private:
	std::unordered_map<std::shared_ptr<Symbol>, SymbolEntry<TypeDef>> typeDefSymbols;
};

}

#endif
