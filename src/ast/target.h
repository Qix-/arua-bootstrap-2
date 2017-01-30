#ifndef ARUAB_AST_TARGET_H__
#define ARUAB_AST_TARGET_H__
#pragma once

namespace arua {

enum class TargetType {
	MODULE,
	TYPE,
};

struct Target {
	Target(TargetType type);
	virtual ~Target() = default;

	TargetType getTargetType() const throw();

private:
	TargetType targetType;
};

}

#endif
