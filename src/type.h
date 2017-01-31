#ifndef ARUAB_TYPE_H__
#define ARUAB_TYPE_H__
#pragma once

#include "type-type.h"
#include "value.h"

namespace arua {

class Type : public Value {
public:
	Type(TypeType typeType);
	virtual ~Type() = default;

	TypeType getTypeType() const throw();

	virtual bool operator==(const Type &other) const throw() = 0;
	virtual bool canBeAssignedTo(const Type &other) const throw() = 0;

private:
	TypeType typeType;
};

}

#endif
