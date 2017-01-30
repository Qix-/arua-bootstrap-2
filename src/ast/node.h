#ifndef ARUAB_AST_NODE_H__
#define ARUAB_AST_NODE_H__
#pragma once

#include <memory>

#include "universe.h"

namespace arua {

class Node {
public:
	virtual ~Node() = default;

	std::shared_ptr<Universe> getUniverse() const throw();
	Universe::ID getID() const throw();
	std::shared_ptr<Node> ptr() throw();

protected:
	Node(std::shared_ptr<Universe> universe);

private:
	std::shared_ptr<Universe> universe;
	Universe::ID id;
	std::weak_ptr<Node> thisPtr;
};

}

#endif
