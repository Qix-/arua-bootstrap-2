#ifndef ARUAB_AST_TYPE_SCALAR_H__
#define ARUAB_AST_TYPE_SCALAR_H__
#pragma once

#include "token-simple.h"
#include "type.h"

namespace arua {

enum class ScalarClass {
	INT,
	UINT,
	FLOAT,
};

class TypeScalar : public Type, public virtual TokenSimple {
public:
	TypeScalar(unsigned int line, unsigned int colStart, unsigned int colEnd, ScalarClass scalarClass, unsigned int width);

	using TokenSimple::getLine;
	using TokenSimple::getColumnStart;
	using TokenSimple::getColumnEnd;

	ScalarClass getScalarClass() const throw();
	unsigned int getWidth() const throw();


	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	ScalarClass scalarClass;
	unsigned int width;
};

}

#endif
