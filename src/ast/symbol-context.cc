#include "symbol-context.h"
#include "problem.h"

using namespace arua;
using namespace std;

SymbolContext::SymbolContext(shared_ptr<Universe> universe, NodeType nodeType, shared_ptr<SymbolContext> parent)
		: Node(universe, nodeType)
		, parent(parent) {
}

shared_ptr<SymbolContext> SymbolContext::getParent() throw() {
	return this->parent;
}

void SymbolContext::addSymbol(Universe::ID id, shared_ptr<Identifier> identifier, bool pub) throw() {
	auto itr = this->symbols.find(identifier->getIdentifier());
	if (itr != this->symbols.end()) {
		this->getUniverse()
			->addError(id, "declaration has the same name as another symbol in module: " + identifier->getIdentifier())
			->addReference(itr->second, "already declared here");
		return;
	}

	this->symbols[identifier->getIdentifier()] = id;
	if (pub) {
		this->publicSymbols.insert(identifier->getIdentifier());
	}
}
