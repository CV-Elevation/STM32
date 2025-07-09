#include "esp8266.h"
#include "../timer/timer.h"



void init_esp8266(void)
{
	printf("AT+RESTORE\r\n");	//重启
	timer_delay_ms(2000);
	printf("AT+CWMODE=2\r\n");	//连接模式
	timer_delay_ms(50);
	printf("AT+CWSAP=\"S09_ESP8266\",\"12345678\",4,3\r\n");	//打开WIFI
	timer_delay_ms(50);
}

void connect_udp(void)
{
	printf("AT+CIPSTART=\"UDP\",\"192.168.4.3\",1234\r\n");
	timer_delay_ms(50);
}

void send_voltage(uint32_t voltage,int mode)
{
	char message[14]={0};
	if(mode==0)
	{
		sprintf(message,"V%010d\r\n",voltage);
	}
	else
	{
		sprintf(message,"Z%010d\r\n",voltage);
	}
	printf("AT+CIPSEND=11\r\n");
	timer_delay_ms(3);
	printf(message);
	timer_delay_ms(3);
}

