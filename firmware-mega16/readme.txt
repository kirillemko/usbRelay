22 ������� 2011
   ��� ���� ������������� usb-relay firmware �� ��������������� ATmega16 (��� ATmega32), ������������ �� �������� ����� AVR-USB-MEGA16.
   
   ������������� ���:
make clean
make hex

   ��� ���������� ��� Windows ����� avr-GCC (����� WinAVR), ��. "���������� ���������� USB - ��� ������ ������ � ������������ AVR USB (V-USB) � libusb." http://microsin.ru/content/view/613/44/.
   
   ����� ����, ����� ��������� � AVR Studio 4.14 ������ usb-relay.aps, ������������� ��� ������ � ���������� ������ �� ����� � ������� JTAG-��������� JTAGICE mkII. � ����� ������� ����������� � ������� AVR Studio ��������� (-O0).

   ���������������� ��������� ��������� � ����� hex.

   ���� ��������� (count.exe, set.exe) �� ����� USB-���������� (�� ��� ����� � ���������� ���������) "Could not find USB device "usb-Relay" with vid=0x16c0 pid=0x5df", �� ���������� ������� ������� �� ���������� (���� -> LibUSB-Win32 -> Filter Wizard).