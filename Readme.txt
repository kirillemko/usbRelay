21 ������� 2011
   � ���� ������ ������� ������ ������ ������������ ������ usb-Relay (http://johannes.krude.de/projects/usb-Relay/) � ��� ����� �� �������� ����� AVR-USB-TINY45 � AVR-USB-MEGA16.
   
   ���������� ������ � �����:
   
commandline - �����, ��� ��������� �������� ��� � ���������������� ��������� (��� Windows) ������ �� ����� cont.exe � set.exe.
common - �����, ���������� ����� � ��� firmware, � ��� �� ����� ����� ���������� *.h.
doc - ����� � �������������
doc\110117usb-relay.zip - ����� ������������� ������� usb-Relay, � ������� ��������� �������� ��� firmware � �� �����, � ����� �������������� ����� � �������� �������� ����� usb-Relay.
firmware-mega16 - �������� ��� ����� usb-relay ��� �������� ����� AVR-USB-MEGA16.
firmware-mega16\hex - ���������������� �������� ��� ��� �������� ����������������� (ATmega16, ATmega32) � ������ ������� (12, 15, 16, 16.5, 20 ���), ������������ �� �������� ����� AVR-USB-MEGA16.
firmware-tiny45 - �������� ��� ����� usb-relay ��� �������� ����� AVR-USB-TINY45.
firmware-tiny45\hex - ���������������� �������� ��� ��� �������� ����������������� (ATtiny45, ATtiny85), ������������ �� �������� ����� AVR-USB-TINY45. ��������� ��������� �� �����, ������������ ���������� �������� ������� 16.5 ���, �������������� �� ������� SOF ���� USB.
include, lib - �������� ���������� libusb. ���� � ��� �� ������������� set.exe ��-�� ���������� ��������� � ������������ ������ � ����� c:\Program Files\LibUSB-Win32\, �� ���������� ����� include � lib � ����� c:\Program Files\LibUSB-Win32\ (� ��� ������ ���� ����������� ���������� libusb).
qt - GUI-����������, ����������� ���� ����� 0 (IDE ���������� Qt Creator �� Nokia).
qt-build-desktop - ���������������� ��������� GUI-���������� ��� Windows.

------------------------------------------------------------------------
26 ������� 2011
   ����� �������� ��������� GUI (Qt) �� ������������ QtCore4.dll � QtGui4.dll ���������������� ���������� Qt ���������� (��. "Building Qt Static (and Dynamic) and Making it Small with GCC, Microsoft Visual Studio, and the Intel Compiler" http://www.formortals.com/build-qt-static-small-microsoft-intel-gcc-compiler/). ����� ����, ����� ������ ����������� �� C:\MinGW\bin\libgcc_s_dw2-1.dll, ������� � ���� qt\gt.pro ������:
QMAKE_LFLAGS += -static-libgcc -static-libstdc++

� �������� � ������������� ����� ������� ��������� Dependency Walker 2.2 http://www.dependencywalker.com/.

�������� ����������� ������ �� ���������� libusb.



thanks bro! I fixed it by running this command: " sudo apt-get install libusb -1.0-0-dev libusb-dev".