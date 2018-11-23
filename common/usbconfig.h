/* ���: usbconfig.h
 * ������: ������� AVR USB	
 * �����: Christian Starkjohann
 * �������: microsin.ru 
 * ���� ��������: 2005-04-01
 * ���������: 4
 * Copyright: (c) 2005 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
 * �������: $Id: usbconfig-prototype.h 600 2008-05-13 10:34:56Z cs $
 */

#ifndef __usbconfig_h_included__
#define __usbconfig_h_included__

/*
�������� ��������:

���� ���� �������� �������� ������������ (�� ���������� �������������) ��� �������� USB. 
 �� ������������ AVR-USB ��� ������� USB D+, ��������������� � ����� D ��� 2 (������� �����
 �������� ������ ����������� ���������� 0 [INT0] �� ������ �����) � ������� USB D- � ����� D ��� 4. 
 �� ������ ������������ ��� ������� �� ����� ������ ����, ������ D+ ������ ���� ����� ��������� �� 
 ���� ����������� ���������� INT0, ������� ����� ��������� ���������, ��. ������ � ����� �����).
*/

/* ---------------------------- Hardware Config ---------------------------- */

#if ( (defined (__AVR_ATmega16__)) || (defined (__AVR_ATmega32__)) )
  #define USB_CFG_IOPORTNAME      D
#elif ( (defined (__AVR_ATtiny45__)) || (defined (__AVR_ATtiny85__)) )
  #define USB_CFG_IOPORTNAME      B
#endif
/* ������ ����, � �������� ���������� ���� USB. ���� �� ��������������� "B", 
    ����� �������������� �������� PORTB, PINB � DDRB.
 */
#if ( (defined (__AVR_ATmega16__)) || (defined (__AVR_ATmega32__)) )
  #define USB_CFG_DMINUS_BIT      4
#elif ( (defined (__AVR_ATtiny45__)) || (defined (__AVR_ATtiny85__)) )
  #define USB_CFG_DMINUS_BIT      0
#endif
/* ��� ����� ���� � USB_CFG_IOPORT, ���� ��������� ������ D-, ����� �������������� 
    ����� ��� � �����.
 */
#if ( (defined (__AVR_ATmega16__)) || (defined (__AVR_ATmega32__)) )
  #define USB_CFG_DPLUS_BIT       2
#elif ( (defined (__AVR_ATtiny45__)) || (defined (__AVR_ATtiny85__)) )
  #define USB_CFG_DPLUS_BIT       2
#endif
/* ��� ����� ���� � USB_CFG_IOPORTNAME, ���� ��������� ������ D+, ����� �������������� 
    ����� ��� � �����. ����������, ������� �� ��������, ��� D+ ������ ���� ����� �����������
    � ����� ���������� INT0! [�� ������ ����� ������������ ������ ����������, ��. ������
    "Optional MCU Description" �����, ��� �� ������ ������������ D- � ����������, ��� ��� 
    ���������� ���� �� ����������� ����� USB_COUNT_SOF. ���� �� ����������� D- ��� ����������, 
    ���������� USB ����� ����������� ����� �� �������� Start-Of-Frame ������ �����������.]
 */
#define USB_CFG_CLOCK_KHZ       (F_CPU/1000)
/* �������� ������� ���� AVR � ����������. ���������� �������� 12000, 15000, 16000, 16500
    � 20000. ������ ���� 16.5 ��� �� ������� ���������, ��� ��������� ���� ������� 
    +/- 1% �� �����������. ��� ������ ������� ������� �������� 2000 ppm, � � ���� ������
    ����������� ����� ��������!
   ���� �� �������, �� �� ���������: 12 ���
 */

/* ----------------------- �������������� Hardware Config ------------------------ */

/* #define USB_CFG_PULLUP_IOPORTNAME   D */
/* ���� �� ������������ 1.5k pullup �������� �� D- � ����� ����� ������ V+, 
    �� ������ ������������ � ����������� ���������� �� firmware ����� ������
    ������� usbDeviceConnect() � usbDeviceDisconnect() (��. usbdrv.h).
    ��� ��������� ������ ����, � �������� ��������� pullup ��������.
 */
