#include "symbol-context.h"
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

const shared_ptr<Identifier> SymbolDirect::getIdentifier() const throw() {
	return this->identifier;
}

shared_ptr<SymbolDirect> SymbolDirect::resolve() const throw() {
	if (!this->getContext()->resolveSymbolEntry(this->identifier, this->getContext())) {
		return nullptr;
	}

	return shared_ptr<SymbolDirect>(new SymbolDirect(this->getContext(), this->identifier));
}

bool SymbolDirect::is(SymbolType type) const throw() {
	return this->getContext()->resolveSymbolEntry(this->identifier, this->getContext())->type == type;
}

shared_ptr<Type> SymbolDirect::asType() const throw() {
	return static_pointer_cast<Type>(this->getContext()->resolveSymbolEntry(this->identifier, this->getContext())->value);
}
