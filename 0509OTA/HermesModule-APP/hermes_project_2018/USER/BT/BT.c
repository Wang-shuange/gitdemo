#include "BT.h"
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include <string.h>  
#include "delay.h"
#define uchar unsigned char
char user_input_BT[2048];
void UART5_IRQHandler(void)
{
  uchar ucTemp;
	if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)
	{		
		USART_ClearFlag(UART5,USART_FLAG_RXNE);
		ucTemp = USART_ReceiveData(UART5);
		strcat(user_input_BT,(const char*)&ucTemp);		
	}	 
}
/******************************************************************/
void BT_Init (void)  
{
  while(wifi_send_command_BT( "AT+SLEEP\r\n", "OK", 1000, 3 )){}
}
/******************************************************************/
unsigned int wifi_send_command_BT(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("--->>send command length:%d\r\n",strlen(Command));
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
					USART_SendData(UART5,Command[i]);
				  printf("%c",Command[i]);
					while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
			}
//      user_input_BT[1]=USART_ReceiveData(UART5);    
			delay_ms(Timeout);
			printf("xxx%s",user_input_BT);
			if(strstr(user_input_BT,Response))
			{   
				  printf("--->>send %s",Command);
				  printf("---<<set ok\r\n");
//				  printf("xxx<<<: %s\r\n",user_input);
					memset(user_input_BT,0,2048);
					return 0;
			}
					else
					continue;			 
	} 
      printf("--->>send :%s fail!\r\n",Command);
	    memset(user_input_BT,0,2048);
			return 1;
}


