#ifndef ARUAB_CLI_H__
#define ARUAB_CLI_H__
#pragma once

#include <string>
#include <vector>

namespace arua {

struct config {
	std::vector<std::string> files;
	bool help;
};

bool build_config(int argc, const char *argv[], config &cfg);

}

#endif
