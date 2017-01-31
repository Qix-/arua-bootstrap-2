.SECONDARY:
.PHONY: all clean

RE2C := ../ext/re2c/re2c/re2c

SRC := \
	cli.cc \
	parser.cc \
	lexer.cc.re \
	lex-util.cc \
	parse-problem.cc \
	value.cc \
	type.cc \
	type-array.cc \
	type-function.cc \
	type-reference.cc \
	type-scalar.cc \
	context.cc \
	type-derived.cc \
	module.cc \
	aruab.cc

CCSRC := $(SRC:.cc.re=.cc)
OBJS := $(CCSRC:.cc=.o)

all: aruab

clean:
	rm -rf lexer.cc
	rm -rf aruab $(OBJS)

aruab : $(OBJS)
	$(CXX) -o $@ $^ -g3 -O0 -lxopt -L../ext/xopt

%.o : %.cc
	$(CXX) -c -o $@ $< -g3 -O0 -Wall -Wextra -Werror -std=c++11 -pedantic -I../ext/xopt -I../ext/filesystem

%.cc : %.cc.re
	$(RE2C) -s8 -W -Werror -o $@ $<
