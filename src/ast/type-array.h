#ifndef ARUAB_AST_TYPE_ARRAY_H__
#define ARUAB_AST_TYPE_ARRAY_H__
#pragma once

#include <memory>

#include "type.h"

namespace arua {

class TypeArray : public Type {
public:
	TypeArray(std::shared_ptr<Type> innerType, unsigned int depth);

	virtual unsigned int getLine() const throw();
	virtual unsigned int getColumnStart() const throw();
	virtual unsigned int getColumnEnd() const throw();

	const std::shared_ptr<Type> getInnerType() const throw();
	unsigned int getDepth() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<Type> innerType;
	unsigned int depth;
};

}

#endif
