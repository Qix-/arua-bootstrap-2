#include "type.h"

using namespace arua;

Type::Type(TypeType typeType)
		: Value(ValueType::TYPE)
		, typeType(typeType) {
}

TypeType Type::getTypeType() const throw() {
	return this->typeType;
}
