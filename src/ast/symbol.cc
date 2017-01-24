#include "symbol.h"

using namespace arua;
using namespace std;

Symbol::Symbol(unsigned int line, unsigned int col, string label)
		: Token(line, col)
		, label(label) {
}

string Symbol::getLabel() const throw () {
	return this->label;
}
