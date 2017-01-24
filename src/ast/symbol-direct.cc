#include "symbol-direct.h"

using namespace arua;
using namespace std;

SymbolDirect::SymbolDirect(shared_ptr<SymbolContext> symCtx, shared_ptr<Identifier> identifier)
		: Symbol(symCtx)
		, identifier(identifier) {
}


unsigned int SymbolDirect::getLine() const throw() {
	return this->identifier->getLine();
}

unsigned int SymbolDirect::getColumnStart() const throw() {
	return this->identifier->getColumnStart();
}

unsigned int SymbolDirect::getColumnEnd() const throw() {
	return this->identifier->getColumnEnd();
}
