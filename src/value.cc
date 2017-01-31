#include "value.h"

using namespace arua;

Value::Value(ValueType type)
		: type(type) {
}

ValueType Value::getValueType() const throw() {
	return this->type;
}
