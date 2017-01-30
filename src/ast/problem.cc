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

Problem * Problem::addReference(Universe::ID id, string message) throw() {
	shared_ptr<ProblemReference> ref(new ProblemReference());
	ref->id = id;
	ref->message = message;
	this->references.push_back(ref);
	return this;
}
