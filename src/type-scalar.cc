#include "type-scalar.h"

using namespace arua;

TypeScalar::TypeScalar()
		: Type(TypeType::SCALAR) {
}

void TypeScalar::setScalarType(ScalarType scalarType) throw() {
	this->type = scalarType;
}

ScalarType TypeScalar::getScalarType() const throw() {
	return this->type;
}

void TypeScalar::setWidth(unsigned int width) throw() {
	this->width = width;
}

unsigned int TypeScalar::getWidth() const throw() {
	return this->width;
}

bool TypeScalar::operator ==(const Type &other) const throw() {
	return (other.getTypeType() == TypeType::SCALAR)
		&& (((TypeScalar *)&other)->type == this->type)
		&& (((TypeScalar *)&other)->width == this->width);
}

bool TypeScalar::canBeAssignedTo(const Type &other) const throw() {
	return (other.getTypeType() == TypeType::SCALAR)
		&& (((TypeScalar *)&other)->type == this->type)
		&& (((TypeScalar *)&other)->width >= this->width);
}
