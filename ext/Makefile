.PHONY: all clean re2c xopt bsdnt
all: xopt re2c bsdnt

clean:
	+$(MAKE) -C xopt clean
	+$(MAKE) -C re2c/re2c clean
	+$(MAKE) -C bsdnt clean

xopt:
	+$(MAKE) -C xopt

re2c/re2c/configure: re2c/re2c/autogen.sh
	cd re2c/re2c && ./autogen.sh

re2c/re2c/Makefile: re2c/re2c/configure
	cd re2c/re2c && ./configure

re2c/re2c/re2c: re2c/re2c/Makefile
	+$(MAKE) -C re2c/re2c

re2c: re2c/re2c/re2c

bsdnt: bsdnt/libbsdnt.a

bsdnt/libbsdnt.a: bsdnt/Makefile
	+$(MAKE) -C bsdnt libbsdnt.a check

bsdnt/Makefile: bsdnt/configure bsdnt/Makefile.unix
	(cd bsdnt && ./configure)
