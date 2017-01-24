#include "type-array.h"

using namespace arua;
using namespace std;

TypeArray::TypeArray(unsigned int line, unsigned int col, std::shared_ptr<Type> innerType)
		: Type(line, col, PrimitiveType::ARRAY)
		, innerType(innerType) {
}

shared_ptr<Type> TypeArray::getInnerType() const throw() {
	return this->innerType;
}

bool TypeArray::equals(const Type &other) const throw() {
	return other.getPrimitiveType() == PrimitiveType::ARRAY
		&& ((TypeArray *)&other)->innerType->equals(*this->innerType);
}

bool TypeArray::canAssignTo(const Type &other) const throw() {
	return this->equals(other);
}
