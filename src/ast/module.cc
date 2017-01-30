#include "module.h"

using namespace arua;
using namespace std;

Module::Module(const string &name)
		: Target(TargetType::Module)
		, name(name) {
}

string Module::getName() const throw() {
	return this->name;
}
