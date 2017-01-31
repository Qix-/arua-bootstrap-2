#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <tuple>

#include "lex-util.h"
#include "lexer.h"
#include "parse-problem.h"
#include "parser.h"
#include "ptr.h"
#include "token.h"
#include "type.h"
#include "value.h"

using namespace arua;
using namespace std;

typedef list<shared_ptr<const Token>>::const_iterator TokenListIterator;

struct Tokenizer {
	TokenListIterator &itr;
	list<tuple<Ptr<Type>, shared_ptr<vector<Token>>>> unresolvedTypes;
	list<tuple<Ptr<Value>, shared_ptr<vector<Token>>>> unresolvedValues;

	bool pub;

	Tokenizer(TokenListIterator &itr)
			: itr(itr)
			, pub(false) {
	}

	shared_ptr<const Token> operator *() const throw() {
		return *itr;
	}

	TokenListIterator operator ++() throw() {
		return ++itr;
	}

	TokenListIterator operator ++(int) throw() {
		return itr++;
	}

	TokenListIterator operator --() throw() {
		return --itr;
	}

	TokenListIterator operator --(int) throw() {
		return itr--;
	}

	const shared_ptr<const Token> * operator ->() const throw() {
		return &*itr;
	}
};

bool unexpected(Tokenizer &t) {
	cerr << "aruab: error: " << (*t)->source << ":" << (*t)->line << ":" << (*t)->columnStart << ": unexpected token: " << arua::formatToken(*t, true) << endl;
	return false;
}

bool burn(Tokenizer &t) {
	for (;; ++t) {
		switch ((*t)->type) {
		case ABT_COMMENT:
		case ABT_COMMENT_DOC:
		case ABT_COMMENT_HEADER:
		case ABT_NL:
			break;
		default:
			return true;
		}
	}
}

bool parse_whitespace(Tokenizer &t) {
	bool found = false;
	while ((*t)->type == ABT_WS) {
		found = true;
		++t;
	}
	return found ? true : unexpected(t);
}

inline bool expect(Tokenizer &t, TokenType type) {
	if ((*t)->type == type) {
		++t;
		return true;
	} else {
		return false;
	}
}

bool parse_identifier(Tokenizer &t, string &id) {
	if ((*t)->type != ABT_ID) return unexpected(t);

	id = (*t)->value;
	++t;
	return true;
}

std::shared_ptr<Module> arua::parseFile(filesystem::path filename, unsigned int tabWidth) throw() {
	auto tokens = arua::lexFile(filename, tabWidth);

	shared_ptr<Module> module(new Module(filename.str()));

	TokenListIterator vitr = tokens->cbegin();
	Tokenizer t(vitr);

	return nullptr;
}
