#ifndef  __BT_H
#define	 __BT_H
#define uchar unsigned char
	
unsigned int wifi_send_command_BT(char *Command, char *Response, unsigned long Timeout,uchar  Retry);
void BT_Init (void);
void BT_Reset(void);
void BT_GPIO_Config(void);
#endif


