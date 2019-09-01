
SHELL := /bin/bash
GCC=armv8-rpi3-linux-gnueabihf-gcc
CFLAGS=-Wall -O3
LIBV4L2=-Ilibv4l2 libv4l2/libv4l2.so libv4l2/libv4lconvert.so libv4l2/libjpeg.so.62 #~/Dev/v4l-utils-1.16.6/lib/libv4l2/libv4l2.c ~/Dev/v4l-utils-1.16.6/lib/libv4l2/log.c ~/Dev/v4l-utils-1.16.6/lib/libv4lconvert/libv4lconvert.c

.PHONY: all main camera camDisp clean update
all: main camera camDisp

main: main.c clean
	$(GCC) $(CFLAGS) $< -o $@

camera: camera.c clean
	$(GCC) $(CFLAGS) $(LIBV4L2) $< -o $@

camDisp: camDisp.c clean
	$(GCC) $(CFLAGS) $(LIBV4L2) $< -o $@

clean:
	rm -f main camera camDisp

update:
	git fetch --all; git reset --hard origin/PiGraphics;

upload:
	sshpass -p "pipass" scp main camera camDisp root@192.168.0.132:/root/Dev/Protogen/PiGraphics

