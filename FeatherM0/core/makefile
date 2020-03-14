

INCLUDES= \
	-I/home/conner/.arduino15/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Include/ \
	-I/home/conner/.arduino15/packages/arduino/tools/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL/ \
	-I/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino \
	-I/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/variants/arduino_zero 
	
DEFINES= \
	-DF_CPU=48000000L \
	-DARDUINO=10812 \
	-DARDUINO_SAMD_ZERO \
	-DARDUINO_ARCH_SAMD \
	-D__SAMD21G18A__ \
	-DUSB_VID=0x2341 \
	-DUSB_PID=0x804d \
	-DUSBCON \
	"-DUSB_MANUFACTURER=\"Arduino LLC\"" \
	"-DUSB_PRODUCT=\"Arduino Zero\""

CFLAGS= -O3 -mcpu=cortex-m0plus -mthumb -Wall -Werror -nostdlib #-nostartfiles -ffreestanding

.PHONY: all prog.hex

all: prog.hex


%.o: %.c
	$(GCC) $(CFLAGS) $(DEFINES) $(INCLUDES) $< -c -o $@

%.o: %.bin
	# $(LD) -r -b binary $< -o $@
	# avr-objcopy -I binary -O elf32-avr -B avr --rename-section .data=.text,readonly,rom,noload,contents $< $@

# FrameData.bin: bin

# bin:
# 	python GenFrames5.py
	# rm FrameData.bin; fallocate -l 2000 FrameData.bin



prog.elf: main.o
	$(GCC) $(CFLAGS) $? -o $@

# prog.hex: prog.elf
# 	avr-objcopy -j .text -j .data -O ihex prog.elf prog.hex

prog.bin: prog.elf
	arm-none-eabi-objdump -D $? > prog.list
	arm-none-eabi-objcopy $? -O binary $@


clean:
	rm -f *.elf *.bin *.o prog.list

upload:
	#avr-size --mcu=atmega328p -A prog.elf
	#avrdude -v -p atmega328p -c arduino -P $(shell ls /dev/ttyUSB* | head -n 1) -b 57600 -D  -U flash:w:prog.hex:i
	/home/conner/.arduino15/packages/arduino/tools/bossac/1.7.0-arduino3/bossac -i -d --port=$(shell ls /dev/ttyACM* | head -n 1) -U true -i -e -v prog.bin -R

# com:
# 	stty -F $(shell ls /dev/ttyUSB* | head -n 1) 9600 -crtscts
# 	#cat $(shell ls /dev/ttyUSB* | head -n 1) 
# 	picocom -b 9600 -p 2 --imap lfcrlf --noreset $(shell ls /dev/ttyUSB* | head -n 1)

# comconfig:
# 	stty -F $(shell ls /dev/ttyUSB* | head -n 1) 9600 \
# 	-parenb -parodd -cmspar cs8 -hupcl cstopb cread clocal -crtscts \
# 	-ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr -icrnl -ixon -ixoff -iuclc -ixany \
# 	-imaxbel -iutf8 \
# 	-opost -olcuc -ocrnl -onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0 \
# 	-isig -icanon -iexten -echo -echoe -echok -echonl -noflsh -xcase -tostop -echoprt -echoctl -echoke \
# 	-flusho -extproc

dump:
	# arm-none-eabi-gcc -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -c start.c -o start.o
	$(GCC) -O3 -mcpu=cortex-m0plus main.c -c -o main.o

	$(GPP) -L/tmp/arduino_build_749933 -Wl,--gc-sections -save-temps -T/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/variants/arduino_zero/linker_scripts/gcc/flash_with_bootloader.ld -Wl,-Map,/tmp/arduino_build_749933/Blink.ino.map --specs=nano.specs --specs=nosys.specs -mcpu=cortex-m0plus -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -o prog.elf main.o /tmp/arduino_build_749933/core/variant.cpp.o -Wl,--start-group -L/home/conner/.arduino15/packages/arduino/tools/CMSIS/4.5.0/CMSIS/Lib/GCC/ -larm_cortexM0l_math -lm /tmp/arduino_build_749933/core/core.a -Wl,--end-group
	
	/home/conner/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-objdump -D prog.elf > prog.list
	/home/conner/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-objcopy prog.elf -O binary prog.bin

dump2:
	# arm-none-eabi-gcc -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -c start.c -o start.o
	$(GCC) -O3 -mcpu=cortex-m0plus main.c -c -o main.o
	
	$(GCC) -Wl,--gc-sections -Tmemmap.ld --specs=nano.specs --specs=nosys.specs -mcpu=cortex-m0plus -mthumb main.o /tmp/arduino_build_749933/core/variant.cpp.o /tmp/arduino_build_749933/core/core.a -o prog.elf
	
	$(OBJDMP) -D prog.elf > prog.list
	$(OBJCPY) prog.elf -O binary prog.bin
	#-larm_cortexM0l_math -lm 

core:
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/variants/arduino_zero/variant.cpp -c -o variant.cpp.o
	$(GCC) -x assembler-with-cpp $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/pulse_asm.S -c -o pulse_asm.S.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/WInterrupts.c -c -o WInterrupts.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/delay.c -c -o delay.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/hooks.c -c -o hooks.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/cortex_handlers.c -c -o cortex_handlers.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/itoa.c -c -o itoa.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/pulse.c -c -o pulse.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/startup.c -c -o startup.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring.c -c -o wiring.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_analog.c -c -o wiring_analog.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_digital.c -c -o wiring_digital.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_private.c -c -o wiring_private.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_shift.c -c -o wiring_shift.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/samd21_host.c -c -o USB_samd21_host.c.o
	$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/avr/dtostrf.c -c -o avr_dtostrf.c.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/IPAddress.cpp -c -o IPAddress.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/SERCOM.cpp -c -o SERCOM.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Print.cpp -c -o Print.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Reset.cpp -c -o Reset.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Stream.cpp -c -o Stream.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Tone.cpp -c -o Tone.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Uart.cpp -c -o Uart.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/WMath.cpp -c -o WMath.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/WString.cpp -c -o WString.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/abi.cpp -c -o abi.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/main.cpp -c -o main.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/new.cpp -c -o new.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/CDC.cpp -c -o USB_CDC.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/PluggableUSB.cpp -c -o USB_PluggableUSB.cpp.o
	$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) /home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/USBCore.cpp -c -o USB_USBCore.cpp.o
	$(AR) rcs core.a *.o





