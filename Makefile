.PHONY: all clean

all:
	+$(MAKE) -C ext
	+$(MAKE) -C src

clean:
	+$(MAKE) -C ext/xopt clean
	+$(MAKE) -C src clean
