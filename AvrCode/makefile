
GCC=avr-gcc
LD=avr-ld
CFLAGS= -O3 -mmcu=atmega328p

.PHONY: all prog.hex

all: prog.hex


%.o: %.c
	$(GCC) $(CFLAGS) $< -c -o $@

%.o: %.bin
	$(LD) -r -b binary $< -o $@

FrameData.bin: bin

bin:
	python GenFrames5.py



prog.elf: main.o globals.o interfaces.o frames.o FrameData.o
	$(GCC) $(CFLAGS) $? -o $@

prog.hex: prog.elf
	avr-objcopy -j .text -j .data -O ihex prog.elf prog.hex


clean:
	rm -f *.elf *.hex *.o

upload:
	avr-size --mcu=atmega328p -A prog.elf
	avrdude -v -p atmega328p -c arduino -P $(shell ls /dev/ttyUSB* | head -n 1) -b 57600 -D  -U flash:w:prog.hex:i

com:
	stty -F $(shell ls /dev/ttyUSB* | head -n 1) 9600 -crtscts
	#cat $(shell ls /dev/ttyUSB* | head -n 1) 
	picocom -b 9600 -p 2 --imap lfcrlf --noreset $(shell ls /dev/ttyUSB* | head -n 1)

comconfig:
	stty -F $(shell ls /dev/ttyUSB* | head -n 1) 9600 \
	-parenb -parodd -cmspar cs8 -hupcl cstopb cread clocal -crtscts \
	-ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr -icrnl -ixon -ixoff -iuclc -ixany \
	-imaxbel -iutf8 \
	-opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0 \
	-isig -icanon -iexten -echo -echoe -echok -echonl -noflsh -xcase -tostop -echoprt -echoctl -echoke \
	-flusho -extproc


