#include "value.h"

using namespace arua;
using namespace std;

Value::Value(ValueType valueType)
		: valueType(valueType) {
}

ValueType Value::getValueType() const throw() {
	return this->valueType;
}

void Value::setName(string name) throw() {
	this->name = name;
}

string Value::getName() const throw() {
	return this->name;
}

void Value::setType(Ptr<Type> type) throw() {
	this->type = type;
}

Ptr<Type> Value::getType() const throw() {
	return this->type;
}

void Value::setContext(Ptr<Context> ctx) throw() {
	this->context = ctx;
}

Ptr<Context> Value::getContext() throw() {
	return this->context;
}
