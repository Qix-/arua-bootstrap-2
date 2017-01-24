#include "type.h"

using namespace arua;

Type::Type(PrimitiveType primType)
		: Target(TargetType::TYPE)
		, primType(primType) {
}

PrimitiveType Type::getPrimitiveType() const throw() {
	return this->primType;
}

bool Type::operator ==(const Type &other) const {
	return this->equals(other);
}
