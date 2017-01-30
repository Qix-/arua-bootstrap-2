#ifndef ARUAB_AST_PROBLEM_H__
#define ARUAB_AST_PROBLEM_H__
#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace arua {

enum class ProblemType {
	NOTICE,
	WARNING,
	ERROR,
};

class Problem : public std::runtime_error {
public:
	Problem(ProblemType type, std::string message);

	ProblemType getType() const throw();
	std::string getMessage() const throw();

private:
	ProblemType type;
	std::string message;
};

}

#endif
