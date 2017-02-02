#ifndef ARUAB_IMMEDIATE_STRING_H__
#define ARUAB_IMMEDIATE_STRING_H__
#pragma once

#include <string>

#include "immediate.h"

namespace arua {

class ImmediateString : public Immediate {
public:
	ImmediateString(std::string str);
	virtual ~ImmediateString() = default;

	void setString(std::string str) throw();
	std::string getString() const throw();

private:
	std::string str;
};

}

#endif
