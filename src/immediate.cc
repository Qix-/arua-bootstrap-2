#include "immediate.h"

using namespace arua;
using namespace std;

Immediate::Immediate(ImmediateType immType)
		: Constant(ValueType::IMMEDIATE)
		, immType(immType) {
}

ImmediateType Immediate::getImmediateType() const throw() {
	return this->immType;
}