/* #define USB_CFG_PULLUP_BIT          4 */
/* ��� ��������� ���������� ����� ���� ����� � USB_CFG_PULLUP_IOPORTNAME (��������
    �����) ���� ��������� 1.5k pullup ��������. ��. ��������, ��������� �����.
 */

/* -------------- Functional Range (�������������� ��������) --------------- */

#define USB_CFG_HAVE_INTRIN_ENDPOINT    0
/* ������� ����� 1, ���� �� ������ �������������� ������ � ����� ��������� ������� 
    (endpoints): default control endpoint 0 � interrupt-in endpoint (����� ������ 
    endpoint �����).
 */
#define USB_CFG_HAVE_INTRIN_ENDPOINT3   0
/* ������� ����� 1, ���� �� ������ �������������� ������ � ����� ��������� ������� 
    (endpoints): default control endpoint 0 � interrupt-in endpoint 3 (��� �����,
    ������������������ �����) � catch-all default interrupt-in endpoint ��� �������
    �����. �� ������ ����� ������ USB_CFG_HAVE_INTRIN_ENDPOINT � 1 ��� ���� �����������.
 */
#define USB_CFG_EP3_NUMBER              3
/* If the so-called endpoint 3 is used, it can now be configured to any other
 * endpoint number (except 0) with this macro. Default if undefined is 3.
 */
/* #define USB_INITIAL_DATATOKEN           USBPID_DATA1 */
/* The above macro defines the startup condition for data toggling on the
 * interrupt/bulk endpoints 1 and 3. Defaults to USBPID_DATA1.
 * Since the token is toggled BEFORE sending any data, the first packet is
 * sent with the oposite value of this configuration!
 */
#define USB_CFG_IMPLEMENT_HALT          0
/* ������� ����� 1, ���� �� ������ ����� ����������� ����������� ENDPOINT_HALT
    ��� endpoint 1 (interrupt endpoint). �������� �� ��, ��� ��� ��� ����������� ����� �� ������������,
    ��� ��������� �� ���������. �� ����� ��� ����� � ������������ ������ ��� 
    ��� ������� ����������� ��������� ���.
 */
#define USB_CFG_INTR_POLL_INTERVAL      10
/* ���� �� �������������� ������ � endpoint 1 (interrupt-in), �� ����� ��������
    �������� ������ (poll interval). �������� ������� � ������������ � �� ������ ���� ������ 10 ms
    ��� ��������������� ���������.
 */
#define USB_CFG_IS_SELF_POWERED         0
/* ������� ����� 1 ���� ���������� ����� ����������� �������� �������. ��������� � 0 �������������
    ����������, ����������� �� ���� USB.
 */
#define USB_CFG_MAX_BUS_POWER           50
/* ���������� ��� ���������� �� max �������� ����������� �������� ������ ���������� �� ���� USB.
    �������� ������� � �����������. [��� ����� �������� ������������ �� 2 ��������� ������������ 
    ��������� ����������������� USB ��������� ��������� � �������� 2 ��.]
 */
#define USB_CFG_IMPLEMENT_FN_WRITE      0
/* ������� ����� 1, ���� �� ������, ����� ������� usbFunctionWrite() ���������� ��� ������� control-out. 
    ������� 0 ���� ��� ��� �� �����, ��� �������� ���������� ��������� ����.
 */
#define USB_CFG_IMPLEMENT_FN_READ       0
/* ������� 1 ���� ��� ����� ���������� ����������� ������� ������, ������� ������������ 
    "�� ����" ��� ������ usbFunctionRead(). ���� ��� ����� ������ ��������� ������
    �� ������������ ������, ������� 0 � ����������� ������ �� usbFunctionSetup(). 
    ��� ��������� ��������� ���������� ����.
 */
