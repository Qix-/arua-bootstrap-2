#ifndef ARUAB_AST_IDENTIFIER_H__
#define ARUAB_AST_IDENTIFIER_H__
#pragma once

#include <string>

#include "node.h"

namespace arua {

class Identifier : public Node {
public:
	Identifier(std::shared_ptr<Universe> universe, std::string identifier);
	virtual ~Identifier() = default;

	const std::string & getIdentifier() const throw();

private:
	std::string identifier;
};

}

#endif
