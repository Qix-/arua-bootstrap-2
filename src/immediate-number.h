#ifndef ARUAB_IMMEDIATE_NUMBER_H__
#define ARUAB_IMMEDIATE_NUMBER_H__
#pragma once

#include <string>

#include "immediate.h"

namespace arua {

class ImmediateNumber : public Immediate {
public:
	ImmediateNumber(std::string number, unsigned int radix = 10);
	virtual ~ImmediateNumber() = default;

	bool parseNumber(std::string number, unsigned int radix = 10) throw();

	std::string getNumber() const throw();

private:
	std::string number;
};

}

#endif
