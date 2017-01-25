#ifndef ARUAB_LEXER_H__
#define ARUAB_LEXER_H__
#pragma once

#include <memory>
#include <string>

#include "ast/module.h"

namespace arua {
	std::shared_ptr<Module> lex_file(std::string filename);
}

#endif
