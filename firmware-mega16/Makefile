# ���: Makefile
# ������: usb-relay firmware

DEVICE  = atmega32
F_CPU   = 12000000	# � ������

# � ATMega16 ���������� ��������� ����� ��������� � ATMega8 - ������� ������ � 2 ����� FUSE_H (6 � 7)
FUSE_L  = CF
FUSE_H  = 98
LOCKOPT = EF
#AVRDUDE = avrdude -c jtag2 -p $(DEVICE) -P usb:xx -v 
JTAGICEII = "c:/Program Files/Atmel/AVR Tools/JTAGICEmkII/jtagiceii.exe" -d $(DEVICE) -mi

CFLAGS  = -Iusbdrv -I. -I../common -DDEBUG_LEVEL=0
OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o usb-relay.o

#COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
#COMPILE = avr-gcc -Wall -O0 -gdwarf-2 -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
COMPILE = avr-gcc -Wall -Os -gstabs -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
RAR = "c:/Program Files/WinRAR/WinRAR.exe"

##############################################################################
# �������� ��������� (������) ��� ��������� ����� �����������������
##############################################################################
# ���� ��� ��������������� �� ������������ �����, ��. ������
#  http://palmavr.sourceforge.net/cgi-bin/fc.cgi
#  � �������� ����� ��� ������� ������ � ���������� �������� �������� �������
#
################################## ATMega8 ##################################
# ATMega8 FUSE_L (������� ���� ������):
# 0x9f = 1 0 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (������� �������� � �������� > 8 ���)
#        | |  +--------------- SUT 1..0 (��������� � ����������, BOD ��������)
#        | +------------------ BODEN (BrownOut Detector ��������)
#        +-------------------- BODLEVEL (2.7V)
# ATMega8 FUSE_H (������� ���� ������):
# 0xc9 = 1 1 0 0   1 0 0 1 <-- BOOTRST (������ ������ �������� 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (�� ��������� EEPROM ��� �������� ����)
#        | | | +-------------- CKOPT (������ ������ ������ ����������)
#        | | +---------------- SPIEN (��������� ���������������� ����������������)
#        | +------------------ WDTON (WDT ������� �� ������)
#        +-------------------- RSTDISBL (����� ������ ���������)
#
################################## ATMega16 ##################################
# ATMega16 FUSE_L (������� ���� ������):
# 0xff = 1 1 1 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (������� �������� � �������� > 8 ���)
#        | |  +--------------- SUT 1..0 (��������� � ����������, ��������� ���������� ������ �������)
#        | +------------------ BODEN (BrownOut Detector ��������)
#        +-------------------- BODLEVEL (2.7V)
# ATMega16 FUSE_H (������� ���� ������):
# 0x09 = 0 0 0 0   1 0 0 1 <-- BOOTRST (������ ������ �������� 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (�� ��������� EEPROM ��� �������� ����)
#        | | | +-------------- CKOPT (������ ������ ������ ����������)
#        | | +---------------- SPIEN (��������� ���������������� ����������������)
#        | +------------------ JTAGEN (����������������, JTAG ��������)
#        +-------------------- OCDEN (����������������, OCD ��������)
#
############################## ATMega48/88/168 ##############################
# ATMega*8 FUSE_L (������� ���� ������):
# 0xdf = 1 1 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (������� �������� � �������� > 8 ���)
#        | |  +--------------- SUT 1..0 (��������� � ����������, BOD ��������)
#        | +------------------ CKOUT (���� 0: ����� ������ ��������)
#        +-------------------- CKDIV8 (���� 0: ������� �������� ������ 8)
# ATMega*8 FUSE_H (������� ���� ������):
# 0xde = 1 1 0 1   1 1 1 0
#        ^ ^ ^ ^   ^ \-+-/
#        | | | |   |   +------ BODLEVEL 0..2 (110 = 1.8 V)
#        | | | |   + --------- EESAVE (��������� EEPROM ��� �������� ����)
#        | | | +-------------- WDTON (���� 0: watchdog ������ �������)
#        | | +---------------- SPIEN (��������� ���������������� ����������������)
#        | +------------------ DWEN (��������� debug wire)
#        +-------------------- RSTDISBL (����� ������ ���������)
#
############################## ATTiny25/45/85 ###############################
# ATMega*5 FUSE_L (������� ���� ������):
# 0xef = 1 1 1 0   1 1 1 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (����� �������� ������� -> �������� @ 12 MHz)
#        | |  +--------------- SUT 1..0 (BOD ��������, ������� ���������� ������ �������)
#        | +------------------ CKOUT (����� ������ �� ����� CKOUT -> ��������)
#        +-------------------- CKDIV8 (�������� ������ �� 8 -> ��������)
# ATMega*5 FUSE_H (������� ���� ������):
# 0xdd = 1 1 0 1   1 1 0 1
#        ^ ^ ^ ^   ^ \-+-/ 
#        | | | |   |   +------ BODLEVEL 2..0 (������� �������� brownout -> 2.7V)
#        | | | |   +---------- EESAVE (��������� EEPROM ��� �������� ���� -> ���)
#        | | | +-------------- WDTON (watchdog ������ ������� -> ���������)
#        | | +---------------- SPIEN (��������� ���������������� ���������������� -> ��)
#        | +------------------ DWEN (��������� debug wire)
#        +-------------------- RSTDISBL (������ �������� ������ -> ���)
#
################################ ATTiny2313 #################################
# ATTiny2313 FUSE_L (������� ���� ������):
# 0xef = 1 1 1 0   1 1 1 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (����� �������� ������� -> �������� @ 12 MHz)
#        | |  +--------------- SUT 1..0 (BOD ��������, ������� ���������� ������ �������)
#        | +------------------ CKOUT (����� ������ �� ����� CKOUT -> ��������)
#        +-------------------- CKDIV8 (�������� ������ �� 8 -> ��������)
# ATTiny2313 FUSE_H (������� ���� ������):
# 0xdb = 1 1 0 1   1 0 1 1
#        ^ ^ ^ ^   \-+-/ ^
#        | | | |     |   +---- RSTDISBL (������ �������� ������ -> ���)
#        | | | |     +-------- BODLEVEL 2..0 (������� �������� brownout -> 2.7V)
#        | | | +-------------- WDTON (watchdog ������ ������� -> ���������)
#        | | +---------------- SPIEN (��������� ���������������� ���������������� -> ��)
#        | +------------------ EESAVE (��������� EEPROM ��� �������� ���� -> ���)
#        +-------------------- DWEN (��������� debug wire)


