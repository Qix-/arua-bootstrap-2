#include <iostream> // XXX remove this when error reporting is formalized

#include "symbol-context.h"
#include "symbol-ref.h"

using namespace arua;
using namespace std;

SymbolEntryBase::SymbolEntryBase(SymbolType type)
		: type(type) {
}

SymbolContext::SymbolContext(shared_ptr<SymbolContext> parent)
		: parent(parent) {
}

// TODO string -> Identifier
void SymbolContext::addType(string name, shared_ptr<Type> type) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name] = shared_ptr<SymbolEntry<Type>>(new SymbolEntry<Type>(SymbolType::TYPE, type));
}

// TODO string -> Identifier
void SymbolContext::addAlias(string name, shared_ptr<SymbolRef> symRef) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name] = shared_ptr<SymbolEntry<SymbolRef>>(new SymbolEntry<SymbolRef>(SymbolType::ALIAS, symRef));
}

// TODO string -> Identifier
void SymbolContext::addRef(string name, shared_ptr<SymbolContext> symCtx) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name] = shared_ptr<SymbolEntry<SymbolContext>>(new SymbolEntry<SymbolContext>(SymbolType::ALIAS, symCtx));
}

shared_ptr<SymbolEntryBase> SymbolContext::resolveSymbolEntry(string name) const throw() {
	// resolve aliases
	auto entry = this->symbols.find(name);
	if (entry == this->symbols.cend()) {
		return shared_ptr<SymbolEntryBase>(new SymbolEntryBase(SymbolType::INVALID));
	}

	switch (entry->second->type) {
	case SymbolType::TYPE:
		return entry->second;
	case SymbolType::CTXREF:
		return entry->second;
	case SymbolType::ALIAS: {
		// further resolve
		auto alias = static_pointer_cast<SymbolEntry<SymbolRef>>(entry->second)->value;
		auto aliasSym = alias->resolve();
		return aliasSym->getSymbolContext()->resolveSymbolEntry(aliasSym->getIdentifier());
	}
	default:
		if (this->parent) {
			return this->parent->resolveSymbolEntry(name);
		}

		// hopefully never hits here.
		// TODO pass to error handler
		// TODO replace with identifier line/col when I get identifiers in
		cerr << "WARNING: symbol '" << name << "' somehow defined as invalid type";// at "
//			<< name->getLine() << ":" << name->getColumn() << endl;
		return shared_ptr<SymbolEntryBase>(new SymbolEntryBase(SymbolType::INVALID));
	}
}

// TODO string -> Identifier
bool SymbolContext::assertDoesntExist(string name) const throw() {
	shared_ptr<SymbolEntryBase> existingSymbol = this->resolveSymbolEntry(name);
	if (existingSymbol->type != SymbolType::INVALID) {
		// TODO notify an error handler
		// TODO replace with full line/col when Identifiers are added
		cerr << "duplicate symbol '" << name << "'" << endl;// at " << name->getLine() << ":" << name->getColumn()
			// << " (originally declared at "
			// << existingSymbol->symbol->getLine() << ":" << existingSymbol->symbol->getColumn()
			// << ")" << endl;

		return false;
	}

	return true;
}
