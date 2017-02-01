#ifndef ARUAB_IMMEDIATE_H__
#define ARUAB_IMMEDIATE_H__
#pragma once

#include "constant.h"

namespace arua {

enum class ImmediateType {
	NUMBER,
	STRING,
};

class Immediate : public Constant {
public:
	Immediate(ImmediateType immType);
	virtual ~Immediate() = default;

	ImmediateType getImmediateType() const throw();

private:
	ImmediateType immType;
};

}

#endif
