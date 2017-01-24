#ifndef ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#define ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#pragma once

#include <memory>
#include <unordered_map>

#include "identifier.h"
#include "symbol-type.h"
#include "symbol-variant.h"
#include "target.h"
#include "type.h"

namespace arua {

struct SymbolEntry {
	SymbolEntry(SymbolType type, const std::shared_ptr<SymbolVariant> value);

	const SymbolType type;
	const std::shared_ptr<SymbolVariant> value;
};

class SymbolContext : public SymbolVariant {
	friend class Symbol;
public:
	SymbolContext(std::shared_ptr<SymbolContext> parent = nullptr);
	virtual ~SymbolContext() = default;

	// TODO accept an error handler for duplicate symbols
	void addType(std::shared_ptr<Identifier> name, std::shared_ptr<Type> type) throw();
	// TODO accept an error handler for duplicate symbols
	void addAlias(std::shared_ptr<Identifier> name, std::shared_ptr<Target> target) throw();
	// TODO accept an error handler for duplicate symbols
	void addRef(std::shared_ptr<Identifier> name, std::shared_ptr<SymbolContext> symCtx) throw();

protected:
	std::shared_ptr<SymbolEntry> resolveSymbolEntry(std::shared_ptr<Identifier> name) const throw();

private:
	bool assertDoesntExist(std::shared_ptr<Identifier> name) const throw();

	std::shared_ptr<SymbolContext> parent;

	// we allow all types instead of just typedefs here since aliases just add a mapping to a type.
	// we also use strings here since they have all of the hash/equality functionality we need
	// to perform simple lookups.
	std::unordered_map<std::string, std::shared_ptr<SymbolEntry>> symbols;
};

}

#endif
