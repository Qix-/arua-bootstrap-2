#ifndef ARUAB_TYPE_FUNCTION_H__
#define ARUAB_TYPE_FUNCTION_H__
#pragma once

#include <vector>

#include "ptr.h"
#include "type.h"

namespace arua {

class TypeFunction : public Type {
public:
	TypeFunction();
	virtual ~TypeFunction() = default;

	void addArgumentType(Ptr<Type> type) throw();
	std::vector<Ptr<Type>> & getArgumentTypes() throw();

	// nullptr for void
	void setReturnType(Ptr<Type> type) throw();
	Ptr<Type> getReturnType() const throw();
	bool isVoid() const throw();

	virtual bool operator ==(const Type &other) const throw();
	virtual bool canBeAssignedTo(const Type &other) const throw();

private:
	Ptr<Type> returnType;
	std::vector<Ptr<Type>> argTypes;
};

}

#endif
