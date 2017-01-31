#ifndef ARUAB_CONTEXT_H__
#define ARUAB_CONTEXT_H__
#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "ptr.h"
#include "type.h"
#include "value.h"

namespace arua {

class Context : public Value {
public:
	Context(std::shared_ptr<Context> parent = nullptr);
	virtual ~Context() = default;

	Ptr<Value> resolve(std::string name, std::shared_ptr<Context> requestingContext = nullptr) throw();

	void addType(Ptr<Type> type, bool pub = false) throw();

private:
	std::shared_ptr<Context> parent;
	std::unordered_map<std::string, Ptr<Type>> types;
	std::unordered_set<std::string> publicNames;
};

}

#endif
