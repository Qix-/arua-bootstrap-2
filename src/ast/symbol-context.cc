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

void SymbolContext::addType(shared_ptr<Identifier> name, shared_ptr<Type> type) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getIdentifier()] = shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::TYPE, type));
}

void SymbolContext::addAlias(shared_ptr<Identifier> name, shared_ptr<Target> target) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getIdentifier()] = shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::ALIAS, target));
}

void SymbolContext::addRef(shared_ptr<Identifier> name, shared_ptr<SymbolContext> symCtx) throw() {
	if (!this->assertDoesntExist(name)) {
		// don't add the new type; it just becomes an invalid type after this if used improperly.
		return;
	}

	this->symbols[name->getIdentifier()] = shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::CTXREF, symCtx));
}

shared_ptr<SymbolEntry> SymbolContext::resolveSymbolEntry(shared_ptr<Identifier> name) const throw() {
	// resolve aliases
	auto entry = this->symbols.find(name->getIdentifier());
	if (entry == this->symbols.cend()) {
		if (this->parent) {
			return this->parent->resolveSymbolEntry(name);
		}

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
			return resolved->getContext()->resolveSymbolEntry(resolved->getIdentifier());
		}
		case TargetType::TYPE:
			return shared_ptr<SymbolEntry>(new SymbolEntry(SymbolType::TYPE, static_pointer_cast<Type>(target)));
		}
	}
	}
}

bool SymbolContext::assertDoesntExist(shared_ptr<Identifier> name) const throw() {
	shared_ptr<SymbolEntry> existingSymbol = this->resolveSymbolEntry(name);
	if (existingSymbol) {
		// TODO notify an error handler
		cerr << "duplicate symbol '" << name->getIdentifier() << "' at " << name->getLine() << ":" << name->getColumnStart() << endl;
		return false;
	}

	return true;
}
