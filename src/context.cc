#include <iostream>

#include "context.h"

using namespace arua;
using namespace std;

Context::Context(shared_ptr<Context> parent)
		: parent(parent) {
}

Ptr<Value> Context::resolve(string name) throw() {
	auto itrType = this->types.find(name);
	if (itrType != this->types.end()) {
		return itrType->second.as<Value>();
	}

	return parent ? parent->resolve(name) : nullptr;
}

void Context::addType(Ptr<Type> type, bool pub) throw() {
	if (type->getName().empty()) {
		cerr << "warning: Context::addType() was passed a type with no name" << endl;
		return;
	}

	this->types[type->getName()] = type;
	if (pub) {
		this->publicNames.insert(type->getName());
	}
}
