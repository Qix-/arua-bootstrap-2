#ifndef ARUAB_TYPE_ARRAY_H__
#define ARUAB_TYPE_ARRAY_H__
#pragma once

#include "ptr.h"
#include "type.h"

namespace arua {

class TypeArray : public Type {
public:
	TypeArray();
	virtual ~TypeArray() = default;

	void setBaseType(Ptr<Type> baseType) throw();
	Ptr<Type> getBaseType() throw();

	virtual bool operator ==(const Type &other) const throw();
	virtual bool canBeAssignedTo(const Type &other) const throw();

private:
	Ptr<Type> baseType;
};

}

#endif
