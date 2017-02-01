#ifndef ARUAB_IMMEDIATE_NUMBER_H__
#define ARUAB_IMMEDIATE_NUMBER_H__
#pragma once

#include <string>

#include "zz.h"

#include "immediate.h"

namespace arua {

class ImmediateNumber : public Immediate {
public:
	ImmediateNumber(std::string number, unsigned int radix = 10);
	virtual ~ImmediateNumber();

	bool parseNumber(std::string number, unsigned int radix = 10) throw();

	std::string getNumber() const throw();

private:
	zz_t number;
};

}

#endif
