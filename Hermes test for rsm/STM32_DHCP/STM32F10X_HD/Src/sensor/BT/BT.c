#include "BT.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include <string.h>  
#include "delay.h"
#include <stdio.h>  
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
void BT_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA , ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOE, &GPIO_InitStructure);
/**********************************************///WiFi失能
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);	
}
void BT_Reset(void)
{
	int count=3;
	while(count--)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_4);
		delay_ms(500);
		GPIO_SetBits(GPIOE, GPIO_Pin_4);
		delay_ms(500);
	}
}
/******************************************************************/
void BT_Init (void)  
{
  BT_Reset();
  while(wifi_send_command_BT( "AT+SLEEP\r\n", "OK", 5000, 1 )){}
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
//			printf("receive: %s\r\n",user_input_BT);
			if(strstr(user_input_BT,"+WAKE"))NVIC_SystemReset();
//			if(user_input_BT[1]=='W')NVIC_SystemReset();
//			strncpy(str,user_input_BT+0,5);
//			printf("receivex: %s\r\n",str);
//			if(strstr(str,"+WAKE"))NVIC_SystemReset();
			if(strstr(user_input_BT,Response))
			{   
				  printf("---<<set ok\r\n");
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


