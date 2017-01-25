#include <iostream>

#include "../lexer.h"
#include "symbol-context-global-directory.h"

using namespace arua;
using namespace std;

SymbolContextGlobalDirectory::SymbolContextGlobalDirectory(const filesystem::path directory)
		: directory(directory) {
}

const filesystem::path SymbolContextGlobalDirectory::getDirectory() const throw() {
	return this->directory;
}

shared_ptr<SymbolEntry> SymbolContextGlobalDirectory::resolveSymbolEntry(string name) throw() {
	// if we've already put it in nonexistent, then it means
	// we've tried this before and it didn't exist, and that by checking
	// again we'll be wasting time and IO resources, and potentially creating an
	// inconsistent compilation since it turns into a race condition.
	//
	// this doesn't really matter much when attempting to resolve anything but the
	// top-most global symbols since the SymbolContextGlobalZone class will
	// be hitting each potential zone to find one that matches.
	if (this->nonexistent.count(name)) {
		return nullptr;
	}

	auto symbol = this->symbols.find(name);
	if (symbol != this->symbols.end()) {
		// we can blindly return it since we only allow
		// context references to be added in the first
		// place (see SymbolContextGlobal's implementation)
		return symbol->second;
	}

	// does a directory exist by that name?
	filesystem::path newDir = this->directory / name;
	if (newDir.exists()) {
		// make sure it's a directory
		if (!newDir.is_directory()) {
			cerr << "aruab: notice: skipping non-directory in zone search: " << newDir.str() << endl;
		} else {
			// cool, load it as a new global directory context and add it.
			shared_ptr<SymbolContext> newDirCtx(new SymbolContextGlobalDirectory(newDir));
			this->addRef(name, newDirCtx);
			return this->symbols[name];
		}
	}

	// does an arua file exist by that name?
	filesystem::path newFile = this->directory / (name + ".aa");
	if (newFile.exists()) {
		if (!newFile.is_file()) {
			cerr << "aruab: notice: skipping non-file in zone search: " << newFile.str() << endl;
		} else {
			// cool, load it as a module and add it.
			shared_ptr<SymbolContext> module = arua::parse_file(newFile.str());
			if (!module) {
				return nullptr;
			}
			this->addRef(name, module);
			return this->symbols[name];
		}
	}

	// nope! register that we've tried this before and fail.
	this->nonexistent.insert(name);
	return nullptr;
}
