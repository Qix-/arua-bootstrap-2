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

	if (cfg.files.size() == 0) {
		throw runtime_error("no files specified");
	}

	for (string &filename : cfg.files) {
		if (filename.substr(filename.length() - 3) != ".aa") {
			cerr << "WARNING: cowardly refusing to compile file without .aa extension: " << filename << endl;
			continue;
		}

		cerr << "aruab: " << filename << endl;
		// TODO do something with output
		lex_file(filename);
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
