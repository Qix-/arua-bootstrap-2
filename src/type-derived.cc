#include "type-derived.h"

using namespace arua;
using namespace std;

TypeDerived::TypeDerived()
		: Type(TypeType::DERIVED) {
}

void TypeDerived::setBaseType(Ptr<Type> baseType) throw() {
	this->baseType = baseType;
}

Ptr<Type> TypeDerived::getBaseType() throw() {
	return this->baseType;
}

bool TypeDerived::operator ==(const Type &other) const throw() {
	// this is the only way to really determine if they're the same.
	// having similar types doesn't make them the same type (they're derived
	// for a reason).
	//
	// this only works in the bootstrap compiler due to it performing whole-program
	// compilation every time. Otherwise, we'd have to have a bit more sophistication
	// when writing our AST and thus checking which module the type came from, the name,
	// etc.
	return (Type *)this == &other;
}

bool TypeDerived::canBeAssignedTo(const Type &other) const throw() {
	return *this == other;
}
