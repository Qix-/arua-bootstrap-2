#ifndef ARUAB_TYPE_SCALAR_H__
#define ARUAB_TYPE_SCALAR_H__
#pragma once

#include "type.h"

namespace arua {

enum class ScalarType {
	UINTEGER,
	INTEGER,
	FLOAT,
};

class TypeScalar : public Type {
public:
	TypeScalar();
	virtual ~TypeScalar() = default;

	void setScalarType(ScalarType scalarType) throw();
	ScalarType getScalarType() const throw();

	void setWidth(unsigned int width) throw();
	unsigned int getWidth() const throw();

	virtual bool operator ==(const Type &other) const throw();
	virtual bool canBeAssignedTo(const Type &other) const throw();

private:
	ScalarType type;
	unsigned int width;
};

}

#endif
