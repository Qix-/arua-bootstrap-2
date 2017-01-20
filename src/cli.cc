#include <cstdlib>
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

static const xoptOption options[] = {
	{
		"help",
		'?',
		offsetof(arua::config, help),
		0,
		XOPT_TYPE_BOOL,
		0,
		"Shows this help message"
	}
};

bool arua::build_config(int argc, const char *argv[], arua::config &cfg) {
	const char *err;

	unique_ptr<xoptContext, free_ptr> ctx(xopt_context("aruab", options, XOPT_CTX_POSIXMEHARDER | XOPT_CTX_STRICT, &err));
	if (err) {
		throw runtime_error(err);
	}

	const char **extrav = 0;
	int extrac = xopt_parse(ctx.get(), argc, argv, &cfg, &extrav, &err);
	if (err) {
		free(extrav);
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

	for (int i = 0; i < extrac; i++) {
		cfg.files.push_back(extrav[i]);
	}
	free(extrav);

	return false;
}
