

GCC=gcc
CFLAGS=-Wall -O3

.PHONY: all main clean
all: main

main: main.c clean
	$(GCC) $(CFLAGS) $< -o $@

clean:
	rm -f main

