#include <iostream> // XXX remove this when error reporting is formalized

#include "symbol-context.h"
#include "symbol-ref.h"

using namespace arua;
using namespace std;

SymbolEntryBase::SymbolEntryBase(SymbolType type, shared_ptr<Symbol> symbol)
		: symbol(symbol)
		, type(type) {
}

SymbolContext::SymbolContext(shared_ptr<SymbolContext> parent)
		: parent(parent) {
}

SymbolType SymbolContext::getSymbolType(const shared_ptr<Symbol> name) const throw() {
	return this->resolveSymbolEntry(name)->type;
}

void SymbolContext::addType(shared_ptr<Symbol> name, shared_ptr<Type> type) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getLabel()] = shared_ptr<SymbolEntry<Type>>(new SymbolEntry<Type>(SymbolType::TYPE, name, type));
}

void SymbolContext::addAlias(shared_ptr<Symbol> name, shared_ptr<SymbolRef> symRef) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getLabel()] = shared_ptr<SymbolEntry<SymbolRef>>(new SymbolEntry<SymbolRef>(SymbolType::ALIAS, name, symRef));
}

shared_ptr<Type> SymbolContext::resolveType(shared_ptr<Symbol> name) const throw(runtime_error) {
	shared_ptr<SymbolEntryBase> baseSym = this->resolveSymbolEntry(name);
	switch (baseSym->type) {
	case SymbolType::TYPE:
		return static_pointer_cast<SymbolEntry<Type>>(baseSym)->value;
	case SymbolType::ALIAS:
		// should never get to this point if resolveSymbolEntry() is correct
		cerr << "WARNING: it appears resolveSymbolEntry did not fully resolve an alias; see below error message." << endl;
	default:
		// TODO notify an error handler that the given symbol is not a valid type
		// TODO somehow figure out how to not throw here.
		cerr << "symbol is not a valid type at " << name->getLine() << ":" << name->getColumn() << endl;
		throw runtime_error("fatal semantic error found in source code; aborting");
	}
}

shared_ptr<SymbolEntryBase> SymbolContext::resolveSymbolEntry(shared_ptr<Symbol> name) const throw() {
	// resolve aliases
	auto entry = this->symbols.find(name->getLabel());
	if (entry == this->symbols.cend()) {
		return shared_ptr<SymbolEntryBase>(new SymbolEntryBase(SymbolType::INVALID, name));
	}

	switch (entry->second->type) {
	case SymbolType::TYPE: {
		return entry->second;
	}
	case SymbolType::ALIAS: {
		// further resolve
		auto alias = static_pointer_cast<SymbolEntry<SymbolRef>>(entry->second)->value;
		return alias->getSymbolContext()->resolveSymbolEntry(alias->getSymbol());
	}
	default:
		if (this->parent) {
			return this->resolveNextSymbolEntry(name);
		}

		// hopefully never hits here.
		// TODO pass to error handler
		cerr << "WARNING: symbol '" << name->getLabel() << "' somehow defined as invalid type at "
			<< name->getLine() << ":" << name->getColumn() << endl;
		return shared_ptr<SymbolEntryBase>(new SymbolEntryBase(SymbolType::INVALID, name));
	}
}

shared_ptr<SymbolEntryBase> SymbolContext::resolveNextSymbolEntry(shared_ptr<Symbol> name) const throw() {
}

bool SymbolContext::assertDoesntExist(const std::shared_ptr<Symbol> name) const throw() {
	shared_ptr<SymbolEntryBase> existingSymbol = this->resolveSymbolEntry(name);
	if (existingSymbol->type != SymbolType::INVALID) {
		// TODO notify an error handler
		cerr << "duplicate symbol at " << name->getLine() << ":" << name->getColumn()
			<< " (originally declared at "
			<< existingSymbol->symbol->getLine() << ":" << existingSymbol->symbol->getColumn()
			<< ")" << endl;

		return false;
	}

	return true;
}