# ������������� ���� (symbolic targets):
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build usb-relay.hex"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware"
	@echo "make clean ..... to delete objects and hex file"
	@echo "make erase ..... erase AVR chip" 
	@echo "make backup ..... backup project"
	
hex: usb-relay.hex

program: flash fuse

# ������� ��� ���������������� ��� ������:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
#	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m
	$(JTAGICEII) -f 0x$(FUSE_H)$(FUSE_L) 
	
# ������� ��� �������� firmware:
flash: usb-relay.hex
#	$(AVRDUDE) -U flash:w:usb-relay.hex:i
	$(JTAGICEII) -pf -if usb-relay.hex 
	
# ������� ��� �������� ������ ������������ (������� ����� ���� ��������� �������� Make):
clean:
	rm -f usb-relay.hex usb-relay.lst usb-relay.obj usb-relay.cof usb-relay.list usb-relay.map usb-relay.eep.hex usb-relay.elf *.o usbdrv/*.o usb-relay.s usbdrv/oddebug.s usbdrv/usbdrv.s
	
# ������� (generic) ������� ��� �������������� ������ �� ����� C:
.c.o:
	$(COMPILE) -c $< -o $@

# ������� (generic) ������� ��� ���������� ������ �� ����� ����������:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" �� ������ ���� ���������, ��������� ��� ������
#  � ����������� .S (����� ��������� S) ����� �� ���������. ������, ������� 
#  � ������� �������� �� ������ ����������� � ������ �� Windows. ����� ����������
#  ������������� � WinAVR ������� ��� ����� �������. 

# Generic ������� ��� �������������� ������ ����� C � ���������, ������������ ������ ��� �������.
.c.s:
	$(COMPILE) -S $< -o $@

usb-relay.elf: $(OBJECTS)
	$(COMPILE) -o usb-relay.elf $(OBJECTS)

usb-relay.hex: usb-relay.elf
	rm -f usb-relay.hex usb-relay.eep.hex
	avr-objcopy -j .text -j .data -O ihex usb-relay.elf usb-relay.hex
	avr-objcopy --debugging -O coff-ext-avr $< usb-relay.cof
	avr-size usb-relay.hex

# ���� �������:
disasm:	usb-relay.elf
	avr-objdump -d usb-relay.elf

cpp:
	$(COMPILE) -E usb-relay.c

# ���� ������� ��������� ����������������
erase:
	$(JTAGICEII) -e 

# ���� ������
backup:
	$(RAR) a -r -dh -ep1 avrusb.rar ../../../../avrusb-20080513
	mv avrusb.rar c:\!MyDoc\FLOPPI\avr\usb\obdev.at
	autoname /pattern:YYMMDDhhmmss c:/!MyDoc/FLOPPI/avr/usb/obdev.at/avrusb.rar
	