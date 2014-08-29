CFLAGS += -Wall

tests := $(patsubst %.c, %, $(wildcard test_*.c))

all: $(tests)
	@for idx in $(tests); do \
		printf "\n%s:\n" $$idx; \
		./$$idx; \
	done

.PHONY: clean
clean:
	rm -f $(tests)
