#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <list>
#include <locale>
#include <memory>
#include <sstream>
#include <stack>
#include <string>

#include "immediate-number.h"
#include "immediate-string.h"
#include "lex-util.h"
#include "lexer.h"
#include "parser.h"
#include "ptr.h"
#include "token.h"
#include "type.h"
#include "type-array.h"
#include "type-derived.h"
#include "type-function.h"
#include "type-scalar.h"
#include "type-reference.h"
#include "value.h"

using namespace arua;
using namespace std;

typedef list<shared_ptr<const Token>> TokenList;
typedef TokenList::const_iterator TokenListIterator;

enum Parsables {
	EXPECTED   = 0x01,
	NUMBER     = 0x02,
	STRING     = 0x04,
	TYPE       = 0x08,
	UNRESOLVED = 0x10,
	IMMEDIATE  = NUMBER | STRING,
};

template <typename E>
bool has(int flags, E v) {
	return (flags & v) != 0;
}

template <typename A, typename B>
struct Tuple {
	Tuple(const Tuple &other)
			: a(other.a)
			, b(other.b) {
	}

	Tuple(A a, B b)
			: a(a)
			, b(b) {
	}

	A a;
	B b;
};

struct Tokenizer {
	stack<TokenListIterator> stack;
	list<Tuple<Ptr<Type>, shared_ptr<TokenList>>> unresolvedTypes;
	list<Tuple<Ptr<Value>, shared_ptr<TokenList>>> unresolvedValues;

	bool pub;

	Tokenizer(TokenListIterator &itr)
			: pub(false) {
		this->stack.push(itr);
	}

	TokenListIterator & top() throw() {
		return this->stack.top();
	}

	shared_ptr<const Token> operator *() throw() {
		return *(this->top());
	}

	TokenListIterator operator ++() throw() {
		return ++(this->top());
	}

	TokenListIterator operator ++(int) throw() {
		return (this->top())++;
	}

	TokenListIterator operator --() throw() {
		return --(this->top());
	}

	TokenListIterator operator --(int) throw() {
		return (this->top())--;
	}

	const shared_ptr<const Token> * operator ->() throw() {
		return &*(this->top());
	}

	void push() throw() {
		this->stack.push(this->top());
	}

	void pop() throw() {
		this->stack.pop();
	}

	void commit() throw() {
		auto okItr = this->top();
		this->stack.pop();
		this->stack.top() = okItr;
	}
};

static wstring_convert<codecvt_utf8<char32_t>, char32_t> utf8Encoder;

ostream & problem(shared_ptr<const Token> t, string severity = "error") {
	return cerr << "aruab: " << severity << ": " << t->source << ":" << t->line << ":" << t->columnStart << ": ";
}

ostream & problem(Tokenizer &t, string severity = "error") {
	return problem(*t, severity);
}

