#include "module.h"

using namespace arua;
using namespace std;

Module::Module(shared_ptr<Universe> universe, const string &name)
		: SymbolContext(universe)
		, Target(TargetType::Module)
		, name(name) {
}

string Module::getName() const throw() {
	return this->name;
}
