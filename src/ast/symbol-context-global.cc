#include <iostream>

#include "symbol-context-global.h"

using namespace arua;
using namespace std;

void SymbolContextGlobal::addType(shared_ptr<Identifier> name, shared_ptr<Type> type, bool pub) throw() {
	(void) type;
	(void) pub;

	// TODO use formal error handler
	cerr << "aruab: error: cannot add type definitions to global symbol contexts (at "
		<< name->getLine() << ":" << name->getColumnStart() << ")" << endl;
}

void SymbolContextGlobal::addAlias(shared_ptr<Identifier> name, shared_ptr<Target> target, bool pub) throw() {
	(void) target;
	(void) pub;

	// TODO use formal error handler
	cerr << "aruab: error: cannot add aliases to global symbol contexts (at "
		<< name->getLine() << ":" << name->getColumnStart() << ")" << endl;
}

void SymbolContextGlobal::addRef(shared_ptr<Identifier> name, shared_ptr<SymbolContext> symCtx, bool pub) throw() {
	(void) symCtx;
	(void) pub;

	// TODO use formal error handler
	cerr << "aruab: error: cannot (directly) add context references to global symbol contexts (via identifiers) - use the string overload (at "
		<< name->getLine() << ":" << name->getColumnStart() << ")" << endl;
}

void SymbolContextGlobal::addRef(string name, shared_ptr<SymbolContext> symCtx) throw() {
	if (this->symbols.count(name)) {
		// TODO notify official error handler
		cerr << "aruab: error: duplicate global symbol found: " << name << endl;
		return;
	}

	this->symbols[name] = shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::CTXREF, symCtx));
	this->publicSymbols.insert(name);
}

shared_ptr<SymbolEntry> SymbolContextGlobal::resolveSymbolEntry(shared_ptr<Identifier> name, shared_ptr<SymbolContext> baseCtx) throw() {
	(void) baseCtx; // not needed as everything within this context is public by design.
	return this->resolveSymbolEntry(name->getIdentifier());
}
