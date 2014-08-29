obj := $(patsubst %.c, %.o, $(wildcard *.c))

fnmatch.c: fnmatch.h

all: $(obj)

.PHONY: clean
clean:
	rm -f $(obj)
