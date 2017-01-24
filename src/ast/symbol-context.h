#ifndef ARUAB_AST_SYMBOL_CONTEXT_H__
#define ARUAB_AST_SYMBOL_CONTEXT_H__
#pragma once

#include <memory>

#include "symbol-context-base.h"

namespace arua {

class SymbolContext : public SymbolContextBase {
public:
	SymbolContext(std::shared_ptr<SymbolContextBase> parent);
	virtual ~SymbolContext() = default;

protected:
	virtual std::shared_ptr<SymbolEntryBase> resolveNextSymbolEntry(std::shared_ptr<Symbol> name) const throw();

private:
	std::shared_ptr<SymbolContextBase> parent;
};

}

#endif
