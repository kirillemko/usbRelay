/* Этот заголовочный файл является общим между firmware и ПО хоста. Он задает
 *  номера запросов USB numbers (и опционально типы данных), используемые для
 *  коммуникации между хостом и устройством USB.
 */

#ifndef __REQUESTS_H_INCLUDED__
#define __REQUESTS_H_INCLUDED__

#define REQUESTTYPE_D2H         0x80

#define REQUEST_DEVICE_COUNT 0
#define REQUEST_SET_RELAYS   1

#endif /* __REQUESTS_H_INCLUDED__ */
