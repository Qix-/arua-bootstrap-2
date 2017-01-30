#include "module.h"

using namespace arua;
using namespace std;

Module::Module(shared_ptr<Universe> universe, const string &name)
		: SymbolContext(universe, NodeType::MODULE)
		, Target(TargetType::MODULE)
		, name(name) {
}

string Module::getName() const throw() {
	return this->name;
}
