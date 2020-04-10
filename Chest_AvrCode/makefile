
GCC=avr-gcc
LD=avr-ld
CFLAGS= -O3 -mmcu=atmega328p

.PHONY: all prog.hex

all: prog.hex

#Get Definitions from Defines.cfg
# GetDefine = "-D $(1)=$(shell awk -F= '$1==\"$(1)\"{print $2}')"

%.o: %.c
	$(GCC) $(CFLAGS) $(shell cat Defines.dat) $< -c -o $@

%.o: %.bin
	# $(LD) -r -b binary $< -o $@
	avr-objcopy -I binary -O elf32-avr -B avr --rename-section .data=.text,readonly,rom,noload,contents $< $@

ButtonData.bin: bin

bin:
	python3 ButtonCodes.py


prog.elf: main.o globals.o interfaces.o buttonCtl.o ButtonData.o
	$(GCC) $(CFLAGS) $? -o $@
	
prog.hex: bin prog.elf
	avr-objcopy -j .text -j .data -O ihex prog.elf prog.hex


clean:
	rm -f *.elf *.hex *.o *.dat

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


