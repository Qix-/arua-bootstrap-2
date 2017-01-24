#ifndef ARUAB_AST_TYPE_DEF_H__
#define ARUAB_AST_TYPE_DEF_H__
#pragma once

#include <memory>

#include "type.h"

namespace arua {

class TypeDef : public Type {
public:
	TypeDef(unsigned int line, unsigned int col, std::shared_ptr<Type> baseType);

	std::shared_ptr<Type> getBaseType() const throw();

private:
	std::shared_ptr<Type> baseType;
};

}

#endif