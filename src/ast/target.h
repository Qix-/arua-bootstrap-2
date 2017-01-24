#ifndef ARUAB_AST_TARGET_H__
#define ARUAB_AST_TARGET_H__
#pragma once

#include "symbol-variant.h"

namespace arua {

enum class TargetType {
	SYMBOL,
	TYPE,
};

struct Target : public SymbolVariant {
	Target(TargetType type);
	virtual ~Target() = default;

	TargetType getTargetType() const throw();

private:
	TargetType targetType;
};

}

#endif
