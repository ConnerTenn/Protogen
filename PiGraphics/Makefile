
SHELL := /bin/bash
GCC=gcc
CFLAGS=-Wall -O3

.PHONY: all main camera camDisp clean update
all: main camera camDisp

main: main.c clean
	$(GCC) $(CFLAGS) $< -o $@

camera: camera.c clean
	$(GCC) $(CFLAGS) -lv4l2 $< -o $@

camDisp: camDisp.c clean
	$(GCC) $(CFLAGS) -lv4l2 $< -o $@

clean:
	rm -f main camera camDisp

update:
	git fetch --all; git reset --hard origin/PiGraphics;

