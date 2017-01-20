.PHONY: all clean

RE2C := ../ext/re2c/re2c/re2c

SRC := \
	cli.cc \
	lexer.cc.re \
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