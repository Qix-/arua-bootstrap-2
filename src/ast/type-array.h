#ifndef ARUAB_AST_TYPE_ARRAY_H__
#define ARUAB_AST_TYPE_ARRAY_H__
#pragma once

#include <memory>

#include "type.h"
#include "symbol-ref.h"

namespace arua {

class TypeArray : public Type {
public:
	TypeArray(unsigned int line, unsigned int col, std::shared_ptr<SymbolRef> innerType, unsigned int depth);

	const std::shared_ptr<SymbolRef> getInnerType() const throw();
	unsigned int getDepth() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<SymbolRef> innerType;
	unsigned int depth;
};

}

#endif
