#include "module.h"

using namespace arua;
using namespace std;

Module::Module(const string &name)
		: name(name) {
}

string Module::getName() const throw() {
	return this->name;
}
