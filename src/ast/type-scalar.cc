#include "type-scalar.h"

using namespace arua;
using namespace std;

TypeScalar::TypeScalar(shared_ptr<Universe> universe, ScalarClass scalarClass, unsigned int width)
		: Type(universe, PrimitiveType::SCALAR)
		, scalarClass(scalarClass)
		, width(width) {
}

ScalarClass TypeScalar::getScalarClass() const throw() {
	return this->scalarClass;
}

unsigned int TypeScalar::getWidth() const throw() {
	return this->width;
}

bool TypeScalar::equals(const Type &other) const throw() {
	return other.getPrimitiveType() == PrimitiveType::SCALAR
		&& ((TypeScalar *)&other)->width == this->width;
}


bool TypeScalar::canAssignTo(const Type &other) const throw() {
	return other.getPrimitiveType() == PrimitiveType::SCALAR
		&& ((TypeScalar *)&other)->scalarClass == this->scalarClass
		&& ((TypeScalar *)&other)->width >= this->width;
}
