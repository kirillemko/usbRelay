22 октября 2011
   Это порт оригинального usb-relay firmware на микроконтроллер ATmega16 (или ATmega32), рассчитанный на макетную плату AVR-USB-MEGA16.
   
   Компилировать так:
make clean
make hex

   Для компиляции под Windows нужен avr-GCC (пакет WinAVR), см. "Разработка устройства USB - как начать работу с библиотеками AVR USB (V-USB) и libusb." http://microsin.ru/content/view/613/44/.
   
   Кроме того, можно загрузить в AVR Studio 4.14 проект usb-relay.aps, компилировать его оттуда и отлаживать проект по шагам с помощью JTAG-отладчика JTAGICE mkII. В целях отладки оптимизация в проекте AVR Studio отключена (-O0).

   Скомпилированные бинарники находятся в папке hex.

   Если программы (count.exe, set.exe) не видят USB-устройства (но оно видно в Диспетчере Устройств) "Could not find USB device "usb-Relay" with vid=0x16c0 pid=0x5df", то установите драйвер фильтра на устройство (Пуск -> LibUSB-Win32 -> Filter Wizard).