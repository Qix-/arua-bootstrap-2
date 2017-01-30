#ifndef ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#define ARUAB_AST_SYMBOL_CONTEXT_BASE_H__
#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "identifier.h"
#include "node.h"
#include "target.h"
#include "type.h"

namespace arua {

class SymbolContext : public Node {
public:
	SymbolContext(std::shared_ptr<Universe> universe, std::shared_ptr<SymbolContext> parent = nullptr);
	virtual ~SymbolContext() = default;

	std::shared_ptr<SymbolContext> getParent() throw();

	void addSymbol(Universe::ID id, std::shared_ptr<Identifier> identifier, bool pub = false) throw();

protected:
	std::unordered_map<std::string, Universe::ID> symbols;
	std::unordered_set<std::string> publicSymbols;

private:
	std::shared_ptr<SymbolContext> parent;
};

}

#endif
