# ���: Makefile
# ������: ������ hid-custom-rq
# �����: 
# �������: microsin.ru 
# ���� ��������: 2011-10-21

# ��������������� ��������� ����������� � ������������ � ����� ��������.
# ���� Makefile ������������� �� Mac OS X, Linux � Windows.

# ����������� ��������� 3 ������ �� Unix (����������������� ��� �������� ��������� Mac OS X):
USBFLAGS = `libusb-config --cflags`
USBLIBS = `libusb-config --libs`
EXE_SUFFIX =

# ����������� ��������� 3 ������ �� Windows � ��������������� 3 ����������. �� ������ 
#  �������� ���� include ��� ������������ ����� ��������� libusb-win32
#USBFLAGS = -I/usr/local/include
#USBFLAGS = -I"c:/Program Files/LibUSB-Win32/include"
#USBLIBS = -L/usr/local/lib -lusb
#USBLIBS = -L"c:/Program Files/LibUSB-Win32/lib/gcc" -lusb
#EXE_SUFFIX = .exe 

DO= set count

all: $(DO)

OBJECTS = obdev_usb.o usbrelay.o

CC		= gcc
CFLAGS	= $(CPPFLAGS) $(USBFLAGS) -O -g -Wall
LIBS	= $(USBLIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

set: $(OBJECTS) set.o
	$(CC) -o set$(EXE_SUFFIX) $(OBJECTS) set.o $(LIBS)

count: $(OBJECTS) count.o
	$(CC) -o count$(EXE_SUFFIX) $(OBJECTS) count.o $(LIBS)

strip: $(DO)
	strip $(DO)

clean:
	rm -f *.o set$(EXE_SUFFIX) count$(EXE_SUFFIX)
