#include "token-simple.h"

using namespace arua;

TokenSimple::TokenSimple(unsigned int line, unsigned int columnStart, unsigned int columnEnd)
		: line(line)
		, columnStart(columnStart)
		, columnEnd(columnEnd) {
}

unsigned int TokenSimple::getLine() const throw() {
	return this->line;
}

unsigned int TokenSimple::getColumnStart() const throw() {
	return this->columnStart;
}

unsigned int TokenSimple::getColumnEnd() const throw() {
	return this->columnEnd;
}
