#include <cstdlib>
#include <iostream>

#include "immediate-number.h"

using namespace arua;
using namespace std;

static unsigned int digitValue(char digit) {
	int result;

	if ((result = (digit - '0')) < 10) {
		return result;
	}

	if ((result = (digit - 'A')) < 26) {
		return result + 10;
	}

	if ((result = (digit - 'a')) < 26) {
		return result + 10;
	}

	return 100;
}

ImmediateNumber::ImmediateNumber(string number, unsigned int radix)
		: Immediate(ImmediateType::NUMBER) {
	zz_init(this->number);
	this->parseNumber(number, radix);
}

ImmediateNumber::~ImmediateNumber() {
	zz_clear(this->number);
}

bool ImmediateNumber::parseNumber(string number, unsigned int radix) throw() {
	if (radix < 1 || radix > 36) {
		cerr << "aruab: warning: radix passed to setNumber() is invalid; is the parser feeling OK?: " << radix << endl;
		return false;
	}

	if (radix == 10) {
		// cool, just validate and set
		for (char c : number) {
			if (digitValue(c) > 9) {
				return false;
			}
		}

		zz_set_str(this->number, number.c_str());
		return true;
	}

	bool success = true;
	zz_t num, rnum;
	zz_init(num);
	zz_init(rnum);

	for (char c : number) {
		unsigned int digit = digitValue(c);
		if (digit >= radix) {
			// bad character found
			success = false;
			goto badCharacter;
		}

		zz_muli(rnum, num, radix);
		zz_addi(num, rnum, digit);
	}

	zz_swap(this->number, num);

badCharacter:
	zz_clear(num);
	zz_clear(rnum);
	return success;
}

string ImmediateNumber::getNumber() const throw() {
	char *cstr = zz_get_str(this->number);
	string result(cstr);
	free(cstr);
	return result;
}
