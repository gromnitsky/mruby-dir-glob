CFLAGS += -Wall

obj := $(patsubst %.c, %.o, $(wildcard *.c))
tests := $(patsubst %.c, %, $(wildcard test_*.c))

test_fnmatch:
	cc $(CFLAGS) ../fnmatch.o $@.o -o $@

all: $(obj) $(tests)
	@for idx in $(tests); do \
		printf "\n%s:\n" $$idx; \
		./$$idx; \
	done

.PHONY: clean
clean:
	rm -f $(tests) $(obj)
