
SHELL := /bin/bash
GCC=gcc
CFLAGS=-Wall -O3

.PHONY: all main camera clean update
all: main camera

main: main.c clean
	$(GCC) $(CFLAGS) $< -o $@

camera: camera.c clean
	$(GCC) $(CFLAGS) -lv4l2 $< -o $@

clean:
	rm -f main

update:
	if [ -n "$(shell git status -s)" ]; then git fetch --all; git reset --hard origin/PiGraphics; fi

