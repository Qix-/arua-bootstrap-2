#include "node.h"

using namespace arua;
using namespace std;

shared_ptr<Universe> Node::getUniverse() const throw() {
	return this->universe;
}

NodeType Node::getNodeType() const throw() {
	return this->type;
}

Universe::ID Node::getID() const throw() {
	return this->id;
}

Node::Node(shared_ptr<Universe> universe, NodeType type)
		: universe(universe)
		, type(type) {
	shared_ptr<Node> thisShared(this);
	this->thisPtr = thisShared;
	this->id = universe->addNode(thisShared);
}
