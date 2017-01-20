.PHONY: all clean re2c xopt
all: xopt re2c

clean:
	+$(MAKE) -C xopt clean
	+$(MAKE) -C re2c clean

xopt:
	+$(MAKE) -C xopt

re2c/re2c/configure: re2c/re2c/autogen.sh
	cd re2c/re2c && ./autogen.sh

re2c/re2c/Makefile: re2c/re2c/configure
	cd re2c/re2c && ./configure

re2c/re2c/re2c: re2c/re2c/Makefile
	+$(MAKE) -C re2c/re2c

re2c: re2c/re2c/re2c
