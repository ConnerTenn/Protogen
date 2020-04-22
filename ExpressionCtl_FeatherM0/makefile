

ARMBIN=/home/conner/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin
GCC=$(ARMBIN)/arm-none-eabi-gcc -std=gnu11
GPP=$(ARMBIN)/arm-none-eabi-g++ -std=gnu++11 -fno-threadsafe-statics -fno-rtti -fno-exceptions 
LD=$(ARMBIN)/arm-none-eabi-ld
AR=$(ARMBIN)/arm-none-eabi-ar
OBJDMP=$(ARMBIN)/arm-none-eabi-objdump
OBJCPY=$(ARMBIN)/arm-none-eabi-objcopy

export ARMBIN
export GCC
export GPP
export LD
export AR
export OBJDMP
export OBJCPY

INCLUDES= \
	-I /home/conner/.arduino15/packages/arduino/tools/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL/ \
	-I /home/conner/.arduino15/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/ 

DEFINES= \
	-D F_CPU=48000000L \
	-D __SAMD21G18A__ \

CFLAGS= -O3 -mcpu=cortex-m0plus -mthumb -Wall -Werror #-nostdlib -nostartfiles -ffreestanding

.PHONY: all bin start prog.bin clean core

all: bin start prog.bin finishinfo

start:
	@echo -e "\nCompiling...\n"

%.o: %.c
	@echo $<
	@$(GCC) $(CFLAGS) $(DEFINES) $(shell cat Defines.dat) $(INCLUDES) $< -c -o $@

%.o: %.cpp
	@echo $<
	@$(GPP) $(CFLAGS) $(DEFINES) $(shell cat Defines.dat) $(INCLUDES) $< -c -o $@

%.o: %.bin
	$(LD) -r -b binary $< -o $@
	$(OBJCPY) -I binary -O elf32-littlearm -B arm --rename-section .data=.text,readonly,rom,noload,contents $< $@

FrameData.bin: bin
DisplayData.bin: bin

bin:
	# echo -en "Testing Data" > FrameData.bin
	python GenFrames5.py
	# rm -f FrameData.bin; fallocate -l 8000 FrameData.bin



prog.elf: main.o core/core.a interfaces.o FrameData.o DisplayData.o # libraries/libraries.a
	@echo -e "\nCreating $@\n"
	@# $(GCC) $(CFLAGS) $? -o $@
	$(GCC) $(CFLAGS) -Wl,--gc-sections -Tmemmap.ld --specs=nano.specs --specs=nosys.specs $? -o $@
	$(OBJCPY) -O elf32-littlearm -j .text -j .data $@ $@ #remove extra sections


prog.bin: prog.elf
	@echo -e "\nCreating $@\n"
	@$(OBJDMP) -D $? > prog.list
	$(OBJCPY) $? -O ihex prog.hex
	$(OBJCPY) $? -O binary $@

core/core.a:
	@echo -e "\nCreating $@\n"
	$(MAKE) -C core/ -j 4 core.a

libraries/libraries.a:
	@echo -e "\nCreating $@\n"
	$(MAKE) -C libraries/ -j 4 libraries.a

clean:
	$(MAKE) -C core/ clean
	$(MAKE) -C libraries/ clean
	rm -f *.elf *.bin *.hex *.o *.a prog.list

upload:
	#avr-size --mcu=atmega328p -A prog.elf
	#avrdude -v -p atmega328p -c arduino -P $(shell ls /dev/ttyUSB* | head -n 1) -b 57600 -D  -U flash:w:prog.hex:i
	/home/conner/.arduino15/packages/arduino/tools/bossac/1.7.0-arduino3/bossac -i -d --port=$(shell ls /dev/ttyACM* | head -n 1) -U true -i -e -w -v prog.bin -R

finishinfo:
	@echo -e "\n\nDone!\n"

	@echo -e "Image Size: $(shell ls -l prog.bin | cut -d ' ' -f5) B"
	@$(ARMBIN)/arm-none-eabi-size -A prog.elf
	@echo


