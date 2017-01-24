#ifndef ARUAB_AST_TYPE_DEF_H__
#define ARUAB_AST_TYPE_DEF_H__
#pragma once

#include <memory>

#include "type.h"
#include "symbol-ref.h"

namespace arua {

class TypeDef : public Type {
public:
	TypeDef(unsigned int line, unsigned int col, std::shared_ptr<SymbolRef> baseType, bool deferred = false);

	std::shared_ptr<SymbolRef> getBaseType() const throw();
	// if a typedef is deferred, it means that it should be resolved after parsing and replaced.
	// this is generally true of alias types or types that cannot be reliably parsed or resolved
	// at parse-time.
	bool isDeferred() const throw();

	virtual bool equals(const Type &type) const throw();
	virtual bool canAssignTo(const Type &other) const throw();

private:
	std::shared_ptr<SymbolRef> baseType;
	bool deferred;
};

}

#endif
