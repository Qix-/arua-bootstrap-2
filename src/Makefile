.PHONY: all clean

RE2C := ../ext/re2c/re2c/re2c

SRC := \
	cli.cc \
	parser.cc \
	lexer.cc.re \
	lex-util.cc \
	value.cc \
	type.cc \
	type-array.cc \
	type-function.cc \
	type-reference.cc \
	type-scalar.cc \
	context.cc \
	type-derived.cc \
	module.cc \
	constant.cc \
	immediate.cc \
	immediate-number.cc \
	immediate-string.cc \
	aruab.cc

CCSRC := $(SRC:.cc.re=.cc)
OBJS := $(addsuffix .o,$(basename $(CCSRC)))
DEPS := $(addsuffix .d,$(basename $(CCSRC)))

CPPFLAGS += -g3 -O0 -Wall -Wextra -Werror -std=c++11 -pedantic -I../ext/xopt -I../ext/filesystem -I../ext/bsdnt

all: aruab

clean:
	-rm -rf aruab
	-rm -rf lexer.cc
	-rm -rf $(DEPS)
	-rm -rf $(OBJS)

aruab : $(OBJS)
	$(CXX) -o $@ $^ -g3 -O0 -lxopt -lbsdnt -L../ext/xopt -L../ext/bsdnt

%.o : %.cc
	$(CXX) -c -o $@ $< $(CPPFLAGS)

%.cc : %.cc.re
	$(RE2C) -s8 -W -Werror -o $@ $<

%.d : %.cc
	@set -e; rm -f $@; \
	$(CXX) -M $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@;

-include $(DEPS)
