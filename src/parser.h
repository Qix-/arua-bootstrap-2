#ifndef ARUAB_PARSER_H__
#define ARUAB_PARSER_H__
#pragma once

#include <memory>

#include "filesystem/path.h"

#include "module.h"

namespace arua {
	std::shared_ptr<Module> parseFile(filesystem::path filename, unsigned int tabWidth = 8) throw();
}

#endif
