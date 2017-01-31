#include "type-function.h"

using namespace arua;
using namespace std;

TypeFunction::TypeFunction()
		: Type(TypeType::FUNCTION) {
}

void TypeFunction::addArgumentType(Ptr<Type> type) throw() {
	this->argTypes.push_back(type);
}

vector<Ptr<Type>> & TypeFunction::getArgumentTypes() throw() {
	return this->argTypes;
}

void TypeFunction::setReturnType(Ptr<Type> type) throw() {
	this->returnType = type;
}

Ptr<Type> TypeFunction::getReturnType() const throw() {
	return this->returnType;
}

bool TypeFunction::isVoid() const throw() {
	return !this->returnType;
}

bool TypeFunction::operator ==(const Type &other) const throw() {
	return (other.getTypeType() == TypeType::FUNCTION)
		&& (((TypeFunction *)&other)->returnType == this->returnType)
		&& (((TypeFunction *)&other)->argTypes == this->argTypes);
}

bool TypeFunction::canBeAssignedTo(const Type &other) const throw() {
	return *this == other;
}
