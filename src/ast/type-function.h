#ifndef ARUBA_AST_TYPE_FUNCTION_H__
#define ARUBA_AST_TYPE_FUNCTION_H__
#pragma once

#include <vector>
#include <memory>

#include "token-simple.h"
#include "type.h"
#include "symbol-ref.h"

namespace arua {

class TypeFunction : public Type, public TokenSimple {
public:
	TypeFunction(unsigned int line, unsigned int colStart, unsigned int colEnd);

	// a shared_ptr with a ptr of 0 means void
	void setReturnType(std::shared_ptr<SymbolRef> returnType);
	void addArg(std::shared_ptr<SymbolRef> argType);

	const std::shared_ptr<SymbolRef> getReturnType() const throw();
	const std::vector<std::shared_ptr<SymbolRef>> & getArgTypes() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<SymbolRef> returnType;
	std::vector<std::shared_ptr<SymbolRef>> argTypes;
};

}

#endif
