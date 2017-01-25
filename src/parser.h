#ifndef ARUAB_LEXER_H__
#define ARUAB_LEXER_H__
#pragma once

#include <memory>
#include <string>

#include "ast/module.h"
#include "ast/symbol-context.h"
#include "ast/symbol-indirect.h"

namespace arua {
	std::shared_ptr<SymbolIndirect> parse_symbol_indirect(std::string symbol, std::shared_ptr<SymbolContext> symCtx);
	std::shared_ptr<Module> parse_file(std::string filename);
}

#endif
