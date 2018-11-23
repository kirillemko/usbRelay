/* Имя: set-led.c
 * Проект: пример usb-relay-tiny45
 * Автор: 
 * Перевод: microsin.ru
 * Дата создания: 2011-10-21
 */

/*
Основное описание:
Это драйвер на стороне хоста для примера устройства custom-class. Он ищет на шине USB
устройство usb-relay и отправляет запросы, понимаемые и распознаваемые этим устройством.
Эта программа должна быть слинкована вместе с libusb на Unix и libusb-win32 на Windows.
См. http://libusb.sourceforge.net/ или http://libusb-win32.sourceforge.net/ соответственно.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>        /* это libusb */

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
