.PHONY: all clean re2c xopt bignumber
all: xopt re2c bignumber

clean:
	+$(MAKE) -C xopt clean
	+$(MAKE) -C re2c clean
	rm -rf bignumber/build

xopt:
	+$(MAKE) -C xopt

re2c/re2c/configure: re2c/re2c/autogen.sh
	cd re2c/re2c && ./autogen.sh

re2c/re2c/Makefile: re2c/re2c/configure
	cd re2c/re2c && ./configure

re2c/re2c/re2c: re2c/re2c/Makefile
	+$(MAKE) -C re2c/re2c

re2c: re2c/re2c/re2c

bignumber: bignumber/build/libBigNumber.a

bignumber/build/libBigNumber.a: bignumber/build/Makefile
	+$(MAKE) -C bignumber/build

bignumber/build/Makefile: bignumber/CMakeLists.txt
	mkdir -p bignumber/build
	(cd bignumber/build && cmake -G"Unix Makefiles" ..)
