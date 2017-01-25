#ifndef ARUAB_AST_SYMBOL_CONTEXT_GLOBAL_ZONE_H__
#define ARUAB_AST_SYMBOL_CONTEXT_GLOBAL_ZONE_H__
#pragma once

#include <memory>
#include <set>

#include "filesystem/path.h"

#include "symbol-context-global.h"
#include "symbol-context-global-directory.h"

namespace arua {

class SymbolContextGlobalZone : public SymbolContextGlobal {
public:
	explicit SymbolContextGlobalZone(const std::set<filesystem::path> &zones);
	virtual ~SymbolContextGlobalZone() = default;

protected:
	virtual std::shared_ptr<SymbolEntry> resolveSymbolEntry(std::string name) throw();

private:
	std::set<std::shared_ptr<SymbolContextGlobalDirectory>> zones;
};

}

#endif
