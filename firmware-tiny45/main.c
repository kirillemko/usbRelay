/* ���: main.c
 * ������: usb-relay firmware (���� �� ATtiny45 ��� ATtiny85, ��� �������� ����� AVR-USB-TINY45) microsin.ru
 * ���� ��������: 2011-10-22
 */

/*
���� ������ ������ �������� �� ����������� AVR � ������������ �����������. �� ������������
������� ���������� ������� ����������������, �� ����������� INT0. �� ������ �������� 
usbconfig.h ��� ������������� ������ ����� I/O USB. ���������� �������, ��� USB D+ ������
���� ����������� �� ����� INT0, ��� ����� ��� ������� ���� ����������� � INT0.
�� ������������, ��� LED ����������� � ����� B, ��� 0. ���� �� ������������ ��� ��
������ ���� ��� ���, ��������� ���������������� ����:
*/
#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define LED_PORT_INPUT      PINB
#define LED_BIT             1

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>  /* ��� sei() */
#include <util/delay.h>     /* ��� _delay_ms() */
#include "requests.h"

#include <avr/pgmspace.h>   /* ����� ��� usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* ��� ����� ������ ������������� ������� ������� */

//���������� ������
#define LED_CHANGE() ( (LED_PORT_INPUT & (1<<LED_BIT))?(LED_PORT_OUTPUT &= ~(1<<LED_BIT)):(LED_PORT_OUTPUT |= (1<<LED_BIT)) )

/* ------------------------------------------------------------------------- */
/* ----------------------------- ��������� USB ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[22] = {    /* ���������� ������� USB */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};
/* ���������� ���� - ������ �����, ��� ��������� ��������. ������, ������� ��� 
 *  ���������, ������� �� ������ ����� �������������� ������. �� �� ��������
 *  ���� ������ ����� HID-�������, ������ ����� �� ���������� custom-�������.
 */

/* ------------------------------------------------------------------------- */
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t    *rq = (void *)data;

	switch (rq->bRequest)
	{
	case REQUEST_DEVICE_COUNT:
		if (((rq->bmRequestType & REQUESTTYPE_D2H) != 0) && (rq->wLength.word == 1))
		{
         static uchar dataBuffer[1];     /* ����� ������ ���������� �������� ��� ������ �� usbFunctionSetup */
         dataBuffer[0] = 1;
         usbMsgPtr = dataBuffer;         /* ������� ��������, ����� ������ ������� */
         return 1;                      /* ������� �������� ������� 1 ���� */
		}
		break;
	case REQUEST_SET_RELAYS:
		if (rq->wValue.bytes[1] != 0)
			return 0;
      if(rq->wValue.bytes[0] & 1)
      {  // ���������� LED
         LED_PORT_OUTPUT |= _BV(LED_BIT);
      }
      else
      {  // �������� LED
         LED_PORT_OUTPUT &= ~_BV(LED_BIT);
      }
		return 0;
	}
   return 0;   /* default ��� ��������������� ��������: �� ���������� ����� ������ ����� */
}

/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */

/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
static void calibrateOscillator(void)
{
uchar       step = 128;
uchar       trialValue = 0, optimumValue;
int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}

/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/

void    usbEventResetReady(void)
{
    calibrateOscillator();
    eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}


/* ------------------------------------------------------------------------- */

int main(void)
{
   uchar   i;
   uchar   calibrationValue;

   calibrationValue = eeprom_read_byte(0); /* calibration value from last time */
   if(calibrationValue != 0xff)
   {
      OSCCAL = calibrationValue;
   }
   wdt_enable(WDTO_1S);
   /* ���� ���� �� �� ����������� ���������� ������ (watchdog), ��������� ��� �����. �� ����� ����� 
    *  ����������������� ��������� watchdog (���\����, ������) ����������� ����� �����!
    */
   /* RESET ������: ��� ���� ������ �������� ������� ��� ����������� ���������� (pull-up).
    *  ��� ����� ��� D+ and D-. ����� �������, ��� �� ����� �����-���� �������������� 
    *  ������������� ������.
    */
   //odDebugInit();
   usbInit();
   usbDeviceDisconnect();  /* ������������� ��������� ��-����������, ������� ���, ����� ���������� ���������! */
   i = 0;
   while(--i)
   {             /* ���������� USB ���������� �� ����� > 250 �� */
      wdt_reset();
      _delay_ms(1);
   }
   usbDeviceConnect();
   LED_PORT_DDR |= _BV(LED_BIT);   /* ������ �����, ���� ��������� LED, ������� */
   sei();
   for(;;){                /* ���� ������� main */
      wdt_reset();
      usbPoll();
   }
   return 0;
}

/* ------------------------------------------------------------------------- */
