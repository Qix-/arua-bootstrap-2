#include "type.h"

using namespace arua;
using namespace std;

Type::Type(shared_ptr<Universe> universe, PrimitiveType primType)
		: Node(universe, NodeType::TYPE)
		, Target(TargetType::TYPE)
		, primType(primType) {
}

PrimitiveType Type::getPrimitiveType() const throw() {
	return this->primType;
}

bool Type::operator ==(const Type &other) const {
	return this->equals(other);
}
