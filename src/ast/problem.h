#ifndef ARUAB_AST_PROBLEM_H__
#define ARUAB_AST_PROBLEM_H__
#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "problem-type.h"
#include "universe.h"

namespace arua {

struct ProblemReference {
	Universe::ID id;
	std::string message;
};

class Problem : public std::runtime_error {
public:
	Problem(ProblemType type, std::string message);

	ProblemType getType() const throw();
	std::string getMessage() const throw();

	Problem * addReference(Universe::ID id, std::string message) throw();

private:
	ProblemType type;
	std::string message;
	std::vector<std::shared_ptr<ProblemReference>> references;
};

}

#endif
