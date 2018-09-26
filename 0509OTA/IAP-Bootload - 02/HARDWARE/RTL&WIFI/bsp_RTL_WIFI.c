/******************************************************************************
* @file    bsp_WiFi_RT.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   WiFi_RTL初始化
*******************************************************************************/ 
#include "bsp_RTL_WIFI.h"
#include <string.h>  
#include "bsp_SysTick.h"
#include "usart.h"
#include "delay.h"
#include "led.h"  
/********************************************************************
* 函数名：RTL_WIFI_Net_Mode_Choose
* 描述  ：选择WF-RTL_WIFI模块的工作模式
* 输入  ：enumMode，工作模式
* 返回  : 1，选择成功
*         0，选择失败
* 调用  ：被外部调用
********************************************************************/
void RTL_WIFI_Reset()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	delay_ms(1000);		
	GPIO_SetBits(GPIOA, GPIO_Pin_1);	
	delay_ms(1000);			
}
/********************************************************************/
void RTL_WIFI_Init ()
{ 
	    printf("\r\nRtl_Wifi_Init\r\n");
	    RTL_WIFI_Reset();
//    wifi_send_command ( "+++", "OK", 2500,3 );
//    wifi_send_command( "AT#Exit\r\n","OK", 4000,3 ); 
		  while(wifi_send_commandx( "+++", "OK", 2500,1 ));							
//		wifi_send_command ( "AT#StaID -sFAST\r\n", "OK", 2500, 3 );
//		wifi_send_command ( "AT#StaSec -kAJK123456\r\n", "OK", 2500, 3 ); 
//		wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A10.235.206.199 -P6000\r\n", "OK", 5000, 3 );	
//		while(wifi_send_command ( "AT#StaID -sHonorV10\r\n", "OK", 2500, 3 )){}
//	  while(wifi_send_command ( "AT#StaSec -kZQWQAZ123\r\n", "OK", 2500, 3 )); 
//	  while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A192.168.43.141 -P5000\r\n", "OK", 5000, 3 )){}
//		while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A10.180.131.39 -P5000\r\n", "OK", 5000, 3 )){}	
//			wifi_send_commandx( "AT#NWK\r\n", "OK", 5000, 3 );			
			while(wifi_send_command ( "AT#StaID -sARTTEST\r\n", "OK", 2500, 1 )){}
			while(wifi_send_command ( "AT#StaSec -k12345678\r\n", "OK", 2500, 1 )){} 
			while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A172.24.111.4 -P5000\r\n", "OK", 5000, 3 )){}
//			while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A10.199.28.81 -P8443\r\n", "OK", 5000, 3 )){}
//			while(wifi_send_command ( "AT#StaID -sF00414\r\n", "OK", 2500, 1 )){}
//			while(wifi_send_command ( "AT#StaSec -kPassword.123\r\n", "OK", 2500, 1 )); 
//			while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A192.168.0.104 -P5000\r\n", "OK", 5000, 3 ));
//		wifi_send_commandx( "AT#StaSec", "OK", 2500, 3 );	
//		wifi_send_commandx( "AT#SCAN", "OK", 2500, 3 ); 
			while(wifi_send_command ( "AT#Exit\r\n", "OK", 4000,3 )); 
			printf ( "Wifi Connect OK\r\n" );
}
/********************************************************************/
unsigned int wifi_send_command(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("--->send command length:%d\r\n",strlen(Command));
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
					USART_SendData(USART2,Command[i]);
					while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
          delay_ms(Timeout);
			if(strstr(user_input,Response))
			{   
				  printf("--->send %s",Command);
				  printf("---<<set ok\r\n");
					memset(user_input,0,2048);
					return 0;
			}
					else
					continue;			 
	} 
      printf("--->send :%s fail!\r\n",Command);
	    memset(user_input,0,2048);
			return 1;
}
/**********************************************************/
unsigned int wifi_send_exit(char *Command)
{
		int i;
		for(i=0;i<strlen(Command);++i)
		{
				USART_SendData(USART2,Command[i]);
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		}
//		    printf("%s",user_input);
		    memset(user_input,0,2048);
				return 1;
}		
/******************************************************/
unsigned int wifi_send_commandx(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("--->send command length:%d\r\n",strlen(Command));
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
				USART_SendData(USART2,Command[i]);
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
			delay_ms(Timeout);
			
			if(strstr(user_input,Response)||strlen(Command)==8)
			{      
				if(strlen(Command)>4)
				{
					printf("--->send %s",Command);
					printf("---<<Mac ");
					for(i=2;i<20;i++)printf("%c",user_input[i]);
					printf("\r\n");	
				}
				else
				{ 
					printf("--->send %s\r\n",Command);
					printf("---<<set ok\r\n"); 
				}
				memset(user_input,0,2048);
				return 0;
			}
				else
				continue;			 
	 } 
			  return 1;
}


