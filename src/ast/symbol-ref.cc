#include <iostream> // XXX remove when error handlers are a thing

#include "symbol-ref.h"

using namespace arua;
using namespace std;

SymbolRef::SymbolRef(unsigned int line, unsigned int col, shared_ptr<SymbolContext> symCtx)
		: Token(line, col)
		, symCtx(symCtx) {
}

void SymbolRef::addIdentifier(string identifier) throw() {
	this->identifiers.push_back(identifier);
}

const shared_ptr<SymbolContext> SymbolRef::getSymbolContext() const throw() {
	return this->symCtx;
}

const vector<std::string> & SymbolRef::getIdentifiers() const throw() {
	return this->identifiers;
}

shared_ptr<Symbol> SymbolRef::resolveSymbol() const throw() {
	if (this->identifiers.size() == 0) {
		return nullptr;
	}

	if (this->identifiers.size() == 1) {
		return shared_ptr<Symbol>(new Symbol(*this->identifiers.begin(), this->symCtx));
	}

	auto curCtx = this->symCtx;
	auto itr = this->identifiers.cbegin();

	// iterate until one before the end of the list
	for (; (itr + 1) != this->identifiers.cend(); itr++) {
		curCtx = Symbol(*itr, curCtx).resolveContext();

		if (!curCtx) {
			// TODO when we're using identifier tokens and not strings, include their line/col info here
			// TODO report to error handler
			cerr << "identifier is not a reference to a symbol context: " << *itr << endl;
			return nullptr;
		}
	}

	// finally, return the last ident and current ctx as a symbol
	return shared_ptr<Symbol>(new Symbol(*itr, curCtx));
}
