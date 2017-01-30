#include "identifier.h"

using namespace arua;
using namespace std;

Identifier::Identifier(shared_ptr<Universe> universe, string identifier)
		: Node(universe)
		, identifier(identifier) {
}

const string & Identifier::getIdentifier() const throw() {
	return this->identifier;
}
