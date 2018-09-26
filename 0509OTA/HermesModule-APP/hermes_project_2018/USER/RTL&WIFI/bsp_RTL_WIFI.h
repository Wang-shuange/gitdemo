#ifndef  __bsp_RTL_WIFI_H
#define	 __bsp_RTL_WIFI_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdbool.h>

#define uchar unsigned char
extern char user_input[2048];
extern char cmd[400];
/******************************** RTL_WIFI 连接引脚定义 ***********************************/

unsigned int wifi_send_command(char *Command, char *Response, unsigned long Timeout,uchar  Retry);
unsigned int wifi_send_exit(char *Command);
unsigned int wifi_send_commandx(char *Command, char *Response, unsigned long Timeout,uchar  Retry);
void wifi_send_data(char* data);
void wifi_disconnect(void);
void wifi_connect(void);
void wifi_init(char* ssid,char* pwd);
void RTL_WIFI_Init(void);
void SmartConfig(void);
void RTL_WIFI_Reset(void);
void Mac_IP_DisPlay(void);
unsigned int wifi_send_commandxx(char *Command, char *Response, unsigned long Timeout,uchar  Retry);

#endif


