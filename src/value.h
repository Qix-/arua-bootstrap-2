#ifndef ARUAB_VALUE_H__
#define ARUAB_VALUE_H__
#pragma once

#include <string>

#include "value-type.h"

namespace arua {

class Value {
public:
	Value(ValueType type);
	virtual ~Value() = default;

	ValueType getValueType() const throw();

	void setName(std::string name) throw();
	std::string getName() const throw();

private:
	ValueType type;
	std::string name;
};

}

#endif
