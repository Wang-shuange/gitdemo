/******************************************************************************
* @file    wifi_esp8266.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   WiFi_esp8266≥ı ºªØ
*******************************************************************************/ 
#include "wifi_esp8266.h"
#include <string.h>
#include "./usart/bsp_usart.h"
#include "delay.h"
/******************************************************/	
//unsigned int wifi_send_command(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
//{
//	int i,j;
//	printf("command length:%d\r\n",strlen(Command));
//	for( j=0;j<Retry;++j)
//	{
//			for(i=0;i<strlen(Command);++i)
//			{
//					USART_SendData(USART2,Command[i]);
//					while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//			}
//          delay_ms(Timeout);
//			if(strstr(user_input,Response))
//			{
//					printf("--->send %s successful\r\n",Command);
//					memset(user_input,0,2048);
//					return 0;
//			}
//					else
//					continue;			 
//			} 
//      printf("--->send :%s fail!\r\n",Command);
//			return 1;
//}

//unsigned int wifi_send_exit(char *Command)
//{
//		int i;
//		printf("command length:%d\r\n",strlen(Command));
//		for(i=0;i<strlen(Command);++i)
//		{
//				USART_SendData(USART2,Command[i]);
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//		}
//				return 1;
//}		
///******************************************************/
//unsigned int wifi_send_commandx(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
//{
//	int i,j;
//	for( j=0;j<Retry;++j)
//	{
//			for(i=0;i<strlen(Command);++i)
//			{
//				USART_SendData(USART2,Command[i]);
//				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//			}
//          delay_ms(Timeout);
//			if(strstr(user_input,Response))
//			{
//				memset(user_input,0,2048);
//				return 0;
//			}
//				else
//				continue;			 
//	 } 
//			  return 1;
//}
///******************************************************/
//void wifi_send_data(char* data)
//{
//	// printf("--->send data :%s\r\n",data);
//	 memset(cmd, 0, 400);  
//   sprintf(cmd, "AT+CIPSEND=%d\r\n",strlen(data));
//	 while(wifi_send_commandx(cmd, ">",100,1));
//	 while(wifi_send_commandx(data, "SEND OK", 500,1));
//}
//void wifi_disconnect()
//{
//	 while(wifi_send_commandx("AT+CIPCLOSE\r\n", "OK",500, 3));
//}
//void wifi_connect()
//{
//	if(wifi_send_commandx("AT+CIPCLOSE=?\r\n","OK",100 ,3))
//	while(wifi_send_commandx(cmd, "CONNECT",1000 ,3));
//}
// /******************************************************/
//void wifi_init(char* ssid,char* pwd)
//{
//		delay_ms(20);
//		wifi_send_exit("+++");
//		delay_ms(20);
//		wifi_send_exit("+++");
//		delay_ms(2000);
//		while(wifi_send_command("AT+RST\r\n", "OK",500, 3));
//		while(wifi_send_command("AT+CWMODE=3\r\n", "OK", 100,3)) {};
//		memset(cmd,0,400);
//		strcpy(cmd, "AT+CWJAP=\"");
//		strcat(cmd, ssid);
//		strcat(cmd, "\",\"");
//		strcat(cmd, pwd);
//		strcat(cmd, "\"\r\n");
//		while(wifi_send_command(cmd, "WIFI GOT IP", 1000, 3));
//		memset(user_input,0,2048);
//	//  while(wifi_send_command("AT+CIFSR\r\n", "OK", 100,1));
//		memset(cmd, 0, 400);  
//		strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
//		strcat(cmd, "192.168.0.116");
//		strcat(cmd, "\",8080\r\n");
//		while(wifi_send_command(cmd, "CONNECT",1000 ,3));
//		while(wifi_send_command("AT+CIPMODE=1\r\n", "OK", 100,3));
//		while(wifi_send_command("AT+CIPSEND\r\n", ">", 100,3));
//		delay_ms(20);
//}
