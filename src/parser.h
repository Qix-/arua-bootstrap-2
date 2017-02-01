#ifndef ARUAB_PARSER_H__
#define ARUAB_PARSER_H__
#pragma once

#include "filesystem/path.h"

#include "module.h"
#include "ptr.h"

namespace arua {
	Ptr<Module> parseFile(filesystem::path filename, unsigned int tabWidth = 8);
}

#endif
