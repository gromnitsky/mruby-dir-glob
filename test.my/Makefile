CFLAGS += -Wall

obj := $(patsubst %.c, %.o, $(wildcard *.c))
tests := $(patsubst %.c, %, $(wildcard test_*.c))

all: $(obj) $(tests)
	@for idx in $(tests); do \
		printf "\n%s:\n" $$idx; \
		./$$idx; \
	done

test_fnmatch:
	cc $(CFLAGS) ../fnmatch.o $@.o -o $@

.PHONY: clean
clean:
	rm -f $(tests) $(obj)
