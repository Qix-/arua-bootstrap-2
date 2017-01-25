#ifndef ARUAB_AST_SYMBOL_CONTEXT_GLOBAL_H__
#define ARUAB_AST_SYMBOL_CONTEXT_GLOBAL_H__
#pragma once

#include <string>

#include "symbol-context.h"

namespace arua {

class SymbolContextGlobal : public SymbolContext {
public:
	virtual ~SymbolContextGlobal() = default;

	virtual void addType(std::shared_ptr<Identifier> name, std::shared_ptr<Type> type, bool pub = false) throw();
	virtual void addAlias(std::shared_ptr<Identifier> name, std::shared_ptr<Target> target, bool pub = false) throw();
	virtual void addRef(std::shared_ptr<Identifier> name, std::shared_ptr<SymbolContext> symCtx, bool pub = false) throw();
	virtual void addRef(std::string name, std::shared_ptr<SymbolContext> symCtx) throw();

	virtual std::shared_ptr<SymbolEntry> resolveSymbolEntry(std::shared_ptr<Identifier> name, std::shared_ptr<SymbolContext> baseCtx) throw();

protected:
	virtual std::shared_ptr<SymbolEntry> resolveSymbolEntry(std::string name) throw() = 0;
};

}

#endif
