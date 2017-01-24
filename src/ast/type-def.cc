#include "type-def.h"

using namespace arua;
using namespace std;

TypeDef::TypeDef(unsigned int line, unsigned int col, shared_ptr<Type> baseType)
		: Type(line, col, PrimitiveType::DEF)
		, baseType(baseType) {
}

shared_ptr<Type> TypeDef::getBaseType() const throw() {
	return this->baseType;
}
