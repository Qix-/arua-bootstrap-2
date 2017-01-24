#include "target.h"

using namespace arua;

Target::Target(TargetType type)
		: targetType(type) {
}

TargetType Target::getTargetType() const throw() {
	return this->targetType;
}
