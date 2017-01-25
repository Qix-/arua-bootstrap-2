#include <iostream> // XXX remove this when error reporting is formalized

#include "symbol.h"
#include "symbol-direct.h"
#include "symbol-indirect.h"
#include "symbol-context.h"

using namespace arua;
using namespace std;

SymbolEntry::SymbolEntry(SymbolType type, const shared_ptr<SymbolVariant> value)
		: type(type)
		, value(value) {
}

SymbolContext::SymbolContext(shared_ptr<SymbolContext> parent)
		: parent(parent) {
}

void SymbolContext::addType(shared_ptr<Identifier> name, shared_ptr<Type> type, bool pub) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getIdentifier()] = shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::TYPE, type));

	if (pub) {
		this->publicSymbols.insert(name->getIdentifier());
	}
}

void SymbolContext::addAlias(shared_ptr<Identifier> name, shared_ptr<Target> target, bool pub) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getIdentifier()] = shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::ALIAS, target));

	if (pub) {
		this->publicSymbols.insert(name->getIdentifier());
	}
}

void SymbolContext::addRef(shared_ptr<Identifier> name, shared_ptr<SymbolContext> symCtx, bool pub) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getIdentifier()] = shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::CTXREF, symCtx));

	if (pub) {
		this->publicSymbols.insert(name->getIdentifier());
	}
}

shared_ptr<SymbolEntry> SymbolContext::resolveSymbolEntry(shared_ptr<Identifier> name, shared_ptr<SymbolContext> baseCtx) throw() {
	// resolve aliases
	auto entry = this->symbols.find(name->getIdentifier());
	if (entry == this->symbols.cend()) {
		if (this->parent) {
			return this->parent->resolveSymbolEntry(name, baseCtx);
		}

		return nullptr;
	}

	if (!this->publicSymbols.count(name->getIdentifier()) && !this->isCtxChild(baseCtx)) {
		return nullptr;
	}

	switch (entry->second->type) {
	case SymbolType::TYPE:
	case SymbolType::CTXREF:
		return entry->second;
	case SymbolType::ALIAS: {
		// further resolve
		auto target = static_pointer_cast<Target>(entry->second->value);

		switch (target->getTargetType()) {
		case TargetType::SYMBOL: {
			auto resolved = static_pointer_cast<SymbolIndirect>(target)->resolve();
			if (!resolved) {
				return nullptr;
			}
			return resolved->getContext()->resolveSymbolEntry(resolved->getIdentifier(), baseCtx);
		}
		case TargetType::TYPE:
			return shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::TYPE, static_pointer_cast<Type>(target)));
		}
	}
	}
}

bool SymbolContext::isCtxChild(shared_ptr<SymbolContext> child) const throw() {
	while (child->parent) {
		if (child->parent.get() == this) {
			return true;
		}

		child = child->parent;
	}

	return false;
}

bool SymbolContext::assertDoesntExist(shared_ptr<Identifier> name) const throw() {
	auto existingSymbol = this->symbols.find(name->getIdentifier());
	if (existingSymbol != this->symbols.end()) {
		// TODO notify an error handler
		cerr << "aruab: error: duplicate symbol '" << name->getIdentifier() << "' at " << name->getLine() << ":" << name->getColumnStart() << endl;
		return false;
	}

	return true;
}
