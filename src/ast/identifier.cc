#include "identifier.h"

using namespace arua;
using namespace std;

Identifier::Identifier(unsigned int line, unsigned int colStart, string identifier)
		: TokenSimple(line, colStart, colStart + identifier.length())
		, identifier(identifier) {
}

const string & Identifier::getIdentifier() const throw() {
	return this->identifier;
}
