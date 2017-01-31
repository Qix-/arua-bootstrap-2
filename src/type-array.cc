#include "type-array.h"

using namespace arua;
using namespace std;

TypeArray::TypeArray()
		: Type(TypeType::ARRAY) {
}

void TypeArray::setBaseType(Ptr<Type> baseType) throw() {
	this->baseType = baseType;
}

Ptr<Type> TypeArray::getBaseType() throw() {
	return this->baseType;
}

bool TypeArray::operator ==(const Type &other) const throw() {
	return (other.getTypeType() == TypeType::ARRAY)
		&& (this->baseType == ((TypeArray *)&other)->baseType);
}


bool TypeArray::canBeAssignedTo(const Type &other) const throw() {
	return *this == other;
}