#define USB_CFG_IMPLEMENT_FN_WRITEOUT   0
/* ������� 1 ���� �� ������ ������������ interrupt-out (��� bulk out) endpoints.
    �� ������ ����������� ������� usbFunctionWriteOut(), ������� ��������� ���
    interrupt/bulk ������, ������������ � ����� endpoint, �������� �� 0. ����� endpoint
    ����� ����� � 'usbRxToken'.
 */
#define USB_CFG_HAVE_FLOWCONTROL        0
/* ������� 1, ���� �� ������ ����������� ���������� ������� (flowcontrol) ����� ������ USB. 
    ��. �������� ������� usbDisableAllRequests() � usbEnableAllRequests() � usbdrv.h.
 */
#define USB_CFG_LONG_TRANSFERS          0
/* Define this to 1 if you want to send/receive blocks of more than 254 bytes
 * in a single control-in or control-out transfer. Note that the capability
 * for long transfers increases the driver size.
 */

#if ( (defined (__AVR_ATtiny45__)) || (defined (__AVR_ATtiny85__)) )
  #ifndef __ASSEMBLER__
    extern void usbEventResetReady(void);
  #endif
#endif

/* #define USB_RX_USER_HOOK(data, len)     if(usbRxToken == (uchar)USBPID_SETUP) blinkLED(); */
/* This macro is a hook if you want to do unconventional things. If it is
 * defined, it's inserted at the beginning of received message processing.
 * If you eat the received message and don't want default processing to
 * proceed, do a return after doing your things. One possible application
 * (besides debugging) is to flash a status LED on each packet.
 */
/* #define USB_RESET_HOOK(resetStarts)     if(!resetStarts){hadUsbReset();} */
#if ( (defined (__AVR_ATtiny45__)) || (defined (__AVR_ATtiny85__)) )
  #define USB_RESET_HOOK(isReset)             if(!isReset){usbEventResetReady();}
#endif
/* ���� ������ �������� �����, ���� ��� ����� �����, ����� ��������� USB RESET. �� �����
 *  ���� ��������, ������� ��������� �������� ������ � ����� ��������� RESET.
 */
/* #define USB_SET_ADDRESS_HOOK()              hadAddressAssigned(); */
/* This macro (if defined) is executed when a USB SET_ADDRESS request was
 * received.
 */
#define USB_COUNT_SOF                   0
/* define this macro to 1 if you need the global variable "usbSofCount" which
 * counts SOF packets. This feature requires that the hardware interrupt is
 * connected to D- instead of D+.
 */
#if ( (defined (__AVR_ATmega16__)) || (defined (__AVR_ATmega32__)) )
  #define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   0
#elif ( (defined (__AVR_ATtiny45__)) || (defined (__AVR_ATtiny85__)) )
  #define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   1
#endif
/* ������� ����� 1, ���� ��� ����� ������� usbMeasureFrameLength() - ����� ��� ��������������.
    ��� ������� ����� �������������� ��� ���������� ���������� RC ���������������� AVR.
 */

/* ---------------- ���������, ����������� USB-���������� ------------------ */

#define  USB_CFG_VENDOR_ID       0xc0, 0x16
/* USB ������ ID ��� ����������, ������� ���� ���� ������. ���� �� ���������������� ���� 
    ����������� Vendor ID, ������� ��� �����. � ��������� ������ ����������� ���� �� 
    �������� ��������������� ��� VID/PID obdev. ���������� ������� USBID-License.txt!
 */
#define  USB_CFG_DEVICE_ID       0xdf, 0x05 /* ��������������� � ����� ����������� obdev PID ��� ��������� HID */
/* ��� ID ��������, ������� ���� ���� ������. �� ���������������� � ��������� 
    vendor ID. ���� �� ���������������� ���� ����������� VID �� usb.org,
    ��� ���� ����������� ��������� �� PID �� ����-������ ���, ������� ��� �����. 
    ����� ����������� ��������������� obdev �������� ���������������� ���� VID/PID.
    ���������� ������� USBID-License.txt!
 */
#define USB_CFG_DEVICE_VERSION  0x00, 0x01
/* ����� ������ ����������: ������� ������� �����, ����� ������� �����.
 */
