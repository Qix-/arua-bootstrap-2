#ifndef ARUAB_AST_SYMBOL_CONTEXT_GLOBAL_DIRECTORY_H__
#define ARUAB_AST_SYMBOL_CONTEXT_GLOBAL_DIRECTORY_H__
#pragma once

#include <unordered_set>

#include "filesystem/path.h"

#include "symbol-context-global.h"

namespace arua {

class SymbolContextGlobalDirectory : public SymbolContextGlobal {
public:
	explicit SymbolContextGlobalDirectory(const filesystem::path directory);
	virtual ~SymbolContextGlobalDirectory() = default;

	const filesystem::path getDirectory() const throw();

protected:
	virtual std::shared_ptr<SymbolEntry> resolveSymbolEntry(std::string name) throw();

private:
	std::unordered_set<filesystem::path> nonexistent;
	const filesystem::path directory;
};

}

#endif
