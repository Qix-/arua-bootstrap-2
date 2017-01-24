#include "symbol-ref.h"

using namespace arua;
using namespace std;

SymbolRef::SymbolRef(unsigned int line, unsigned int col, shared_ptr<SymbolContext> symCtx)
		: Token(line, col)
		, symCtx(symCtx) {
}

const shared_ptr<SymbolContext> SymbolRef::getSymbolContext() const throw() {
	return this->symCtx;
}

const vector<shared_ptr<Symbol>> & SymbolRef::getSymbols() const throw() {
	return this->symbols;
}

SymbolType SymbolRef::getSymbolType() const throw() {
	this->symCtx->getSymbolType(this->symbol);
}

const shared_ptr<Type> SymbolRef::resolveType() const throw() {
	return this->symCtx->resolveType(this->symbol);
}
