
SHELL := /bin/bash
GCC=armv8-rpi3-linux-gnueabihf-gcc
LD=armv8-rpi3-linux-gnueabihf-ld
CFLAGS=-Wall -O3 -lm
LIBV4L2=libv4l2/libv4l2.so libv4l2/libv4lconvert.so libv4l2/libjpeg.so.62 #~/Dev/v4l-utils-1.16.6/lib/libv4l2/libv4l2.c ~/Dev/v4l-utils-1.16.6/lib/libv4l2/log.c ~/Dev/v4l-utils-1.16.6/lib/libv4lconvert/libv4lconvert.c

.PHONY: all main clean update
all: main


main: main.o globals.o camera.o graphics.o controller.o matrix.o clean
	$(GCC) $(CFLAGS) *.o $(LIBV4L2) -o $@

main.o: main.c clean
	$(GCC) $(CFLAGS) $< -c -o $@

globals.o: globals.c clean
	$(GCC) $(CFLAGS) $< -c -o $@

camera.o: camera.c clean
	$(GCC) $(CFLAGS) -Ilibv4l2 $< -c -o $@

graphics.o: graphics.c clean
	$(GCC) $(CFLAGS) $< -c -o $@
	
controller.o: controller.c clean
	$(GCC) $(CFLAGS) $< -c -o $@

matrix.o: matrix.c clean
	$(GCC) $(CFLAGS) $< -c -o $@

clean:
	rm -f main *.o

update:
	git fetch --all; git reset --hard origin/PiCode;

upload:
	sshpass -p "pipass" scp main root@192.168.0.133:/root/Dev/Protogen/PiCode/main

ssh:
	sshpass -p "pipass" ssh root@192.168.0.133

