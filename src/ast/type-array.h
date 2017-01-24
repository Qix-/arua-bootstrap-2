#ifndef ARUAB_AST_TYPE_ARRAY_H__
#define ARUAB_AST_TYPE_ARRAY_H__
#pragma once

#include <memory>

#include "type.h"

namespace arua {

class TypeArray : public Type {
public:
	TypeArray(unsigned int line, unsigned int col, std::shared_ptr<Type> innerType);

	std::shared_ptr<Type> getInnerType() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<Type> innerType;
};

}

#endif