#define USB_CFG_VENDOR_NAME     'j', 'o', 'h', 'a', 'n', 'n', 'e', 's', '.', 'k', 'r', 'u', 'd', 'e', '.', 'd', 'e'
#define USB_CFG_VENDOR_NAME_LEN 17
/* ��� ��� �������� ������ ��� ������� (vendor name), ������������ �����������. 
    ��� ������ ��������������� ��� ������ �������� � ��������� ��������. 
    ������� ���������������� ��� Unicode (UTF-16). ���� ��� �� ����� ��� �������,
    ��������������� ���� ������. ������ ���������� ��� �������, ���������� ���� 
    �������� ��� Internet, ���� �� ����������� �������� ���������������� ���� 
    obdev VID/PID. �� �������� ����������� � ����� USBID-License.txt.
 */
#define USB_CFG_DEVICE_NAME     'u', 's', 'b', '-', 'R', 'e', 'l', 'a', 'y'
#define USB_CFG_DEVICE_NAME_LEN 9
/* ����� ����������� ��� ���������� (device name) ����� �� ��������, ��� � � ���������� 
    ��������� ����������� ��� �������. ���� ��� �� ����� ��� ����������, ���������������
    ���� ������. ��. ���� USBID-License.txt ����� ����������� �����, ���� �� ����������� 
    �������� ���������������� ���� obdev VID/PID.
 */
/*#define USB_CFG_SERIAL_NUMBER   'N', 'o', 'n', 'e' */
/*#define USB_CFG_SERIAL_NUMBER_LEN   0 */
/* �������� ����� (serial number). ���� �������� ����� ��� �� �����, ��������������� ������.
    ����� ��������� �������� ������������ serial number �� ������ ������������, ������������ 
    �� ������� ����������. ��. ������, ����������� �������� ����������� �����, ��� ������� 
    ��� ������� ������ ���������� ������������� USB (USB descriptors), ������ ��� ���������
    ��������� ��� ��������� ������.
 */
#define USB_CFG_DEVICE_CLASS        0       /* ������� 0, ���� �������� ��� ���������� */
#define USB_CFG_DEVICE_SUBCLASS     0
/* ��. ������������ USB, ���� �� ������ ������������ ���������� � ������������� ������ ��������� 
    (device class). ����� 0xff �������� "������������� ��� �������" ("vendor specific").
 */
#define USB_CFG_INTERFACE_CLASS     3   /* HID */
#define USB_CFG_INTERFACE_SUBCLASS  0   /* no boot interface */
#define USB_CFG_INTERFACE_PROTOCOL  0   /* no protocol */
/* ��. ������������ USB, ���� �� ������, ����� ���������� �������� ����������� ������������� ������
    ��� ���������. ��������� ������ ������ ���� ����������� �� ������ ����������:
 * HID class 3, �� ��������� ���������� � ���������� (����� ��������� �������!)
 * CDC class 2, ����������� �������� 2 � �������� 1 ��� ACM
 */
#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    22
/* ������� ����� ����� HID report descriptor, ���� �� ����������� HID-����������. 
    ����� �� ��������� ���� �������� ��� ���������� ��� � 0.
   ���� ����������� ��� ����������, �� �� ������ �������� ���������� ������ PROGMEM, ���������
    "usbHidReportDescriptor", � ���, ������� �������� report descriptor. �� �������� �������
    ������ � ���� �������� � ������������!
 */

/* #define USB_PUBLIC static */
/* ����������� ���� define, ���� �� ����������� ��������� #include usbdrv.c ������ ����������.
    ��� ������� �������� ��������� ���� flash-������.
 */

