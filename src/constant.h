#ifndef ARUAB_CONSTANT_H__
#define ARUAB_CONSTANT_H__
#pragma once

#include "value.h"

namespace arua {

class Constant : public Value {
public:
	Constant(ValueType valueType);
	virtual ~Constant() = default;

	virtual void setMutable(bool mut) throw();
	virtual bool isMutable() const throw();
};

}

#endif
