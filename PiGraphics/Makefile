

GCC=gcc
CFLAGS=-Wall -O3

.PHONY: all main camera clean
all: main camera

main: main.c clean
	$(GCC) $(CFLAGS) $< -o $@

camera: camera.c clean
	$(GCC) $(CFLAGS) $< -o $@

clean:
	rm -f main