/* ------------------- ������ ���������� ����� ����������� USB ------------------- */
/* ���� �� �� ������ ������������ ����������� �� ��������� �������� USB, �� ������
    ������������ ���� �����������. ��� ����� ���� ������� ��� (1) ����������� ������ 
    ������������� ����� � ������ flash, (2) ����������� ������ ������������� ����� � RAM, 
    ��� (3) ������������ �� ����� ���������� � ������� usbFunctionDescriptor(). 
    ��. usbdrv.h ��� ������� ���������� �� ����� ������������.
   ��������� ������������ ��������������� ����� �������� �����������. ���� �� ������ 
    �������� ��� ���� ��� ����� 0, ������������ ���������� �� ���������.
   �������� ��������:
     + USB_PROP_IS_DYNAMIC: ������ ��� ����������� ������ ���� ��������� (fetched)
       �� ����� ���������� ����� usbFunctionDescriptor().
     + USB_PROP_IS_RAM: ������, ������������ usbFunctionDescriptor() ��� ��������� 
       � ����������� ������ RAM, �� � ������ flash.
     + USB_PROP_LENGTH(len): ���� ������ � ����������� ������ (RAM ��� flash),
       ������� ������ ����� ����� �����������. ���������� ��� �� ���� ������ �� ������,
       ���������� ��� ������������� (��. �����).
   ������ ����������� ���� ������������ (������ ���� �������������� PROGMEM � flash):
     char usbDescriptorDevice[];
     char usbDescriptorConfiguration[];
     char usbDescriptorHidReport[];
     char usbDescriptorString0[];
     int usbDescriptorStringVendor[];
     int usbDescriptorStringDevice[];
     int usbDescriptorStringSerialNumber[];
   ������ ����������� �� ����� ���� ������������� ����������, ��� ������ ��������������� 
   ����������� �� ����� ����������.
 
   �������� ������������ ������������ �� ��� ���� ������������ ������, ��������:
   #define USB_CFG_DESCR_PROPS_DEVICE   (USB_PROP_IS_RAM | USB_PROP_LENGTH(18))
 
   ��������� ����������� ������:
     USB_CFG_DESCR_PROPS_DEVICE
     USB_CFG_DESCR_PROPS_CONFIGURATION
     USB_CFG_DESCR_PROPS_STRINGS
     USB_CFG_DESCR_PROPS_STRING_0
     USB_CFG_DESCR_PROPS_STRING_VENDOR
     USB_CFG_DESCR_PROPS_STRING_PRODUCT
     USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER
     USB_CFG_DESCR_PROPS_HID
     USB_CFG_DESCR_PROPS_HID_REPORT
     USB_CFG_DESCR_PROPS_UNKNOWN (��� ���� ������������, �� �������������� ���������)
 */

#define USB_CFG_DESCR_PROPS_DEVICE                  0
#define USB_CFG_DESCR_PROPS_CONFIGURATION           0
#define USB_CFG_DESCR_PROPS_STRINGS                 0
#define USB_CFG_DESCR_PROPS_STRING_0                0
#define USB_CFG_DESCR_PROPS_STRING_VENDOR           0
#define USB_CFG_DESCR_PROPS_STRING_PRODUCT          0
#define USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER    0
#define USB_CFG_DESCR_PROPS_HID                     0
#define USB_CFG_DESCR_PROPS_HID_REPORT              0
#define USB_CFG_DESCR_PROPS_UNKNOWN                 0

/* ----------------------- �������������� �������� MCU ------------------------ */
/* ��������� ������������ �������� �� ��������� � usbdrv.h. ��� ������ �� �����
    ������������� ��. ������ ���� ��� ����� ��������� ������� �� ����������,
    ������� ���� �� �������������� ������������, ������� �� ��������� �������������� 
    (���, ��������, IAR C) ��� ���� ����������� ����������, �������� �� INT0, 
    �� ������ ������ ���-��� �� �����.
 */
/* #define USB_INTR_CFG            MCUCR */
/* #define USB_INTR_CFG_SET        ((1 << ISC00) | (1 << ISC01)) */
/* #define USB_INTR_CFG_CLR        0 */
/* #define USB_INTR_ENABLE         GIMSK */
/* #define USB_INTR_ENABLE_BIT     INT0 */
/* #define USB_INTR_PENDING        GIFR */
/* #define USB_INTR_PENDING_BIT    INTF0 */
/* #define USB_INTR_VECTOR         SIG_INTERRUPT0 */

#endif /* __usbconfig_h_included__ */
