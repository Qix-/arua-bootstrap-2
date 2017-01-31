#include "type-reference.h"

using namespace arua;
using namespace std;

TypeReference::TypeReference()
		: Type(TypeType::REFERENCE) {
}

void TypeReference::setBaseType(Ptr<Type> baseType) throw() {
	this->baseType = baseType;
}

Ptr<Type> TypeReference::getBaseType() throw() {
	return this->baseType;
}

bool TypeReference::operator ==(const Type &other) const throw() {
	return (other.getTypeType() == TypeType::REFERENCE)
		&& (this->baseType == ((TypeReference *)&other)->baseType);
}


bool TypeReference::canBeAssignedTo(const Type &other) const throw() {
	return *this == other;
}
