#include "problem.h"
#include "universe.h"

using namespace arua;
using namespace std;

Universe::Universe()
		: count(1LL) {
}

unsigned long long Universe::getCount() const throw() {
	return this->count;
}

Universe::ID Universe::addNode() throw() {
	return this->count++;
}

Universe::ID Universe::addNode(shared_ptr<Node> node) throw() {
	ID id = this->count++;
	this->nodes[id] = node;
	return id;
}

void Universe::addToken(Universe::ID id, const shared_ptr<const Token> token) throw() {
	this->tokens[id] = token;
}

void Universe::forward(Universe::ID from, Universe::ID to) throw() {
	this->forwardings[from] = to;
}

shared_ptr<const Token> Universe::getToken(Universe::ID id) const throw() {
	auto tkn = this->tokens.find(id);
	if (tkn == this->tokens.cend()) {
		return nullptr;
	}
	return tkn->second;
}

shared_ptr<Node> Universe::getNode(ID id) throw() {
	id = this->resolve(id);
	auto node = this->nodes.find(id);
	if (node == this->nodes.end()) {
		return nullptr;
	}
	return node->second;
}

Universe::ID Universe::resolve(ID id) const throw() {
	unordered_map<Universe::ID, Universe::ID>::const_iterator citr;
	while ((citr = this->forwardings.find(id)) != this->forwardings.cend()) {
		id = citr->second;
	}

	return id;
}

Problem * Universe::addProblem(Universe::ID id, ProblemType type, string message) throw() {
	shared_ptr<Problem> problem(new Problem(type, message));
	this->problems[id].push_back(problem);
	return problem.get();
}

Problem * Universe::addError(Universe::ID id, string message) throw() {
	return this->addProblem(id, ProblemType::ERROR, message);
}

Problem * Universe::addWarning(Universe::ID id, string message) throw() {
	return this->addProblem(id, ProblemType::WARNING, message);
}

Problem * Universe::addNotice(Universe::ID id, string message) throw() {
	return this->addProblem(id, ProblemType::NOTICE, message);
}
