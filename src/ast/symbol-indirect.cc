#include "symbol-indirect.h"

using namespace arua;
using namespace std;

SymbolIndirect::SymbolIndirect(shared_ptr<SymbolContext> symCtx)
		: Symbol(symCtx)
		, Target(TargetType::SYMBOL) {
}

unsigned int SymbolIndirect::getLine() const throw() {
	if (this->identifiers.size() == 0) {
		return 0;
	}

	return (*this->identifiers.cbegin())->getLine();
}

unsigned int SymbolIndirect::getColumnStart() const throw() {
	if (this->identifiers.size() == 0) {
		return 0;
	}

	return (*this->identifiers.cbegin())->getColumnStart();
}

unsigned int SymbolIndirect::getColumnEnd() const throw() {
	if (this->identifiers.size() == 0) {
		return 0;
	}

	return (*(this->identifiers.cend() - 1))->getColumnEnd();
}

void SymbolIndirect::addIdentifier(const shared_ptr<Identifier> identifier) throw() {
	this->identifiers.push_back(identifier);
}

const vector<const shared_ptr<Identifier>> SymbolIndirect::getIdentifiers() const throw() {
	return this->identifiers;
}
