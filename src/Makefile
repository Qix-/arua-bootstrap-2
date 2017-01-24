.PHONY: all clean

RE2C := ../ext/re2c/re2c/re2c

SRC := \
	cli.cc \
	lexer.cc.re \
	ast/module.cc \
	ast/token.cc \
	ast/type.cc \
	ast/type-scalar.cc \
	ast/type-array.cc \
	ast/symbol.cc \
	ast/type-function.cc \
	ast/type-def.cc \
	ast/symbol-ref.cc \
	ast/symbol-context.cc \
	aruab.cc

CCSRC := $(SRC:.cc.re=.cc)
OBJS := $(CCSRC:.cc=.o)

all: aruab

clean:
	rm -rf parser.cc
	rm -rf aruab $(OBJS)

aruab : $(OBJS)
	$(CXX) -o $@ $^ -lxopt -L../ext/xopt

%.o : %.cc
	$(CXX) -c -o $@ $< -g3 -O0 -Wall -Wextra -Werror -std=c++11 -pedantic -I../ext/xopt

%.cc : %.cc.re
	$(RE2C) -s8 -W -Werror -o $@ $<
