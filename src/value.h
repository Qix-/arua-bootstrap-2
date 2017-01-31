#ifndef ARUAB_VALUE_H__
#define ARUAB_VALUE_H__
#pragma once

#include <string>

#include "ptr.h"
#include "value-type.h"

namespace arua {

class Type;

class Value {
public:
	Value(ValueType valueType);
	virtual ~Value() = default;

	ValueType getValueType() const throw();

	void setName(std::string name) throw();
	std::string getName() const throw();

	void setType(Ptr<Type> type) throw();
	Ptr<Type> getType() const throw();

private:
	ValueType valueType;
	std::string name;
	Ptr<Type> type;
};

}

#endif
