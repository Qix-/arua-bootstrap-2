#include "value.h"

using namespace arua;
using namespace std;

Value::Value(ValueType type)
		: type(type) {
}

ValueType Value::getValueType() const throw() {
	return this->type;
}

void Value::setName(string name) throw() {
	this->name = name;
}

string Value::getName() const throw() {
	return this->name;
}
