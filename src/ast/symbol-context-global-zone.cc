#include "symbol-context-global-zone.h"

using namespace arua;
using namespace std;

SymbolContextGlobalZone::SymbolContextGlobalZone(const set<filesystem::path> &zonePaths) {
	for (const filesystem::path &zonePath : zonePaths) {
		this->zones.insert(shared_ptr<SymbolContextGlobalDirectory>(new SymbolContextGlobalDirectory(zonePath)));
	}
}

shared_ptr<SymbolEntry> SymbolContextGlobalZone::resolveSymbolEntry(string name) throw() {
	// just iterate all directory zones.
	for (shared_ptr<SymbolContextGlobal> dirCtx : this->zones) {
		auto entry = dirCtx->resolveSymbolEntry(name);
		if (entry) {
			return entry;
		}
	}

	return nullptr;
}
