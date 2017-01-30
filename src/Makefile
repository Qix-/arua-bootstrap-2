.SECONDARY:
.PHONY: all clean

RE2C := ../ext/re2c/re2c/re2c

SRC := \
	cli.cc \
	parser.cc \
	lexer.cc.re \
	ast/module.cc \
	ast/type.cc \
	ast/type-scalar.cc \
	ast/type-array.cc \
	ast/type-function.cc \
	ast/symbol-context.cc \
	ast/identifier.cc \
	ast/type-symbol.cc \
	ast/target.cc \
	ast/symbol-context-global.cc \
	ast/symbol-context-global-directory.cc \
	ast/symbol-context-global-zone.cc \
	ast/universe.cc \
	ast/node.cc \
	ast/problem.cc \
	ast/lex-util.cc \
	aruab.cc

CCSRC := $(SRC:.cc.re=.cc)
OBJS := $(CCSRC:.cc=.o)

all: aruab

clean:
	rm -rf parser.cc
	rm -rf aruab $(OBJS)

aruab : $(OBJS)
	$(CXX) -o $@ $^ -g3 -O0 -lxopt -L../ext/xopt

%.o : %.cc
	$(CXX) -c -o $@ $< -g3 -O0 -Wall -Wextra -Werror -std=c++11 -pedantic -I../ext/xopt -I../ext/filesystem

%.cc : %.cc.re
	$(RE2C) -s8 -W -Werror -o $@ $<
