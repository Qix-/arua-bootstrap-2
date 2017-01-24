#include "symbol-ref.h"

using namespace arua;
using namespace std;

SymbolRef::SymbolRef(unsigned int line, unsigned int col, shared_ptr<SymbolContextBase> symCtx, shared_ptr<Symbol> symbol)
		: Token(line, col)
		, symCtx(symCtx)
		, symbol(symbol) {
}

const shared_ptr<SymbolContextBase> SymbolRef::getSymbolContext() const throw() {
	return this->symCtx;
}

const shared_ptr<Symbol> SymbolRef::getSymbol() const throw() {
	return this->symbol;
}

SymbolType SymbolRef::getSymbolType() const throw() {
	return this->symCtx->getSymbolType(*this->symbol);
}
