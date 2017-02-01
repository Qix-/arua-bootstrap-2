#ifndef ARUAB_CONTEXT_H__
#define ARUAB_CONTEXT_H__
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

#include "ptr.h"
#include "type.h"
#include "value.h"

namespace arua {

class Context : public Value {
public:
	Context(Ptr<Context> parent = nullptr);
	virtual ~Context() = default;

	Ptr<Value> resolve(std::string name, Ptr<Value> requester = nullptr) throw();

	void addType(Ptr<Type> type, bool pub = false) throw();

private:
	Ptr<Context> parent;
	std::unordered_map<std::string, Ptr<Type>> types;
	std::unordered_set<std::string> publicNames;
};

}

#endif
