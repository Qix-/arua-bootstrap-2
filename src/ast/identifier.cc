#include "identifier.h"

using namespace arua;
using namespace std;

Identifier::Identifier(shared_ptr<Universe> universe, string identifier)
		: Node(universe, NodeType::IDENTIFIER)
		, identifier(identifier) {
}

const string & Identifier::getIdentifier() const throw() {
	return this->identifier;
}
