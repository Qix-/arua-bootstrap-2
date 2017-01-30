#ifndef ARUAB_LEXER_H__
#define ARUAB_LEXER_H__
#pragma once

#include <memory>

#include "filesystem/path.h"

#include "ast/module.h"

namespace arua {
	std::shared_ptr<Module> parseFile(filesystem::path filename, unsigned int tabWidth = 8);
}

#endif
