#ifndef ARUAB_AST_SYMBOL_INDIRECT_H__
#define ARUAB_AST_SYMBOL_INDIRECT_H__
#pragma once

#include <memory>
#include <vector>

#include "identifier.h"
#include "symbol.h"
#include "target.h"

namespace arua {

class SymbolIndirect : public Symbol, public Target {
public:
	SymbolIndirect(std::shared_ptr<SymbolContext> symCtx);

	virtual unsigned int getLine() const throw();
	virtual unsigned int getColumnStart() const throw();
	virtual unsigned int getColumnEnd() const throw();

	void addIdentifier(const std::shared_ptr<Identifier> identifier) throw();

	const std::vector<const std::shared_ptr<Identifier>> getIdentifiers() const throw();

	virtual std::shared_ptr<SymbolDirect> resolve() const throw();
private:
	std::vector<const std::shared_ptr<Identifier>> identifiers;
};

}

#endif
