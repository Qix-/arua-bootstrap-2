#include "problem.h"

using namespace arua;
using namespace std;

Problem::Problem(ProblemType type, string message)
		: runtime_error("There was a fatal, unhandled problem with the code you're compiling.")
		, type(type)
		, message(message) {
}

ProblemType Problem::getType() const throw() {
	return this->type;
}

string Problem::getMessage() const throw() {
	return this->message;
}
