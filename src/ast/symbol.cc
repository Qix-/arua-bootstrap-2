#include "symbol.h"

using namespace arua;
using namespace std;

Symbol::Symbol(shared_ptr<SymbolContext> symCtx)
		: symCtx(symCtx) {
}

shared_ptr<SymbolContext> Symbol::getContext() const throw() {
	return this->symCtx;
}
