CFLAGS += -Wall -I ../include
VPATH := ../src ../include

obj := fnmatch.o $(patsubst %.c, %.o, $(wildcard test_*.c))
tests := $(patsubst %.c, %, $(wildcard test_*.c))

.PHONY: all
all: $(obj) $(tests)
	@for idx in $(tests); do \
		printf "\n%s:\n" $$idx; \
		./$$idx; \
	done

test_fnmatch: fnmatch.o
	cc $(CFLAGS) $< $@.o -o $@

fnmatch.o: fnmatch.h

.PHONY: clean
clean:
	rm -f $(tests) $(obj)

# Use 'gmake p-obj' to print $(obj) variable.
p-%:
	@echo $* = $($*)
	@echo $*\'s origin is $(origin $*)
