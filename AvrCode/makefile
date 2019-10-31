
GCC=avr-gcc
LD=avr-ld
CFLAGS= -Os -mmcu=atmega328p

.PHONY: all prog.hex

all: prog.hex


%.o: %.c
	$(GCC) $(CFLAGS) $< -c -o $@

%.o: %.bin
	$(LD) -r -b binary $< -o $@

FrameData.bin: bin

bin:
	python GenFrames.py



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
	#stty -F $(shell ls /dev/ttyUSB* | head -n 1) 9600
	#cat $(shell ls /dev/ttyUSB* | head -n 1) 
	picocom -b 9600 -p 2 --imap lfcrlf $(shell ls /dev/ttyUSB* | head -n 1)
