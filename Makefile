CFLAGS += -Wall

obj := $(patsubst %.c, %.o, $(wildcard *.c))

all: $(obj)

fnmatch.c: fnmatch.h

.PHONY: clean
clean:
	rm -f $(obj)
