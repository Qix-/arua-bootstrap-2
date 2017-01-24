#include "symbol.h"

using namespace arua;
using namespace std;

Symbol::Symbol(unsigned int line, unsigned int col, string label, shared_ptr<SymbolContext> symCtx)
		: Token(line, col)
		, label(label)
		, symCtx(symCtx) {
}

string Symbol::getLabel() const throw () {
	return this->label;
}

shared_ptr<SymbolContext> Symbol::getSymbolContext() const throw() {
	return this->symCtx;
}
