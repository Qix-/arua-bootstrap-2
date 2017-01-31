#ifndef ARUAB_MODULE_H__
#define ARUAB_MODULE_H__
#pragma once

#include <string>

#include "context.h"

namespace arua {

class Module : public Context {
public:
	Module(std::string name);
	virtual ~Module() = default;

	std::string getName() const throw();

private:
	std::string name;
};

}

#endif
