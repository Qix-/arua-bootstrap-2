#ifndef ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#define ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "type.h"

namespace arua {

class SymbolRef;

enum class SymbolType {
	INVALID,
	TYPE,
	ALIAS,
	CTXREF,
};

struct SymbolEntryBase {
	SymbolEntryBase(SymbolType type);

	// TODO include originating declaration statment reference
	const SymbolType type;
};

template <typename TSymbol>
struct SymbolEntry : public SymbolEntryBase {
	typedef TSymbol Type;

	SymbolEntry(SymbolType type, const std::shared_ptr<TSymbol> value)
			: SymbolEntryBase(type)
			, value(value) {
	}

	const std::shared_ptr<TSymbol> value;
};

class SymbolContext {
	friend class Symbol;
public:
	SymbolContext(std::shared_ptr<SymbolContext> parent = nullptr);
	virtual ~SymbolContext() = default;

	// TODO accept an error handler for duplicate symbols
	void addType(std::string name, std::shared_ptr<Type> type) throw();
	// TODO accept an error handler for duplicate symbols
	void addAlias(std::string name, std::shared_ptr<SymbolRef> symRef) throw();
	// TODO accept an error handler for duplicate symbols
	void addRef(std::string name, std::shared_ptr<SymbolContext> symCtx) throw();

protected:
	std::shared_ptr<SymbolEntryBase> resolveSymbolEntry(std::string name) const throw();

private:
	bool assertDoesntExist(std::string name) const throw();

	std::shared_ptr<SymbolContext> parent;

	// we allow all types instead of just typedefs here since aliases just add a mapping to a type.
	// we also use strings here since they have all of the hash/equality functionality we need
	// to perform simple lookups.
	std::unordered_map<std::string, std::shared_ptr<SymbolEntryBase>> symbols;
};

}

#endif
