#ifndef ARUAB_AST_MODULE_H__
#define ARUAB_AST_MODULE_H__
#pragma once

#include <string>

#include "symbol-context-base.h" // XXX when we start compiling against other modules, this will change to symbol-context.

namespace arua {

class Module : public SymbolContextBase { // TODO change to SymbolContext when we start compiling against other modules
public:
	Module(const std::string &name);

	std::string getName() const throw();

private:
	const std::string name;
};

}

#endif
