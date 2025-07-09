#ifndef ESP8266_H_
#define ESP8266_H_

#include <stdio.h>



void init_esp8266(void);
void connect_udp(void);
void send_voltage(uint32_t voltage,int mode=0);


#endif
