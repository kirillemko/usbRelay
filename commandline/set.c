/* ���: set-led.c
 * ������: ������ usb-relay-tiny45
 * �����: 
 * �������: microsin.ru
 * ���� ��������: 2011-10-21
 */

/*
�������� ��������:
��� ������� �� ������� ����� ��� ������� ���������� custom-class. �� ���� �� ���� USB
���������� usb-relay � ���������� �������, ���������� � �������������� ���� �����������.
��� ��������� ������ ���� ���������� ������ � libusb �� Unix � libusb-win32 �� Windows.
��. http://libusb.sourceforge.net/ ��� http://libusb-win32.sourceforge.net/ ��������������.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>        /* ��� libusb */

#include "usbrelay.h"

static void usage()
{
	printf("set usb-Relay\n");
	printf("usage *nix: ./set [relay_number] [relay_on]\n");
	printf("usage Windows: set.exe [relay_number] [relay_on]\n");
	exit(1);
}

int main(int argc, char **argv)
{
   unsigned char relay_adr, relay_val;

	if (argc != 3)
		usage();
	if (strlen(argv[1]) > 2)
		usage();
	if (strlen(argv[2]) != 1)
		usage();
	
	if (1 != sscanf(argv[1], "%i", &relay_adr))
      usage();
   if (relay_adr>22)
   {
      printf ("22 is max relay address\n");
      usage();
   }
	if ((argv[2][0] < '0') || (argv[2][0] > '1'))
		usage();
	
	relay_val = argv[2][0] - '0';
	if (usbrelay_init()!=0) {
		printf("%s\n", usbrelay_strerror());
		return 1;
	}
	
	usbrelay_set(relay_adr, relay_val);
	return 0;
}
