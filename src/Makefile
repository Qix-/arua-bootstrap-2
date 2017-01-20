.PHONY: all clean

SRC := \
	cli.cc \
	aruab.cc

all: aruab

clean:
	rm -rf aruab $(SRC:.cc=.o)

aruab : $(SRC:.cc=.o)
	$(CXX) -o $@ $^ -lxopt -L../ext/xopt

%.o : %.cc
	$(CXX) -c -o $@ $< -g3 -O0 -Wall -Wextra -Werror -std=c++11 -pedantic -I../ext/xopt
