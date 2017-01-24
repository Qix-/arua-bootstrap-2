#include "type-def.h"

using namespace arua;
using namespace std;

TypeDef::TypeDef(unsigned int line, unsigned int col, shared_ptr<SymbolRef> baseType)
		: Type(line, col, PrimitiveType::DEF)
		, baseType(baseType) {
}

shared_ptr<SymbolRef> TypeDef::getBaseType() const throw() {
	return this->baseType;
}


bool TypeDef::equals(const Type &other) const throw() {
	return this->baseType->resolveType()->equals(other);
}

bool TypeDef::canAssignTo(const Type &other) const throw() {
	return this->baseType->resolveType()->canAssignTo(other);
}
