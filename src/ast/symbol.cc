#include "symbol.h"

using namespace arua;
using namespace std;

Symbol::Symbol(string identifier, shared_ptr<SymbolContext> symCtx)
		: identifier(identifier)
		, symCtx(symCtx) {
}

string Symbol::getIdentifier() const throw () {
	return this->identifier;
}

shared_ptr<SymbolContext> Symbol::getSymbolContext() const throw() {
	return this->symCtx;
}

SymbolType Symbol::getSymbolType() const throw() {
	return this->symCtx->resolveSymbolEntry(this->identifier)->type;
}

const shared_ptr<Type> Symbol::asType() const throw() {
	auto entry = this->symCtx->resolveSymbolEntry(this->identifier);
	if (entry->type != SymbolType::TYPE) {
		return nullptr;
	}

	return static_pointer_cast<SymbolEntry<Type>>(entry)->value;
}

const shared_ptr<SymbolContext> Symbol::asContext() const throw() {
	auto entry = this->symCtx->resolveSymbolEntry(this->identifier);
	if (entry->type != SymbolType::CTXREF) {
		return nullptr;
	}

	return static_pointer_cast<SymbolEntry<SymbolContext>>(entry)->value;
}
