
ARMBIN=/home/conner/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin
GCC=$(ARMBIN)/arm-none-eabi-gcc -std=gnu11
GPP=$(ARMBIN)/arm-none-eabi-g++ -std=gnu++11 -fno-threadsafe-statics -fno-rtti -fno-exceptions 
LD=$(ARMBIN)/arm-none-eabi-ld
AR=$(ARMBIN)/arm-none-eabi-ar
OBJDMP=$(ARMBIN)/arm-none-eabi-objdump
OBJCPY=$(ARMBIN)/arm-none-eabi-objcopy

INCLUDES= \
	-I/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/libraries/ \
	-I/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/libraries/SPI/ \
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

CFLAGS= -O3 -mcpu=cortex-m0plus -mthumb -Wall -Werror #-nostdlib #-nostartfiles -ffreestanding

.PHONY: all

all: libraries.a


%.o: %.c
	@echo $<
	@$(GCC) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) $< -c -o $(notdir $@)

%.o: %.cpp
	@echo $<
	@$(GPP) $(CFLAGS) -ffunction-sections -fdata-sections $(DEFINES) $(INCLUDES) $< -c -o $(notdir $@)

%.o: %.S
	@echo $<
	@$(GCC) $(CFLAGS) -x assembler-with-cpp $(DEFINES) $(INCLUDES) $< -c -o $(notdir $@)

%.o: %.bin
	# $(LD) -r -b binary $< -o $@
	# avr-objcopy -I binary -O elf32-avr -B avr --rename-section .data=.text,readonly,rom,noload,contents $< $@

libraries.a: libraries
	$(AR) rcs libraries.a *.o

clean:
	rm -f *.o *.a

OBJS= #\
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/libraries/SPI/SPI.o
	
libraries: $(OBJS)

