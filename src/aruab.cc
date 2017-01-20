#include <iostream>
#include <stdexcept>

#include "cli.h"

using namespace arua;
using namespace std;

int protected_main(int argc, const char *argv[]) {
	config cfg;
	if (build_config(argc, argv, cfg)) {
		return 1;
	}

	if (cfg.files.size() == 0) {
		throw runtime_error("no files specified");
	}

	cerr << "aruab: compiling " << cfg.files.size() << " file(s)" << endl;

	return 0;
}

int main(int argc, const char *argv[]) {
	try {
		return protected_main(argc, argv);
	} catch (const exception &e) {
		cerr << "FATAL ERROR: " << e.what() << endl;
	}

	return -1;
}
