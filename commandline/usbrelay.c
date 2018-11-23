/*
 * © Johannes Krude 2008
 *
 * This file is part of spi2usb.
 *
 * spi2usb is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * spi2usb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with spi2usb.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <usb.h>

#include "obdev_usb.h"
#include "usbrelay.h"
#include "../common/requests.h"   /* номера custom request */
#include "../common/usbconfig.h"  /* имена и VID/PID устройства */ 

#define USB_TIMEOUT 1000

static usb_dev_handle  *usb_handle = NULL;
static char str_error[128]= {0};

int usbrelay_init()
{
   const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID}; 
   char                  vendor[]  = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
   int vid, pid; 
   
   /* вычисляем VID/PID из usbconfig.h, так как это центральный источник информации */
   vid = rawVid[1] * 256 + rawVid[0];
   pid = rawPid[1] * 256 + rawPid[0];
   usb_init();
	if(usbOpenDevice(&usb_handle, vid, vendor, pid, product) != 0)
	{
		snprintf(str_error, sizeof(str_error), "Could not find USB device \"%s\" with vid=0x%x pid=0x%x", product, vid, pid);
		return(1);
	}
	else
      return 0;
}

char *usbrelay_strerror()
{
	return str_error;
}

static int usb_do(int direction, int request, char value, char *data, int len)
{
	int i;
	while (len > 254) {
		usb_do(direction, request, value, data, 254);
		data+= 254;
		len-= 254;
	}
	i= usb_control_msg(usb_handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | direction, request, value, 0, data, len, USB_TIMEOUT);
	if (i < 0) {
		snprintf(str_error, sizeof(str_error), "usb error: %s", usb_strerror());
		return i;
	}
	return 0;
}

int usbrelay_set(uchar number, uchar value)
{
   //printf("number %i value %i\n", number, value);
	//return usb_do(USB_ENDPOINT_OUT, REQUEST_SET, number*256+value, 0, 0);
	
	int i;

	i= usb_control_msg(usb_handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, REQUEST_SET, number*256+value, 0, NULL, 0, USB_TIMEOUT);
	if (i < 0) {
		snprintf(str_error, sizeof(str_error), "usb error: %s", usb_strerror());
		return i;
	}
	return 0;
}

uchar usbrelay_count()
{
	uchar result;
        usb_do(USB_ENDPOINT_IN, REQUEST_COUNT, 0, (char*)&result, 1);
	return result;
}

