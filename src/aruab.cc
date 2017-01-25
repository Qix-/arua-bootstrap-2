#include <iostream>
#include <stdexcept>

#include "cli.h"
#include "lexer.h"

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
