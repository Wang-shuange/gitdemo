#include "BT.h"
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "utility.h"
#include <stdio.h>  
#include <string.h>
#define uchar unsigned char
char str[10];
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
void BT_Reset(void)
{
  GPIO_ResetBits(GPIOE, GPIO_Pin_4);
	delay_ms(500);
	GPIO_SetBits(GPIOE, GPIO_Pin_4);
}
/******************************************************************/
void BT_Init (void)  
{
  BT_Reset();
//  while(wifi_send_command_BT( "AT+SLEEP\r\n", "OK", 2000, 1 )){}
  wifi_send_command_BT( "AT+SLEEP\r\n", "OK", 2000, 1 );
}
/******************************************************************/
unsigned int wifi_send_command_BT(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("--->>send %s",Command);
	delay_ms(300);
	printf("--->>send command length:%d\r\n",strlen(Command));
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
					USART_SendData(UART5,Command[i]);
					while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
			}  
			delay_ms(Timeout);
			if(strstr(user_input_BT,"+WAKE"))NVIC_SystemReset();
			if(strstr(user_input_BT,Response))
			{   
				  printf("---<<set ok\r\n");
	        printf("--->>>BT TEST OK\r\n");
					memset(user_input_BT,0,2048);
					return 0;
			}
					else
					continue;			 
	} 
      printf("--->>send :%s fail!\r\n",Command);
	    printf("--->>> BT TEST FAIL\r\n");
	    memset(user_input_BT,0,2048);
			return 1;
}


