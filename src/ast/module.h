#ifndef ARUAB_AST_MODULE_H__
#define ARUAB_AST_MODULE_H__
#pragma once

#include <string>

#include "symbol-context.h"
#include "target.h"

namespace arua {

class Module : public SymbolContext, public Target {
public:
	Module(const std::string &name);

	std::string getName() const throw();

private:
	const std::string name;
};

}

#endif
