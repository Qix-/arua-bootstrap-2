#ifndef ARUAB_AST_TYPE_DEF_H__
#define ARUAB_AST_TYPE_DEF_H__
#pragma once

#include <memory>

#include "type.h"
#include "symbol-ref.h"

namespace arua {

class TypeDef : public Type {
public:
	TypeDef(unsigned int line, unsigned int col, std::shared_ptr<SymbolRef> baseType);

	std::shared_ptr<SymbolRef> getBaseType() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<SymbolRef> baseType;
};

}

#endif
