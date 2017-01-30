#ifndef ARUAB_LEX_UTIL_H__
#define ARUAB_LEX_UTIL_H__
#pragma once

#include <ostream>
#include <list>
#include <memory>
#include <string>

#include "ast/token.h"

namespace arua {
	std::string formatToken(const std::shared_ptr<Token> tkn, bool human = false);
	void renderTokens(std::list<std::shared_ptr<Token>> &tokens, std::ostream &stream);
	void renderTokens(std::list<std::shared_ptr<Token>>::const_iterator begin, std::list<std::shared_ptr<Token>>::const_iterator end, std::ostream &stream);
}

#endif
