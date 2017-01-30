#include "type-array.h"

using namespace arua;
using namespace std;

TypeArray::TypeArray(std::shared_ptr<Type> innerType, unsigned int depth)
		: Type(PrimitiveType::ARRAY)
		, innerType(innerType)
		, depth(depth) {
}

const shared_ptr<Type> TypeArray::getInnerType() const throw() {
	return this->innerType;
}

bool TypeArray::equals(const Type &other) const throw() {
	// as always, make sure the internal symbolrefs are indeed types.
	// this should be done in a check prior to this method being called, but if you're
	// getting weird segfaults this is probably why.

	return other.getPrimitiveType() == PrimitiveType::ARRAY
		&& ((TypeArray *)&other)->depth == this->depth
		&& ((TypeArray *)&other)->innerType->equals(*this->innerType);
}

bool TypeArray::canAssignTo(const Type &other) const throw() {
	return this->equals(other);
}
