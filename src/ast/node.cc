#include "node.h"

using namespace arua;
using namespace std;

shared_ptr<Universe> Node::getUniverse() const throw() {
	return this->universe;
}

Universe::ID Node::getID() const throw() {
	return this->id;
}

Node::Node(shared_ptr<Universe> universe)
		: universe(universe) {
	shared_ptr<Node> thisShared(this);
	this->thisPtr = thisShared;
	this->id = universe->addNode(thisShared);
}
