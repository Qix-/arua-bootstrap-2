#include "type-function.h"

using namespace arua;
using namespace std;

TypeFunction::TypeFunction(shared_ptr<Universe> universe)
		: Type(universe, PrimitiveType::FUNCTION) {
}

const shared_ptr<Type> TypeFunction::getReturnType() const throw() {
	return this->returnType;
}

const vector<shared_ptr<Type>> & TypeFunction::getArgTypes() const throw() {
	return this->argTypes;
}

bool TypeFunction::equals(const Type &other) const throw() {
	if (other.getPrimitiveType() != PrimitiveType::FUNCTION) {
		return false;
	}


	// check that return type is either both void (nullptr) or equal to each other
	if (!((this->returnType.get() == nullptr && ((TypeFunction *)&other)->returnType.get() == nullptr)
			|| ((TypeFunction *)&other)->returnType->equals(*this->returnType))) {
		return false;
	}

	// check that argument counts match
	if (this->argTypes.size() != ((TypeFunction *)&other)->argTypes.size()) {
		return false;
	}

	auto thisArgItr = this->argTypes.cbegin();
	auto otherArgItr = ((TypeFunction *)&other)->argTypes.cbegin();
	auto thisArgEnd = this->argTypes.cend();
	auto otherArgEnd = ((TypeFunction *)&other)->argTypes.cend();

	for (; thisArgItr != thisArgEnd && otherArgItr != otherArgEnd; thisArgItr++, otherArgItr++) {
		if (!(*thisArgItr)->equals(**otherArgItr)) {
			return false;
		}
	}

	return true;
}

bool TypeFunction::canAssignTo(const Type &other) const throw() {
	return this->equals(other);
}
