###############################################################################
# Makefile for the project usb-relay
###############################################################################

## General Flags
PROJECT = usb-relay
MCU = atmega32
TARGET = usb-relay.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99         -DF_CPU=12000000UL -O0 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=usb-relay.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\asm\avrusb-20080513\examples\usb-relay\firmware-mega16\." -I"C:\asm\avrusb-20080513\examples\usb-relay\firmware-mega16\usbdrv" -I"C:\asm\avrusb-20080513\examples\usb-relay\firmware-mega16\..\common" 

## Objects that must be built in order to link
OBJECTS = usb-relay.o usbdrv.o usbdrvasm.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) usb-relay.hex usb-relay.eep usb-relay.lss size

## Compile
usbdrvasm.o: ../usbdrv/usbdrvasm.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

usb-relay.o: ../usb-relay.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usbdrv.o: ../usbdrv/usbdrv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) usb-relay.elf dep/* usb-relay.hex usb-relay.eep usb-relay.lss usb-relay.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

