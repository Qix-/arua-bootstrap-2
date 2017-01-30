#ifndef ARUAB_AST_NODE_H__
#define ARUAB_AST_NODE_H__
#pragma once

#include <memory>

#include "universe.h"

namespace arua {

enum class NodeType {
	MODULE,
	TYPE,
	IDENTIFIER,
};

class Node {
public:
	virtual ~Node() = default;

	std::shared_ptr<Universe> getUniverse() const throw();
	NodeType getNodeType() const throw();
	Universe::ID getID() const throw();

	template <typename T>
	std::shared_ptr<T> ptr() throw() {
		return std::static_pointer_cast<T>(this->thisPtr.lock());
	}

protected:
	Node(std::shared_ptr<Universe> universe, NodeType type);

private:
	std::shared_ptr<Universe> universe;
	NodeType type;
	Universe::ID id;
	std::weak_ptr<Node> thisPtr;
};

}

#endif
