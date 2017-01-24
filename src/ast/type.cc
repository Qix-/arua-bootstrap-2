#include "type.h"

using namespace arua;

Type::Type(PrimitiveType primType)
		: primType(primType) {
}

PrimitiveType Type::getPrimitiveType() const throw() {
	return this->primType;
}

bool Type::operator ==(const Type &other) const {
	return this->equals(other);
}
