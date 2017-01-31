#ifndef ARUAB_CONTEXT_H__
#define ARUAB_CONTEXT_H__
#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "ptr.h"
#include "type.h"
#include "value-type.h"

namespace arua {

class Context {
public:
	Context(std::shared_ptr<Context> parent = nullptr);
	virtual ~Context() = default;

	Ptr<Value> resolve(std::string name) throw();

	void addType(Ptr<Type> type) throw();

private:
	std::shared_ptr<Context> parent;
	std::unordered_map<std::string, Ptr<Type>> types;
};

}

#endif
