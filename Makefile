.PHONY: all clean

all:
	+$(MAKE) -C ext
	+$(MAKE) -C src

clean:
	+$(MAKE) -C ext clean
	+$(MAKE) -C src clean
