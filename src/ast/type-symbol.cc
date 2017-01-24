#include "type-symbol.h"

using namespace arua;
using namespace std;

TypeSymbol::TypeSymbol(shared_ptr<Symbol> symbol)
		: Type(PrimitiveType::SYMBOL)
		, symbol(symbol) {
}


const shared_ptr<Symbol> TypeSymbol::getSymbol() const throw() {
	return this->symbol;
}

unsigned int TypeSymbol::getLine() const throw() {
	return this->symbol->getLine();
}

unsigned int TypeSymbol::getColumnStart() const throw() {
	return this->symbol->getColumnStart();
}

unsigned int TypeSymbol::getColumnEnd() const throw() {
	return this->symbol->getColumnEnd();
}

bool TypeSymbol::equals(const Type &type) const throw() {
	auto resolved = this->symbol->resolve();
	return resolved->is(SymbolType::TYPE)
		&& resolved->asType()->equals(type);
}

bool TypeSymbol::canAssignTo(const Type &other) const throw() {
	auto resolved = this->symbol->resolve();
	return resolved->is(SymbolType::TYPE)
		&& resolved->asType()->canAssignTo(type);
}
