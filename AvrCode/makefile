
GCC=avr-gcc
CFLAGS= -Os -g -mmcu=atmega328p

.PHONY: prog.hex

all: prog.hex

prog.elf:
	$(GCC) $(CFLAGS) main.c -o prog.elf

prog.hex: prog.elf
	avr-objcopy -j .text -j .data -O ihex prog.elf prog.hex

clean:
	rm -f *.elf *.hex *.o

upload:
	avrdude -v -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D  -U flash:w:prog.hex:i
