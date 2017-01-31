#ifndef ARUAB_VALUE_H__
#define ARUAB_VALUE_H__
#pragma once

#include "value-type.h"

namespace arua {

class Value {
public:
	Value(ValueType type);
	virtual ~Value() = default;

	ValueType getValueType() const throw();

private:
	ValueType type;
};

}

#endif
