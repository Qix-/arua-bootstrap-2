#ifndef ARUBA_AST_TYPE_FUNCTION_H__
#define ARUBA_AST_TYPE_FUNCTION_H__
#pragma once

#include <vector>
#include <memory>

#include "token-simple.h"
#include "type.h"

namespace arua {

class TypeFunction : public Type {
public:
	TypeFunction(unsigned int line, unsigned int colStart, unsigned int colEnd);

	// a shared_ptr with a ptr of 0 means void
	void setReturnType(std::shared_ptr<Type> returnType);
	void addArg(std::shared_ptr<Type> argType);

	const std::shared_ptr<Type> getReturnType() const throw();
	const std::vector<std::shared_ptr<Type>> & getArgTypes() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<Type> returnType;
	std::vector<std::shared_ptr<Type>> argTypes;
};

}

#endif
