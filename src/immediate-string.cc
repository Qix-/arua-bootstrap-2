#include "immediate-string.h"

using namespace arua;
using namespace std;

ImmediateString::ImmediateString(string str)
		: Immediate(ImmediateType::STRING)
		, str(str) {
}

void ImmediateString::setString(string str) throw() {
	this->str = str;
}

string ImmediateString::getString() const throw() {
	return this->str;
}
