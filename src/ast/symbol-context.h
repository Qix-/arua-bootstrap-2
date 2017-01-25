#ifndef ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#define ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

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
	friend class SymbolIndirect;
	friend class SymbolDirect;
	friend class SymbolContextGlobal;
public:
	SymbolContext(std::shared_ptr<SymbolContext> parent = nullptr);
	virtual ~SymbolContext() = default;

	// TODO accept an error handler for duplicate symbols
	virtual void addType(std::shared_ptr<Identifier> name, std::shared_ptr<Type> type, bool pub = false) throw();
	// TODO accept an error handler for duplicate symbols
	virtual void addAlias(std::shared_ptr<Identifier> name, std::shared_ptr<Target> target, bool pub = false) throw();
	// TODO accept an error handler for duplicate symbols
	virtual void addRef(std::shared_ptr<Identifier> name, std::shared_ptr<SymbolContext> symCtx, bool pub = false) throw();

protected:
	virtual std::shared_ptr<SymbolEntry> resolveSymbolEntry(std::shared_ptr<Identifier> name, std::shared_ptr<SymbolContext> baseCtx) const throw();

private:
	bool assertDoesntExist(std::shared_ptr<Identifier> name) const throw();
	bool isCtxChild(std::shared_ptr<SymbolContext> child) const throw();

	std::shared_ptr<SymbolContext> parent;

	// we allow all types instead of just typedefs here since aliases just add a mapping to a type.
	// we also use strings here since they have all of the hash/equality functionality we need
	// to perform simple lookups.
	std::unordered_map<std::string, std::shared_ptr<SymbolEntry>> symbols;
	std::unordered_set<std::string> publicSymbols;
};

}

#endif
