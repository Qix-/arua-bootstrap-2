#include <unordered_set>

#include "ast/module.h"
#include "ast/target.h"
#include "ast/type.h"
#include "ast/type-array.h"
#include "ast/type-scalar.h"
#include "lex-util.h"
#include "lexer.h"
#include "parse-problem.h"
#include "parser.h"

using namespace arua;
using namespace std;

typedef list<shared_ptr<const Token>>::const_iterator TokenListIterator;

struct Tokenizer {
	shared_ptr<Universe> universe;
	TokenListIterator &itr;
	unordered_set<Universe::ID> deferred;

	bool pub;

	Tokenizer(shared_ptr<Universe> universe, TokenListIterator &itr)
			: universe(universe)
			, itr(itr)
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
	auto id = t.universe->addNode();
	t.universe->addToken(id, *t);
	t.universe->addError(id, "unexpected token");
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

bool parse_identifier(Tokenizer &t, shared_ptr<Identifier> &id) {
	if ((*t)->type != ABT_ID) return unexpected(t);

	id = (new Identifier(t.universe, (*t)->value))->ptr<Identifier>();
	t.universe->addToken(id->getID(), *t);
	++t;
	return true;
}

std::shared_ptr<Module> arua::parseFile(std::shared_ptr<Universe> universe, filesystem::path filename, unsigned int tabWidth) throw() {
	auto tokens = arua::lexFile(filename, tabWidth);

	shared_ptr<Module> module(new Module(universe, filename.str()));

	TokenListIterator vitr = tokens->cbegin();
	Tokenizer t(universe, vitr);

	return nullptr;
}
