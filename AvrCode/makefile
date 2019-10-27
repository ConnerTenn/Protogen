
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



prog.elf: main.o interfaces.o FrameData.o
	$(GCC) $(CFLAGS) $? -o $@

prog.hex: prog.elf
	avr-objcopy -j .text -j .data -O ihex prog.elf prog.hex


clean:
	rm -f *.elf *.hex *.o

upload:
	avr-size --mcu=atmega328p -A prog.elf
	avrdude -v -p atmega328p -c arduino -P /dev/ttyUSB0 -b 57600 -D  -U flash:w:prog.hex:i
