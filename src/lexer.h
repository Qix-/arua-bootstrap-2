#ifndef ARUAB_LEXER_H__
#define ARUAB_LEXER_H__
#pragma once

#include <list>
#include <memory>
#include <stdexcept>

#include "filesystem/path.h"

#include "ast/token.h"

namespace arua {

class LexerProblem : public std::runtime_error {
public:
	LexerProblem(unsigned int line, unsigned int column, std::string message);
	virtual ~LexerProblem() = default;

	std::string getMessage() const throw();
	unsigned int getLine() const throw();
	unsigned int getColumn() const throw();

private:
	unsigned int line;
	unsigned int column;
	std::string message;
};

// note that these WILL throw exceptions in the event there is an invalid token.
std::shared_ptr<std::list<std::shared_ptr<Token>>> lexFile(filesystem::path path, unsigned int tabWidth = 8);
std::shared_ptr<std::list<std::shared_ptr<Token>>> lexString(std::string source, unsigned int tabWidth = 8);

}

#endif
