#include "symbol-context.h"

using namespace arua;
using namespace std;

SymbolContext::SymbolContext(shared_ptr<SymbolContextBase> parent)
		: parent(parent) {
}

shared_ptr<SymbolEntryBase> SymbolContext::resolveNextSymbolEntry(shared_ptr<Symbol> name) const throw() {
	return this->parent->resolveSymbolEntry(name);
}
