#ifndef ARUAB_CLI_H__
#define ARUAB_CLI_H__
#pragma once

#include <string>
#include <set> // don't use unordered_here - it's not a bottleneck and we definitely need to maintain order.

#include "filesystem/path.h"

namespace arua {

struct config {
	std::string mainSymbol;
	std::set<filesystem::path> zones;
	int tabWidth;
	bool help;
};

bool build_config(int argc, const char *argv[], config &cfg);

}

#endif
