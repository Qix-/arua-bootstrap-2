#include "module.h"

using namespace arua;
using namespace std;

Module::Module(string name)
		: name(name) {
}


string Module::getName() const throw() {
	return this->name;
}
