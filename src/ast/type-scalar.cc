#include "type-scalar.h"

using namespace arua;

TypeScalar::TypeScalar(unsigned int line, unsigned int colStart, unsigned int colEnd, ScalarClass scalarClass, unsigned int width)
		: TokenSimple(line, colStart, colEnd)
		, Type(PrimitiveType::SCALAR)
		, scalarClass(scalarClass)
		, width(width) {
}

ScalarClass TypeScalar::getScalarClass() const throw() {
	return this->scalarClass;
}

unsigned int TypeScalar::getWidth() const throw() {
	return this->width;
}

bool TypeScalar::equals(const Type &other) const throw() {
	return other.getPrimitiveType() == PrimitiveType::SCALAR
		&& ((TypeScalar *)&other)->width == this->width;
}


bool TypeScalar::canAssignTo(const Type &other) const throw() {
	return other.getPrimitiveType() == PrimitiveType::SCALAR
		&& ((TypeScalar *)&other)->scalarClass == this->scalarClass
		&& ((TypeScalar *)&other)->width >= this->width;
}
