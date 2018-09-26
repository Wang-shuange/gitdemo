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
#include "./usart/bsp_usart.h"
#include "delay.h"
#include "bsp_led.h"  
#include "bsp_spi_flash.h"
#include "bsp_iwdg.h"  

char cmdxxx[50];
char AT_StaID[]="AT#StaID -s";
char AT_StaSe[]="AT#StaSec -k";
char AT_Socket[]="AT#Socket -C0 -T0 -M1 -A";
char AT_Port[]=" -P";  

extern char zhon_zhuan[30];
//char Mac[]="34:00:8a:e1:44:2e";
char Read_IP[30];
uint8_t Rx_Buffer[20];
uint8_t SSID_Buffer[30];
uint8_t Password_Buffer[30];
uint8_t Read_IP_Buffer[30];
uint8_t Port_Buffer[20];
uint8_t IP_Buffer[20];
int ix=22;
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
		GPIO_ResetBits(LED7_GPIO_PORT, LED7_GPIO_PIN);
		delay_ms(1000);		
		GPIO_SetBits(LED7_GPIO_PORT, LED7_GPIO_PIN);	
		delay_ms(1000);			
}
/********************************************************************/
void SmartConfig()
{
	  RTL_WIFI_Reset();
		wifi_send_command ( "+++", "OK", 2500,3 );
		wifi_send_command ( "AT#SMTCFG\r\n", "OK", 2500, 3 );
		wifi_send_command ( "AT#Exit\r\n", "OK", 4000,3 ); 
	  delay_ms(1000);delay_ms(1000);delay_ms(1000);
	  memset(user_input,0,2048);
		printf(" smartconfig is doing\r\n");
		while(!strstr(user_input,"STA Start"))
	{
		printf("receive <<<: %s\r\n",user_input);
		memset(user_input,0,1024);	
    delay_ms(500);
		IWDG_Feed();       //喂狗		
	}	
    printf("receive <<<: %s\r\n",user_input);
	  printf(" smartconfig set ok \r\n");
		memset(user_input,0,1024);		
}
/********************************************************************/
void RTL_WIFI_Init ()
{ 
		printf("\r\nRtl_Wifi_Init \r\n");
		RTL_WIFI_Reset();
		while(wifi_send_command( "+++", "OK", 2500,3 )){}
/****************************************************************/				
//		wifi_send_command ( "AT#StaID -sFAST\r\n", "OK", 2500, 3 );
//		wifi_send_command ( "AT#StaSec -kAJK123456\r\n", "OK", 2500, 3 ); 
//		wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A10.235.206.199 -P6000\r\n", "OK", 5000, 3 );	
/*************************/				
//		while(wifi_send_command ( "AT#StaID -sHonorV10\r\n", "OK", 2500, 3 )){}
//	  while(wifi_send_command ( "AT#StaSec -kZQWQAZ123\r\n", "OK", 2500, 3 )){} 
//	  while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A10.201.10.158 -P5000\r\n", "OK", 5000, 3 ));
/*****************************************************/				
//		while(wifi_send_commandx( "AT#NWK\r\n", "OK", 3000, 3 ));
//		Mac_IP_DisPlay();  //打印Mac与IP地址
//    while(wifi_send_command ( (char*)SSID_Buffer, "OK", 2500, 1 )){}  //设置SSID
//    memset((char*)SSID_Buffer,0,30);
//    while(wifi_send_command ( (char*)Password_Buffer, "OK", 3000, 1 )){}  //设置Password
//    memset((char*)Password_Buffer,0,30);
//    while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A10.199.61.250 -P8443\r\n", "OK", 5000, 3 )){}
/******************************************************/			
		while(wifi_send_command ( "AT#StaID -sARTTEST\r\n", "OK", 2500, 1 )){}
		while(wifi_send_command ( "AT#StaSec -k12345678\r\n", "OK", 2500, 1 )){}; 
		while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A172.24.111.4 -P5000\r\n", "OK", 5000, 3 )){}
//		while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A10.199.28.81 -P8443\r\n", "OK", 5000, 3 )){}
/*************************/				
//		while(wifi_send_command ( "AT#StaID\r\n", "OK", 2500, 1 )){}
//		while(wifi_send_command ( "AT#StaSec\r\n", "OK", 2500, 1 )){}; 
//    while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A172.27.35.12 -P5000\r\n", "OK", 5000, 3 )){};
//		while(wifi_send_command ( "AT#SMTCFG\r\n", "OK", 5000, 3 )){};
/*************************/					
//		while(wifi_send_command ( "AT#StaID -sF00414\r\n", "OK", 2500, 1 )){}
//		while(wifi_send_command ( "AT#StaSec -kPassword.123\r\n", "OK", 2500, 1 )); 
//		while(wifi_send_command ( "AT#Socket -C0 -T0 -M1 -A192.168.1.120 -P5000\r\n", "OK", 5000, 3 )){};
/*************************/
//	  wifi_send_commandx( "AT#StaSec", "OK", 2500, 3 );	
//	  wifi_send_commandx( "AT#SCAN", "OK", 2500, 3 ); 
		while(wifi_send_command ( "AT#Exit\r\n", "OK", 4000,3 )){} 
		printf ( "Wifi Connect OK\r\n" );
		IWDG_Feed();	
}
/********************************************************************/
void Mac_IP_DisPlay()
{   
	  int i;		
//	  printf("All_Code: %s\r\n",user_input);
	  printf("---<<Mac ");
		for(i=2;i<20;i++)printf("%c",user_input[i]);
//	  for(i=2;i<20;i++)Mac[i]=user_input[i];
		printf("\r\n");
	  if(user_input[21]=='A')
		{  
			i=22;
			do
			{
			  Read_IP[i-22]=user_input[i];
				i++;
			}
			while(user_input[i]!=0&&user_input[i+1]!='-');			
		}
//		if(user_input[23]==0&&user_input[25]==0&&user_input[27]==0)
		if(strlen(user_input)<10)
	     NVIC_SystemReset();
//		printf("fff%s\r\n",Read_IP);			
	  memset(user_input,0,30);  
}
/**********************************************************/
unsigned int wifi_send_command(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("--->>send command length:%d\r\n",strlen(Command));
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
				  printf("--->>send %s",Command);
				  printf("---<<set ok\r\n");
//				  printf("xxx<<<: %s\r\n",user_input);
					memset(user_input,0,2048);
					return 0;
			}
					else
					continue;			 
	} 
      printf("--->>send :%s fail!\r\n",Command);
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
		    memset(user_input,0,2048);
				return 1;
}		
/******************************************************/
unsigned int wifi_send_commandx(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("--->>send command length:%d\r\n",strlen(Command));
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
					printf("--->>send %s",Command);
//					sprintf(zhon_zhuan,"%s",user_input);//中转打印:void Mac_IP_DisPlay()
				}
				else
				{ 
					printf("--->>send %s\r\n",Command);
					printf("---<<set ok\r\n"); 
				}
//				memset(user_input,0,2048);
				return 0;
			}
				else
				continue;			 
	 } 
			  return 1;
}
/**********************************************************/
unsigned int wifi_send_commandxx(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("--->>send %s\r\n",Command);
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
				USART_SendData(USART2,Command[i]);
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
          delay_ms(Timeout);
			    for(i=17;i<2048;i++)user_input[i]=0;
					for(i=0;i<strlen(user_input);i++)zhon_zhuan[i]=user_input[i];
					memset(user_input,0,2048);
					ix=0;	 
	 } 
	 return 0 ;
}

