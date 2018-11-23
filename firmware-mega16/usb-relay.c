/* Имя: main.c
 * Проект: usb-relay firmware (порт на ATmega16 или ATmega32, для макетной платы AVR-USB-MEGA16) microsin.ru
 * Дата создания: 2011-10-22
 */

/*
Этот пример должен работать на большинстве AVR с минимальными изменениями. Не используются
никакие аппаратные ресурсы микроконтроллера, за исключением INT0. Вы можете поменять 
usbconfig.h для использования других ножек I/O USB. Пожалуйста помните, что USB D+ должен
быть подсоединен на ножку INT0, или также как минимум быть соединенным с INT0.
Мы предполагаем, что LED подсоединен к порту B, бит 0. Если Вы подсоединили его на
другой порт или бит, поменяйте макроопределение ниже:
*/
#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define LED_PORT_INPUT      PINB
#define LED_BIT             PB0

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>  /* для sei() */
#include <util/delay.h>     /* для _delay_ms() */
#include "../common/requests.h"

#include <avr/pgmspace.h>   /* нужен для usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* Это также пример использования макроса отладки */

//отладочный макрос
#define LED_CHANGE() ( (LED_PORT_INPUT & (1<<LED_BIT))?(LED_PORT_OUTPUT &= ~(1<<LED_BIT)):(LED_PORT_OUTPUT |= (1<<LED_BIT)) )

/* ------------------------------------------------------------------------- */
/* ----------------------------- интерфейс USB ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[22] = {    /* дескриптор репорта USB */
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
/* Дескриптор выше - только макет, это заглушает драйверы. Репорт, который его 
 *  описывает, состоит из одного байта неопределенных данных. Мы не передаем
 *  наши данные через HID-репорты, вместо этого мы используем custom-запросы.
 */

static const uchar maskTable [23][4] =
{
   //    A        B         C        D
   {     0,   1<<LED_BIT,   0,       0},       //LED
   {1<<PA0,       0,        0,       0},       //P1
   {1<<PA1,       0,        0,       0},       //P2
   {1<<PA2,       0,        0,       0},       //P3
   {1<<PA3,       0,        0,       0},       //P4
   {1<<PA4,       0,        0,       0},       //P5
   {1<<PA5,       0,        0,       0},       //P6
   {1<<PA6,       0,        0,       0},       //P7
   {1<<PA7,       0,        0,       0},       //P8
   {     0,  1<<PB1,        0,       0},       //P9
   {     0,  1<<PB2,        0,       0},       //P10
   {     0,  1<<PB3,        0,       0},       //P11
   {     0,  1<<PB4,        0,       0},       //P12
   {     0,       0,   1<<PC0,       0},       //P13
   {     0,       0,   1<<PC1,       0},       //P14
   {     0,       0,   1<<PC6,       0},       //P15
   {     0,       0,   1<<PC7,       0},       //P16
   {     0,       0,        0,  1<<PD0},       //P17
   {     0,       0,        0,  1<<PD1},       //P18
   {     0,       0,        0,  1<<PD3},       //P19
   {     0,       0,        0,  1<<PD5},       //P20
   {     0,       0,        0,  1<<PD6},       //P21
   {     0,       0,        0,  1<<PD7}        //P22
};

/* ------------------------------------------------------------------------- */
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
   usbRequest_t    *rq = (void *)data;
   uchar maskA, maskB, maskC, maskD;

   if (REQUEST_DEVICE_COUNT == rq->bRequest)
   {
      if (((rq->bmRequestType & REQUESTTYPE_D2H) != 0) && (rq->wLength.word == 1))
      {
         static uchar dataBuffer[1];     /* буфер должен оставаться валидным при выходе из usbFunctionSetup */
         dataBuffer[0] = 23;              //поддержмваем 23 реле
         usbMsgPtr = dataBuffer;         /* говорим драйверу, какие данные вернуть */
         return 1;                      /* говорим драйверу послать 1 байт */
      }
   }
   else if (REQUEST_SET_RELAYS == rq->bRequest)
   {
      if (rq->wValue.bytes[1] < 23)
      {
         maskA = maskTable[rq->wValue.bytes[1]][0];
         maskB = maskTable[rq->wValue.bytes[1]][1];
         maskC = maskTable[rq->wValue.bytes[1]][2];
         maskD = maskTable[rq->wValue.bytes[1]][3];
         if (0==rq->wValue.bytes[0])
         {
            PORTA &= ~maskA;
            PORTB &= ~maskB;
            PORTC &= ~maskC;
            PORTD &= ~maskD;
         }
         else
         {
            PORTA |= maskA;
            PORTB |= maskB;
            PORTC |= maskC;
            PORTD |= maskD;
         }
      }
   }
   return 0;   /* default для нереализованных запросов: не возвращаем назад данные хосту */
}

/* ------------------------------------------------------------------------- */
int main(void)
{
   uchar   i;

   wdt_enable(WDTO_1S);
   /* Даже если Вы не используете сторожевой таймер (watchdog), выключите его здесь. На более новых 
    *  микроконтроллерах состояние watchdog (вкл\выкл, период) СОХРАНЯЕТСЯ ЧЕРЕЗ СБРОС!
    */
   /* RESET статус: все биты портов являются входами без нагрузочных резисторов (pull-up).
    *  Это нужно для D+ and D-. Таким образом, нам не нужна какая-либо дополнительная 
    *  инициализация портов.
    */
   //odDebugInit();
   usbInit();
   usbDeviceDisconnect();  /* принудительно запускаем ре-энумерацию, делайте это, когда прерывания запрещены! */
   i = 0;
   while(--i)
   {             /* иммитируем USB дисконнект на время > 250 мс */
      wdt_reset();
      _delay_ms(1);
   }
   usbDeviceConnect();
   // делаем ножку, куда подключен LED, выходом
   LED_PORT_DDR |= _BV(LED_BIT);
   // делаем ножки P1..P22 выходами
   DDRA  = 0xFF;                                                   //P1..P8
   DDRB |= (1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4);                    //P9..P12
   DDRC |= (1<<PC0)|(1<<PC1)|(1<<PC6)|(1<<PC7);                    //P13..P16
   DDRD |= (1<<PD0)|(1<<PD1)|(1<<PD3)|(1<<PD5)|(1<<PD6)|(1<<PD7);  //P17..P22
   sei();
   for(;;)
   {  // цикл событий main
      wdt_reset();
      usbPoll();
   }
   return 0;
}

/* ------------------------------------------------------------------------- */
