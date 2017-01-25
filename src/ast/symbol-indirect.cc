#include <iostream> // XXX remove this when error handling is added

#include "symbol-context.h"
#include "symbol-direct.h"
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

shared_ptr<SymbolDirect> SymbolIndirect::resolve() const throw() {
	if (this->identifiers.size() == 0) {
		return nullptr;
	}

	if (this->identifiers.size() == 1) {
		return shared_ptr<SymbolDirect>(new SymbolDirect(this->getContext(), this->identifiers[0]));
	}

	auto lastCtx = this->getContext();
	auto curCtx = this->getContext();
	auto idItr = this->identifiers.cbegin();
	auto idLast = this->identifiers.cend() - 1;

	for (; idItr != idLast; ++idItr) {
		auto resolved = curCtx->resolveSymbolEntry(*idItr, lastCtx);
		if (!resolved) {
			return nullptr;
		}

		if (resolved->type != SymbolType::CTXREF) {
			// TODO hand off to error handler
			cerr << "ERROR: identifier '" << (*idItr)->getIdentifier() << "' in indirect symbol does not refer to a symbol context at "
				<< (*idItr)->getLine() << ":" << (*idItr)->getColumnStart() << endl;
			return nullptr;
		}

		lastCtx = curCtx;
		curCtx = static_pointer_cast<SymbolContext>(resolved->value);
	}

	return shared_ptr<SymbolDirect>(new SymbolDirect(curCtx, *idLast));
}
