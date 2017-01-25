#include <iostream>
#include <memory>
#include <stdexcept>

#include "ast/symbol-context-global-zone.h"
#include "ast/symbol-indirect.h"
#include "cli.h"
#include "parser.h"

using namespace arua;
using namespace std;

int protected_main(int argc, const char *argv[]) {
	config cfg;
	if (build_config(argc, argv, cfg)) {
		return 1;
	}

	if (cfg.zones.size() == 0) {
		cerr << "aruab: notice: no zones specified; defaulting to current directory" << endl;
		cfg.zones.insert(filesystem::path::getcwd());
	}

	shared_ptr<SymbolContextGlobalZone> globalZone(new SymbolContextGlobalZone(cfg.zones));

	// parse the main identifier
	shared_ptr<SymbolIndirect> mainSymbol = arua::parse_symbol_indirect(cfg.mainSymbol, globalZone);
	if (!mainSymbol) {
		cerr << "aruab: fatal error: invalid main symbol: " << cfg.mainSymbol << endl;
		return 1;
	}

	// resolve it!
	auto mainSym = mainSymbol->resolve();
	if (!mainSym) {
		cerr << "aruab: fatal error: failed to resolve main function: " << cfg.mainSymbol << endl;
		return 1;
	}

	return 0;
}

int main(int argc, const char *argv[]) {
	try {
		return protected_main(argc, argv);
	} catch (const exception &e) {
		cerr << "aruab: fatal error: " << e.what() << endl;
	}

	return -1;
}
