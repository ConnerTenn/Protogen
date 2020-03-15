
ARMBIN=/home/conner/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin
GCC=$(ARMBIN)/arm-none-eabi-gcc -std=gnu11
GPP=$(ARMBIN)/arm-none-eabi-g++ -std=gnu++11 -fno-threadsafe-statics -fno-rtti -fno-exceptions 
LD=$(ARMBIN)/arm-none-eabi-ld
AR=$(ARMBIN)/arm-none-eabi-ar
OBJDMP=$(ARMBIN)/arm-none-eabi-objdump
OBJCPY=$(ARMBIN)/arm-none-eabi-objcopy

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

CFLAGS= -O3 -mcpu=cortex-m0plus -mthumb -Wall -Werror #-nostdlib #-nostartfiles -ffreestanding

.PHONY: all

all: core.a


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

core.a: core
	$(AR) rcs core.a *.o

clean:
	rm -f *.o *.a

# OBJS= \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/WInterrupts.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/variants/arduino_zero/variant.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/pulse_asm.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/WInterrupts.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/delay.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/hooks.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/cortex_handlers.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/itoa.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/pulse.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/startup.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_analog.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_digital.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_private.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_shift.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/samd21_host.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/avr/dtostrf.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/IPAddress.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/SERCOM.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Print.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Reset.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Stream.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Tone.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Uart.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/WMath.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/WString.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/abi.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/main.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/new.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/CDC.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/PluggableUSB.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/USB/USBCore.o

OBJS= \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/variants/arduino_zero/variant.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/delay.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/hooks.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/cortex_handlers.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/startup.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_analog.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_digital.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_private.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/wiring_shift.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/SERCOM.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Print.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Reset.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/Uart.o \
	/home/conner/.arduino15/packages/arduino/hardware/samd/1.8.5/cores/arduino/main.o
	
core: $(OBJS)

coreold:
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
	$(AR) rcs core.a variant.cpp.o
	$(AR) rcs core.a pulse_asm.S.o
	$(AR) rcs core.a USB_samd21_host.c.o
	$(AR) rcs core.a WInterrupts.c.o
	$(AR) rcs core.a avr_dtostrf.c.o
	$(AR) rcs core.a cortex_handlers.c.o
	$(AR) rcs core.a delay.c.o
	$(AR) rcs core.a hooks.c.o
	$(AR) rcs core.a itoa.c.o
	$(AR) rcs core.a pulse.c.o
	$(AR) rcs core.a startup.c.o
	$(AR) rcs core.a wiring.c.o
	$(AR) rcs core.a wiring_analog.c.o
	$(AR) rcs core.a wiring_digital.c.o
	$(AR) rcs core.a wiring_private.c.o
	$(AR) rcs core.a wiring_shift.c.o
	$(AR) rcs core.a IPAddress.cpp.o
	$(AR) rcs core.a Print.cpp.o
	$(AR) rcs core.a Reset.cpp.o
	$(AR) rcs core.a SERCOM.cpp.o
	$(AR) rcs core.a Stream.cpp.o
	$(AR) rcs core.a Tone.cpp.o
	$(AR) rcs core.a USB_CDC.cpp.o
	$(AR) rcs core.a USB_PluggableUSB.cpp.o
	$(AR) rcs core.a USB_USBCore.cpp.o
	$(AR) rcs core.a Uart.cpp.o
	$(AR) rcs core.a WMath.cpp.o
	$(AR) rcs core.a WString.cpp.o
	$(AR) rcs core.a abi.cpp.o
	$(AR) rcs core.a main.cpp.o
	$(AR) rcs core.a new.cpp.o





