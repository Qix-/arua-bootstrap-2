#ifndef ARUAB_PARSE_PROBLEM_H__
#define ARUAB_PARSE_PROBLEM_H__
#pragma once

#include <stdexcept>
#include <string>

namespace arua {

class ParseProblem : public std::runtime_error {
public:
	ParseProblem(unsigned int line, unsigned int column, std::string message);
	virtual ~ParseProblem() = default;

	std::string getMessage() const throw();
	unsigned int getLine() const throw();
	unsigned int getColumn() const throw();

private:
	unsigned int line;
	unsigned int column;
	std::string message;
};

}

#endif
