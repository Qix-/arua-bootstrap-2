#ifndef ARUAB_AST_UNIVERSE_H__
#define ARUAB_AST_UNIVERSE_H__
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "problem.h"
#include "token.h"

namespace arua {

class Node;

class Universe {
public:
	typedef unsigned long long ID;

	ID getCount() const throw();

	ID addNode(std::shared_ptr<Node> node) throw();
	void addToken(ID node, const std::shared_ptr<const Token> token) throw();
	void forward(ID from, ID to) throw();

	std::shared_ptr<const Token> getToken(ID id) const throw();
	std::shared_ptr<Node> getNode(ID id) throw();

	Problem & addProblem(ID id, ProblemType type, std::string message) throw();
	Problem & addError(ID id, std::string message) throw();
	Problem & addWarning(ID id, std::string message) throw();
	Problem & addNotice(ID id, std::string message) throw();

private:
	ID resolveForwards(ID id) const throw();

	std::unordered_map<ID, std::shared_ptr<Node>> nodes;
	std::unordered_map<ID, ID> forwardings;
	std::unordered_map<ID, std::shared_ptr<const Token>> tokens;
	std::unordered_map<ID, std::vector<std::shared_ptr<Problem>>> problems;

	ID count;
};

}

#endif
