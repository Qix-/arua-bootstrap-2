#include "token.h"

using namespace arua;

Token::Token(unsigned int line, unsigned int column)
		: line(line)
		, col(column) {
}

unsigned int Token::getLine() const throw() {
	return this->line;
}

unsigned int Token::getColumn() const throw() {
	return this->col;
}
