
GCC=avr-gcc
CFLAGS= -Os -mmcu=atmega328p

.PHONY: prog.hex

all: prog.hex

prog.elf: main.o interfaces.o
	$(GCC) $(CFLAGS) *.o -o prog.elf

main.o:
	$(GCC) $(CFLAGS) main.c -c -o main.o

interfaces.o:
	$(GCC) $(CFLAGS) interfaces.c -c -o interfaces.o

prog.hex: prog.elf
	avr-objcopy -j .text -j .data -O ihex prog.elf prog.hex

clean:
	rm -f *.elf *.hex *.o

upload:
	avrdude -v -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D  -U flash:w:prog.hex:i
