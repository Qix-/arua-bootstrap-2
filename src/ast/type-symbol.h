#ifndef ARUAB_AST_TYPE_SYMBOL_H__
#define ARUAB_AST_TYPE_SYMBOL_H__
#pragma once

#include <memory>

#include "symbol.h"
#include "type.h"

namespace arua {

class TypeSymbol : public Type {
public:
	TypeSymbol(std::shared_ptr<Symbol> symbol);

	const std::shared_ptr<Symbol> getSymbol() const throw();

	virtual unsigned int getLine() const throw();
	virtual unsigned int getColumnStart() const throw();
	virtual unsigned int getColumnEnd() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<Symbol> symbol;
};

}

#endif
