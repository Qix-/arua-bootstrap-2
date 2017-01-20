#include <iostream>
#include <stdexcept>

#include "cli.h"
#include "parser.h"

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

	for (string &filename : cfg.files) {
		cerr << "aruab: " << filename << endl;
		// TODO do something with output
		parse_file(filename);
	}

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
