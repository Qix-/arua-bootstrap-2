#ifndef ARUAB_LEXER_H__
#define ARUAB_LEXER_H__
#pragma once

#include <list>
#include <memory>

#include "filesystem/path.h"

#include "ast/token.h"

namespace arua {

// note that these WILL throw exceptions in the event there is an invalid token.
std::shared_ptr<std::list<std::shared_ptr<const Token>>> lexFile(filesystem::path path, unsigned int tabWidth = 8);
std::shared_ptr<std::list<std::shared_ptr<const Token>>> lexString(std::string source, unsigned int tabWidth = 8);

}

#endif
