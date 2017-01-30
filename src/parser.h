#ifndef ARUAB_PARSER_H__
#define ARUAB_PARSER_H__
#pragma once

#include <memory>

#include "filesystem/path.h"

#include "ast/module.h"
#include "ast/universe.h"

namespace arua {
	std::shared_ptr<Module> parseFile(std::shared_ptr<Universe> universe, filesystem::path filename, unsigned int tabWidth = 8) throw();
}

#endif
