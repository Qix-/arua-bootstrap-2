#ifndef ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#define ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "symbol.h"
#include "type.h"

namespace arua {

class SymbolRef;

enum class SymbolType {
	INVALID,
	TYPE,
	ALIAS,
};

struct SymbolEntryBase {
	SymbolEntryBase(SymbolType type, std::shared_ptr<Symbol> symbol);

	// TODO include originating declaration statment reference
	const std::shared_ptr<Symbol> symbol;
	const SymbolType type;
};

template <typename TSymbol>
struct SymbolEntry : public SymbolEntryBase {
	typedef TSymbol Type;

	SymbolEntry(SymbolType type, const std::shared_ptr<Symbol> symbol, const std::shared_ptr<TSymbol> value)
			: SymbolEntryBase(type, symbol)
			, value(value) {
	}

	const std::shared_ptr<TSymbol> value;
};

class SymbolContextBase {
	friend class SymbolContext;
public:
	virtual ~SymbolContextBase() = default;

	// returns SymbolType::INVALID if the given symbol isn't found.
	SymbolType getSymbolType(const std::shared_ptr<Symbol> name) const throw();

	// TODO accept an error handler for duplicate symbols
	void addType(std::shared_ptr<Symbol> name, std::shared_ptr<Type> type) throw();
	// TODO accept an error handler for duplicate symbols
	void addAlias(std::shared_ptr<Symbol> name, std::shared_ptr<SymbolRef> symRef) throw();

	// These throw if their respective types are not correct; make sure to call getSymbolType() first!
	std::shared_ptr<Type> resolveType(std::shared_ptr<Symbol> name) const throw(std::runtime_error);

protected:
	std::shared_ptr<SymbolEntryBase> resolveSymbolEntry(std::shared_ptr<Symbol> name) const throw();
	virtual std::shared_ptr<SymbolEntryBase> resolveNextSymbolEntry(std::shared_ptr<Symbol> name) const throw();

private:
	bool assertDoesntExist(const std::shared_ptr<Symbol> name) const throw();

	// we allow all types instead of just typedefs here since aliases just add a mapping to a type.
	// we also use strings here since they have all of the hash/equality functionality we need
	// to perform simple lookups.
	std::unordered_map<std::string, std::shared_ptr<SymbolEntryBase>> symbols;
};

}

#endif
