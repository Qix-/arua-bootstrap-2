#include <iostream>

#include "constant.h"

using namespace arua;
using namespace std;

Constant::Constant(ValueType type)
		: Value(type) {
}

void Constant::setMutable(bool mut) throw() {
	(void) mut;
	cerr << "aruab: warning: attempting to set a constant as mutable. Is the AST feeling okay?" << endl;
}

bool Constant::isMutable() const throw() {
	return false;
}
