#include "parse-problem.h"

using namespace arua;
using namespace std;

ParseProblem::ParseProblem(unsigned int line, unsigned int column, string message)
		: runtime_error(message)
		, line(line)
		, column(column)
		, message(message) {
}

string ParseProblem::getMessage() const throw() {
	return this->message;
}

unsigned int ParseProblem::getLine() const throw() {
	return this->line;
}

unsigned int ParseProblem::getColumn() const throw() {
	return this->column;
}
