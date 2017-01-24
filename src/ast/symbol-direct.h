#ifndef ARUAB_AST_SYMBOL_DIRECT_H__
#define ARUAB_AST_SYMBOL_DIRECT_H__
#pragma once

#include <memory>

#include "identifier.h"
#include "symbol.h"

namespace arua {

class SymbolDirect : public Symbol {
public:
	SymbolDirect(std::shared_ptr<SymbolContext> symCtx, std::shared_ptr<Identifier> identifier);

	virtual unsigned int getLine() const throw();
	virtual unsigned int getColumnStart() const throw();
	virtual unsigned int getColumnEnd() const throw();
private:
	std::shared_ptr<Identifier> identifier;
};

}

#endif
