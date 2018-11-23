21 октября 2011
   В этом пакете собраны вместе проект оригинальный проект usb-Relay (http://johannes.krude.de/projects/usb-Relay/) и его порты на макетные платы AVR-USB-TINY45 и AVR-USB-MEGA16.
   
   Назначение файлов и папок:
   
commandline - папка, где находится исходный код и скомпилированные бинарники (для Windows) утилит ПО хоста cont.exe и set.exe.
common - папка, содержащие общие и для firmware, и для ПО хоста файлы заголовков *.h.
doc - папка с документацией
doc\110117usb-relay.zip - архив оригинального проекта usb-Relay, в котором находится исходный код firmware и ПО хоста, а также принципиальная схема и разводка печатной платы usb-Relay.
firmware-mega16 - исходный код порта usb-relay для макетной платы AVR-USB-MEGA16.
firmware-mega16\hex - скомпилированные прошивки под все варианты микроконтроллеров (ATmega16, ATmega32) и частот кварцев (12, 15, 16, 16.5, 20 МГц), используемые на макетной плате AVR-USB-MEGA16.
firmware-tiny45 - исходный код порта usb-relay для макетной платы AVR-USB-TINY45.
firmware-tiny45\hex - скомпилированные прошивки под все варианты микроконтроллеров (ATtiny45, ATtiny85), используемых на макетной плате AVR-USB-TINY45. Кварцевый резонатор не нужен, используется внутренняя тактовая частота 16.5 МГц, подстраиваемая по сигналу SOF шины USB.
include, lib - каталоги библиотеки libusb. Если у Вас не компилируется set.exe из-за отсутствия библиотек и подключаемых файлов в папке c:\Program Files\LibUSB-Win32\, то скопируйте папки include и lib в папку c:\Program Files\LibUSB-Win32\ (у Вас должна быть установлена библиотека libusb).
qt - GUI-приложение, управляющее реле номер 0 (IDE разработки Qt Creator от Nokia).
qt-build-desktop - скомпилированные бинарники GUI-приложения для Windows.

------------------------------------------------------------------------
26 октября 2011
   Чтобы избавить программу GUI (Qt) от зависимостей QtCore4.dll и QtGui4.dll перекомпилировал библиотеки Qt статически (см. "Building Qt Static (and Dynamic) and Making it Small with GCC, Microsoft Visual Studio, and the Intel Compiler" http://www.formortals.com/build-qt-static-small-microsoft-intel-gcc-compiler/). Кроме того, чтобы убрать зависимость от C:\MinGW\bin\libgcc_s_dw2-1.dll, добавил в файл qt\gt.pro строку:
QMAKE_LFLAGS += -static-libgcc -static-libstdc++

В разборке с зависимостями очень помогла программа Dependency Walker 2.2 http://www.dependencywalker.com/.

Осталась зависимость только от библиотеки libusb.



thanks bro! I fixed it by running this command: " sudo apt-get install libusb -1.0-0-dev libusb-dev".