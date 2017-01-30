#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "xopt.h"

#include "cli.h"

using namespace std;

struct free_ptr {
	void operator()(void *x) {
		free(x);
	}
};

void add_zone_path(const char *path, void *data, const xoptOption *option, bool longArg, const char **err) {
	(void) option;
	(void) longArg;

	arua::config *cfg = (arua::config *)data;

	filesystem::path fpath = path;
	if (!fpath.exists()) {
		*err = strdup(("zone path does not exist: " + fpath.str()).c_str());
		return;
	}

	if (!fpath.is_directory()) {
		*err = strdup(("zone path is not a directory: " + fpath.str()).c_str());
		return;
	}

	cfg->zones.insert(fpath);
}

static const xoptOption options[] = {
	{
		"zone",
		'Z',
		0, // we don't care about it in our setter function
		&add_zone_path,
		XOPT_TYPE_STRING,
		0,
		"Specifies a zone search path"
	},
	{
		"tab-width",
		0,
		offsetof(arua::config, tabWidth),
		0,
		XOPT_TYPE_INT,
		"4",
		"The tab width, in columns (used for error messages)"
	},
	{
		"help",
		'?',
		offsetof(arua::config, help),
		0,
		XOPT_TYPE_BOOL,
		0,
		"Shows this help message"
	},
	XOPT_NULLOPTION
};

bool arua::build_config(int argc, const char *argv[], arua::config &cfg) {
	const char *err;

	cfg.help = false;
	cfg.tabWidth = 4;

	unique_ptr<xoptContext, free_ptr> ctx(xopt_context("aruab", options, XOPT_CTX_POSIXMEHARDER | XOPT_CTX_STRICT, &err));
	if (err) {
		throw runtime_error(err);
	}

	const char **extrav = 0;
	int extrac = xopt_parse(ctx.get(), argc, argv, &cfg, &extrav, &err);
	if (err) {
		throw runtime_error(err);
	}

	if (cfg.help) {
		xoptAutohelpOptions opts;
		opts.usage = "usage: aruab [options] [file...]";
		opts.prefix = "Arua bootstrap compiler suite";
		opts.suffix = "https://github.com/arua-lang/bootstrap";
		opts.spacer = 15;

		xopt_autohelp(ctx.get(), stderr, &opts, &err);
		free(extrav);
		return true;
	}

	if (extrac < 1) {
		cerr << "aruab: error: no main symbol specified" << endl;
		return true;
	}

	if (extrac > 1) {
		cerr << "aruab: error: a single main symbol must be specified; " << extrac << " were specified" << endl;
		return true;
	}

	cfg.mainSymbol = extrav[0];
	free(extrav);

	return false;
}
