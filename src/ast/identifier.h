#ifndef ARUAB_AST_IDENTIFIER_H__
#define ARUAB_AST_IDENTIFIER_H__
#pragma once

#include <string>

#include "token-simple.h"

namespace arua {

class Identifier : public TokenSimple {
public:
	Identifier(unsigned int line, unsigned int colStart, std::string identifier);
	virtual ~Identifier() = default;

	const std::string & getIdentifier() const throw();

private:
	std::string identifier;
};

}

#endif