bool unexpected(Tokenizer &t) {
	problem(t) << "unexpected token: " << arua::formatToken(*t, true) << endl;
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
	return found;
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

bool parse_pub(Tokenizer &t) {
	if (!expect(t, ABT_PUB)) return unexpected(t);
	if (!parse_whitespace(t)) return unexpected(t);

	switch ((*t)->type) {
	case ABT_TYPEDEF:
	case ABT_TRAIT:
	case ABT_STRUCT:
	case ABT_USE:
	case ABT_ALIAS:
	case ABT_FN:
	case ABT_ID:
		break;
	default:
		return unexpected(t);
	}

	t.pub = true;

	return true;
}

bool parse_type(Tokenizer &t, Ptr<Type> &type, int allowed = TYPE | UNRESOLVED | EXPECTED);

bool parse_array(Tokenizer &t, Ptr<Type> &type) {
	if (!expect(t, ABT_OBRACKET)) return unexpected(t);

	Ptr<Type> inner;
	if (!parse_type(t, inner)) return false;

	auto typeArray = Ptr<TypeArray>::make();
	typeArray->setBaseType(inner);

	if (!expect(t, ABT_CBRACKET)) return unexpected(t);

	type = typeArray.as<Type>();
	return true;
}

bool parse_fn(Tokenizer &t, Ptr<Type> &type) {
	if (!expect(t, ABT_FN)) return unexpected(t);
	if (!expect(t, ABT_OPAREN)) return unexpected(t);
	parse_whitespace(t);

	auto fnType = Ptr<TypeFunction>::make();

	if (!expect(t, ABT_CPAREN)) {
		// there might be args
		Ptr<Type> firstArg;
		if (!parse_type(t, firstArg)) return false;
		fnType->addArgumentType(firstArg);

		// any other args
		while (!expect(t, ABT_CPAREN)) {
			if (!expect(t, ABT_COMMA) || !parse_whitespace(t)) return unexpected(t);
			Ptr<Type> nextArg;
			if (!parse_type(t, nextArg)) return false;
			fnType->addArgumentType(nextArg);
			parse_whitespace(t);
		}
	}

	// gotta be sneaky and check for whitespace + valid type as a return type.
	t.push();
	Ptr<Type> returnType;
	if (parse_whitespace(t) && parse_type(t, returnType, false)) {
		t.commit();
		fnType->setReturnType(returnType);
	} else {
		t.pop();
	}

	type = fnType.as<Type>();
	return true;
}

bool parse_reference(Tokenizer &t, Ptr<Type> &type) {
	if (!expect(t, ABT_AMP)) return unexpected(t);

	Ptr<Type> innerType;
	if (!parse_type(t, innerType)) return false;

	auto refType = Ptr<TypeReference>::make();
	refType->setBaseType(innerType);

	type = refType.as<Type>();
	return true;
}

bool parse_scalar(Tokenizer &t, Ptr<Type> &type) {
	if ((*t)->type != ABT_SCALAR) return unexpected(t);

	ScalarType scalarFormat;
	char clazz = (*t)->value[0];

	switch (clazz) {
	case 'u':
		scalarFormat = ScalarType::UINTEGER;
		break;
	case 'i':
		scalarFormat = ScalarType::INTEGER;
		break;
	case 'f':
		scalarFormat = ScalarType::FLOAT;
		break;
	default:
		problem(t) << "unknown scalar format (is the lexer feeling okay?): " << arua::formatToken(*t, true) << endl;
		return false;
	}

	// I know there are pitfalls with this.
	// This is the bootstrap compiler. I don't care.
	int width = atoi((*t)->value.substr(1).c_str());

	if (width == 0) {
		problem(t) << "scalar type cannot have a width of 0: " << arua::formatToken(*t, true) << endl;
		return false;
	}

	auto scalarType = Ptr<TypeScalar>::make();
	scalarType->setScalarType(scalarFormat);
	scalarType->setWidth(width);

	type = scalarType.as<Type>();
	++t;
	return true;
}

bool parse_unresolved_type(Tokenizer &t, Ptr<Type> &type) {
	if ((*t)->type != ABT_ID) return unexpected(t);

	shared_ptr<TokenList> idents(new TokenList());

	idents->push_back(*t);
	++t;

	while (expect(t, ABT_DOT)) {
		if ((*t)->type != ABT_ID) return unexpected(t);
		idents->push_back(*t);
		++t;
	}

	t.unresolvedTypes.push_back(Tuple<Ptr<Type>, shared_ptr<TokenList>>(type, idents));
	return true;
}

bool parse_unresolved_value(Tokenizer &t, Ptr<Value> &value) {
	if ((*t)->type != ABT_ID) return unexpected(t);

	shared_ptr<TokenList> idents(new TokenList());

	idents->push_back(*t);
	++t;

	while (expect(t, ABT_DOT)) {
		if ((*t)->type != ABT_ID) return unexpected(t);
		idents->push_back(*t);
		++t;
	}

	t.unresolvedValues.push_back(Tuple<Ptr<Value>, shared_ptr<TokenList>>(value, idents));
	return true;
}

bool parse_concrete_type(Tokenizer &t, Ptr<Type> &type) {
	// array?
	if ((*t)->type == ABT_OBRACKET) {
		return parse_array(t, type);
	}

	// function?
	if ((*t)->type == ABT_FN) {
		return parse_fn(t, type);
	}

	// reference?
	if ((*t)->type == ABT_AMP) {
		return parse_reference(t, type);
	}

	// scalar?
	if ((*t)->type == ABT_SCALAR) {
		return parse_scalar(t, type);
	}

	return false;
}

bool parse_immediate_number(Tokenizer &t, Ptr<Value> &value) {
	switch ((*t)->type) {
	case ABT_NUM:
	case ABT_NUM_FLOAT:
		value = Ptr<ImmediateNumber>::make((*t)->value, 10).as<Value>();
		return true;
	case ABT_NUM_RADIX: {
		int radix = atoi((*t)->value.substr(0, (*t)->value.length() - 1).c_str());
		if (radix < 0 || radix > 36) {
			problem(t) << "radix must be between 0 and 36: " << arua::formatToken(*t, true);
			return false;
		}

		if (radix == 0) {
			radix = 16;
		}

		if ((*++t)->type != ABT_NUM_EX) return unexpected(t);

		value = Ptr<ImmediateNumber>::make((*t)->value, radix).as<Value>();
		return true;
	}
	case ABT_NUM_EX:
		problem(t, "warning") << "unexpected extended number (no radix); is the lexer feeling OK?: "
			<< arua::formatToken(*t, true) << endl;
		// fallthrough
	default:
		return unexpected(t);
	}
}

static string fromHex(string s) {
	static_assert('9' < 'A', "ASCII '9' must come before 'A' for this algorithm to work");
	char32_t result = 0;
	int shift = 0;
	int digit = 0;
	int c;
	for (ssize_t i = s.length() - 1; i >= 0; i--, shift += 4) {
		c = toupper(s[i]);
		digit = c - (c < 'A' ? '0' : ('A' - 10));
		result |= digit << shift;
	}

	return utf8Encoder.to_bytes(result);
}

// also known as string literals
bool parse_immediate_string(Tokenizer &t, Ptr<Value> &value) {
	if ((*t)->type != ABT_STR_BEGIN) return unexpected(t);

	stringstream ss;

	while ((*++t)->type != ABT_STR_END) {
		string code = (*t)->value.substr(2);
		switch ((*t)->type) {
		case ABT_STR_LITERAL:
			ss << (*t)->value;
			break;
		case ABT_STR_ESCAPE:
			switch ((*t)->value[1]) {
			case 'n': ss << "\n"; break;
			case 'r': ss << "\r"; break;
			case 'f': ss << "\f"; break;
			case 't': ss << "\t"; break;
			case 'a': ss << "\a"; break;
			case 'v': ss << "\v"; break;
			case 'b': ss << "\b"; break;
			case 'U':
			case 'u':
			case 'x': ss << fromHex(code); break;
			default:
				problem(t) << "unknown literal: " << arua::formatToken(*t, true) << endl;
				return false;
			}
			break;
		default:
			return unexpected(t);
		}
	}

	value = Ptr<ImmediateString>::make(ss.str()).as<Value>();
	return true;
}

bool parse_type(Tokenizer &t, Ptr<Type> &type, int allowed) {
	if (has(allowed, TYPE) && parse_concrete_type(t, type)) {
		return true;
	}

	// make a deferred resolution type from an identifier if there is one
	if (has(allowed, UNRESOLVED) && (*t)->type == ABT_ID) {
		return parse_unresolved_type(t, type);
	}

	// otherwise, this is an invalid token
	return has(allowed, EXPECTED) ? unexpected(t) : false;
}

bool parse_value(Tokenizer &t, Ptr<Value> &value, int allowed) {
	Ptr<Type> type;
	if (has(allowed, TYPE) && parse_concrete_type(t, type)) {
		value = type.as<Value>();
		return true;
	}

	if (has(allowed, NUMBER)
			&& ((*t)->type == ABT_NUM
			|| (*t)->type == ABT_NUM_RADIX
			|| (*t)->type == ABT_NUM_FLOAT)) {
		return parse_immediate_number(t, value);
	}

	if (has(allowed, STRING) && (*t)->type == ABT_STR_BEGIN) {
		return parse_immediate_string(t, value);
	}

	if (has(allowed, UNRESOLVED) && (*t)->type == ABT_ID) {
		return parse_unresolved_value(t, value);
	}

	return false;
}

bool parse_typedef(Tokenizer &t, Ptr<Module> module) {
	if (!expect(t, ABT_TYPEDEF)) return unexpected(t);
	if (!parse_whitespace(t)) return unexpected(t);

	Ptr<Type> base;
	if (!parse_type(t, base)) return false;

	if (!parse_whitespace(t)) return unexpected(t);
	if (!expect(t, ABT_AS)) return unexpected(t);
	if (!parse_whitespace(t)) return unexpected(t);

	string name;
	if (!parse_identifier(t, name)) return false;
	parse_whitespace(t);
	if (!expect(t, ABT_NL)) return unexpected(t);

	auto derived = Ptr<TypeDerived>::make();
	derived->setBaseType(base);
	derived->setName(name);
	module->addType(derived.as<Type>(), t.pub);

	return true;
}

bool parse_module(Tokenizer &t, Ptr<Module> module) {
	while ((*t)->type != ABT_EOF) {
		if (!t.pub) {
			burn(t);
		}

		switch ((*t)->type) {
		case ABT_PUB:
			if (!parse_pub(t)) return false;
			continue;
		case ABT_EOF:
			break;
		case ABT_TYPEDEF:
			if (!parse_typedef(t, module)) return false;
			break;
		default:
			return unexpected(t);
		}

		t.pub = false;
	}

	return true;
}

bool resolve(Tokenizer &t, Ptr<Context> ctx) {
	// TODO values
	size_t typeSize;

	bool success = true;

	do {
		typeSize = t.unresolvedTypes.size();

		for (auto itr = t.unresolvedTypes.begin(); itr != t.unresolvedTypes.end();) {
			Ptr<Value> prevValue = ctx.as<Value>();
			Ptr<Value> curValue = prevValue;
			shared_ptr<const Token> lastToken;

			// attempt to resolve the identifier
			for (auto &tkn : *itr->b) {
				if (curValue->getValueType() != ValueType::CONTEXT) {
					problem(lastToken) << "trying to resolve against a symbol that is not a context: " << arua::formatToken(lastToken, true) << endl;
					success = false;
					goto remove;
				}

				Ptr<Value> tmpValue = curValue;
				curValue = curValue.as<Context>()->resolve(tkn->value, prevValue);
				prevValue = tmpValue;
				lastToken = tkn;

				if (!curValue) {
					goto noResolve;
				}
			}

			if (curValue->getValueType() != ValueType::TYPE) {
				problem(lastToken) << "resolved symbol is not a type: " << arua::formatToken(lastToken, true) << endl;
				success = false;
			} else {
				itr->a.replace(curValue.as<Type>());
			}

remove:
			itr = t.unresolvedTypes.erase(itr);
			continue;

noResolve:
			++itr;
		}
	} while (t.unresolvedTypes.size() != typeSize);

	if (typeSize > 0) {
		for (auto &pair : t.unresolvedTypes) {
			auto &stream = problem(*pair.b->begin()) << "could not resolve symbol: ";
			arua::renderTokens(*pair.b, stream);
			stream << endl;
		}

		return false;
	}

	return success;
}

Ptr<Module> arua::parseFile(filesystem::path filename, unsigned int tabWidth) {
	auto tokens = arua::lexFile(filename, tabWidth);

	auto module = Ptr<Module>::make(filename.str());

	TokenListIterator vitr = tokens->cbegin();
	Tokenizer t(vitr);

	if (!parse_module(t, module)) return nullptr;

	return resolve(t, module.as<Context>()) ? module : nullptr;
}
