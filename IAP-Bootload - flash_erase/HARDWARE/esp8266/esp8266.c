/******************************************************************************
* @file    esp8266.c
* @author  ART
* @version V1.0
* @date    2018-04-25
* @brief   wifi初始化
*******************************************************************************/ 
#include "esp8266.h"
#include "usart.h"
#include "string.h"
#include "delay.h"
#include "bsp_iwdg.h" 
#include "led.h" 
/********************************************/
char cmdxxx[50];
char AT_StaID[]="AT#StaID -s";
char AT_StaSe[]="AT#StaSec -k";
char AT_Socket[]="AT#Socket -C0 -T0 -M1 -A";
char AT_Port[]=" -P";  

extern char zhon_zhuan[30];
char MAC[30];
char Read_IP[30];
//uint8_t Rx_Buffer[20];
uint8_t SSID_Buffer[30]="ARTTESTx";
uint8_t Password_Buffer[30]="12345678x";
uint8_t Read_IP_Buffer[30];
uint8_t Port_Buffer[20];
uint8_t IP_Buffer[20];
int ix=22;

char code_ssid[30];
char code_password[30];
extern uint8_t Server_IP_Buffer[30];
extern uint8_t Server_Port_Buffer[10];

char buffer[200];
char buffer1[200];
char sd[30],pass[30];
extern char key_flag;
char jump_flag;
/******************************************************/
void WIFI_Reset(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	delay_ms(500);		
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
//  GPIO_SetBits(GPIOA, GPIO_Pin_0);	
}
/******************************************************/
unsigned int wifi_send_commandx(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("command length:%d\r\n",strlen(Command));
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
					USART_SendData(USART2,Command[i]);
					while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
         delay_ms(Timeout);
			   printf("---<<%s\r\n",user_input);
				if(strstr(user_input,Response))
				{
					printf("--->>send %s\r\n",Command);
					memset(user_input,0,2048);
					return 0;
				}
				else
				continue;			 
			} 
        printf("--->>fail!\r\n");
			  return 1;
		}
/******************************************************/
unsigned int wifi_send_command(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;  
	printf("command length:%d\r\n",strlen(Command));
	printf("--->>send %s\r\n",Command);
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
					USART_SendData(USART2,Command[i]);
					while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
          delay_ms(Timeout);
//					printf("---<<%s\r\n",user_input);
				if(strstr(user_input,Response))
				{
					printf("---<<set ok\r\n");
					memset(user_input,0,2048);
					return 0;
				}
				  else
				  continue;			 
	} 
					printf("--->>fail!\r\n");
					return 1;
}
/******************************************************/
unsigned int wifi_send_command_ReadMAC(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;  
	printf("command length:%d\r\n",strlen(Command));
	printf("--->>send %s\r\n",Command);
	for( j=0;j<Retry;++j)
	{
			for(i=0;i<strlen(Command);++i)
			{
					USART_SendData(USART2,Command[i]);				 
					while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
          delay_ms(Timeout);
//			    printf("receive: %s\r\n",user_input);
			    strncpy(MAC,user_input+28,17);
			    printf("---<<MAC: %s\r\n",MAC);
				if(strstr(user_input,Response))
				{
					printf("---<<set ok\r\n");
					memset(user_input,0,2048);
					return 0;
				}
				  else
				  continue;			 
	} 
					printf("--->>fail!\r\n");
					return 1;
}
/**********************************************************/
unsigned int wifi_send_commandxx(char *Command, char *Response, unsigned long Timeout,uchar  Retry)
{
	int i,j;
	printf("command length:%d\r\n",strlen(Command));
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
					printf("--->send %s\r\n",Command);
					printf("---<<set ok\r\n");
					memset(user_input,0,2048);
					return 0;
				}
				  else
				  continue;			 
	} 
					printf("--->send :%s fail!\r\n",Command);
					return 1;
}
/**********************************************************/
void wifi_send_exit(char *Command)
{
	int i;
//	printf("command length:%d\r\n",strlen(Command));
	for(i=0;i<strlen(Command);++i)
	{
			USART_SendData(USART2,Command[i]);
			while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}			
/**********************************************************/		
void wifi_send_data(char* data)
{
	// printf("--->send data :%s\r\n",data);
	 memset(cmd, 0, 400);  
   sprintf(cmd, "AT+CIPSEND=%d\r\n",strlen(data));
	 while(wifi_send_commandx(cmd, ">",100,1));
	 while(wifi_send_commandx(data, "SEND OK", 500,1));
}
/******************************************************/
void wifi_disconnect()
{
	 while(wifi_send_commandx("AT+CIPCLOSE\r\n", "OK",500, 3));
}
void wifi_connect()
{
	if(wifi_send_commandx("AT+CIPCLOSE=?\r\n","OK",100 ,3))
	while(wifi_send_commandx(cmd, "CONNECT",1000 ,3));
}
/******************************************************/
void wifi_init(char* ssid,char* pwd)
{
	int i;
	Blue_LED_ON();//wifi連接指示燈(等待連接)
	printf("******wifi is setting******\r\n");
	WIFI_Reset();
/****************************/	
	wifi_send_exit("+++");//退出透传模式
	delay_ms(20);
	wifi_send_exit("+++");
	delay_ms(20);
	wifi_send_exit("+++");
	delay_ms(20);
/****************************/
	while(wifi_send_command("AT\r\n", "OK",500, 3));
	while(wifi_send_command("AT+RST\r\n", "OK",500, 3));
	while(wifi_send_command("AT+CWMODE=3\r\n", "OK", 100,3)) {};
	while(wifi_send_command_ReadMAC("AT+CIPSTAMAC?\r\n", "OK", 100,3)){};
	memset(cmd,0,400);
  strcpy(cmd, "AT+CWJAP=\"");
  strcat(cmd, ssid);
  strcat(cmd, "\",\"");
  strcat(cmd, pwd);
  strcat(cmd, "\"\r\n");
  while(wifi_send_command(cmd, "WIFI GOT IP", 3000, 1))
  {}//当WiFi处于连接异常时能够通过按键跳转至bin文件接收模式即主while()循环中
//  while(wifi_send_commandx("AT+CIFSR\r\n", "OK", 3000,3));	
	memset(user_input,0,2048);
  memset(cmd, 0, 400); 
  IWDG_Feed(); 	//喂狗（由于执行到此时，余下的命令很难设置成功）	
  strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
	strcat(cmd, "192.168.43.141");
//	strcat(cmd, "172.22.142.3");
//	strcat(cmd, "10.199.61.250");
  strcat(cmd, "\",8443\r\n");

//	strcat(cmd, "10.207.249.140");
//  strcat(cmd, "\",1080\r\n");
		
		
//	strcat(cmd, "mlbserver.foxconn.com");
//  strcat(cmd, "\",1080\r\n");			

	
//	strcat(cmd, (char*)Server_IP_Buffer);
//  strcat(cmd, "\",");
//	strcat(cmd, (char*)Server_Port_Buffer);
//  strcat(cmd, "\r\n");	
	
  while(wifi_send_command(cmd, "CONNECT",6000 ,1))
	{}
	while(wifi_send_command("AT+CIPMODE=1\r\n", "OK", 100,3))
	{}
	while(wifi_send_command("AT+CIPSEND\r\n", ">", 100,3))
	{}
	key_flag=0;
  printf("******wifi is ok******\r\n");
	for(i=0;i<3;i++)flash_green_LED();//WiFi連接OK指示燈
	LED_All_OFF();
	delay_ms(20);


//	printf("******wifi is setting******\r\n");
//	IWDG_Feed(); //喂狗防止前面执行执行时间过长
//	WIFI_Reset();
///****************************/	
//	wifi_send_exit("+++");	
//	delay_ms(20);
//	wifi_send_exit("+++");	
//	delay_ms(20);
///****************************/
//	wifi_send_command("AT\r\n", "OK",500, 3);
////	wifi_send_command("AT+RST\r\n", "OK",1000, 3);
//	wifi_send_command("AT+CWMODE=3\r\n", "OK", 100,3);
//	while(wifi_send_command_ReadMAC("AT+CIPSTAMAC?\r\n", "OK", 100,1)){};
//	memset(cmd,0,400);
//  strcpy(cmd, "AT+CWJAP=\"");
//  strcat(cmd, ssid);
//  strcat(cmd, "\",\"");
//  strcat(cmd, pwd);
//  strcat(cmd, "\"\r\n");
//  wifi_send_command(cmd, "WIFI GOT IP", 3000, 10);
////  while(wifi_send_commandx("AT+CIFSR\r\n", "OK", 3000,3));	
//	memset(user_input,0,2048);
//  memset(cmd, 0, 400); 
////  IWDG_Feed(); 	//喂狗（由于执行到此时，余下的命令需要一定時間才能设置成功）	
//  strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
////	strcat(cmd, "10.251.19.16");
////	strcat(cmd, "192.168.43.141");
////	strcat(cmd, "172.22.142.3");
////	strcat(cmd, "192.168.0.104");
//	strcat(cmd, "mlbserver.foxconn.com");
//  strcat(cmd, "\",1080\r\n");			
//		
////	strcat(cmd, "10.199.61.250");
////  strcat(cmd, "\",8443\r\n");
//		
////	strcat(cmd, (char*)Server_IP_Buffer);
////  strcat(cmd, "\",");
////	strcat(cmd, (char*)Server_Port_Buffer);
////  strcat(cmd, "\r\n");
//  wifi_send_command(cmd, "CONNECT",6000 ,10);
//	memset(cmd, 0, 400); 
//	wifi_send_command("AT+CIPMODE=0\r\n", "OK", 100,3);
////	while(wifi_send_command("AT+CIPSEND\r\n", ">", 100,3)){}
//  printf("******wifi is ok******\r\n");
//	IWDG_Feed();
//	delay_ms(20);
}
/******************************************************/
void ssid_password_jiequ(char*buffer,char*code,int len)
{
  int n,h;
	while(!(strstr(buffer,code)))
	{
   	memset(buffer,0,200);
		strncpy(buffer,user_input+n,len);
		n++;
//	printf("receive:%s\r\n",buffer);
		if(n>110)n=0;
		h=n+len-1;
	}
	  IWDG_Feed();
//	printf("xxxxxxxxxxxxxx %s\r\n",sd);
//	printf("lenth %d\r\n",n);
	while(!(user_input[n]=='\r'&&user_input[n+1]=='\n'))
	{
//	sprintf(sd,"%c",user_input[n]);
		n++;   
//	printf("receivex:%d\r\n",n);
//	printf("receivexxx:%c\r\n",user_input[n]);
	}
	  strncpy(buffer,user_input+h,(n-h)+2);    //(n-h)是计算返回数据的长度/ (n-h)+2是为了添加上\r\n
	  sprintf(cmdxxx,"%s",buffer);
	  memset(buffer,0,50);
	  strncpy(buffer,cmdxxx+0,(n-h));
//	printf("receivexsxs:%s\r\n",buffer);
}
void smartconfig(void)
{
		  int i;
	    GPIO_ResetBits(GPIOE, GPIO_Pin_10);
	    WIFI_Reset();
	while(wifi_send_commandxx("AT\r\n", "OK",500, 3))
		{
/****************************/
			delay_ms(20);
			wifi_send_exit("+++");//退出透传模式
			delay_ms(20);
			wifi_send_exit("+++");
			delay_ms(2000);
/****************************/
		}
		printf("smartconfig is doing\r\n");
		while(wifi_send_commandxx("AT\r\n", "OK",500, 3));
		while(wifi_send_commandxx("AT+RST\r\n", "OK",500, 3));
		while(wifi_send_command("AT+CWMODE=1\r\n", "OK", 100,3)) {};
		while(wifi_send_command_ReadMAC("AT+CIPSTAMAC?\r\n", "OK", 100,3)){};
		while(wifi_send_commandxx("AT+CWSTARTSMART\r\n", "OK", 6000, 1));
		delay_ms(1000);delay_ms(1000);delay_ms(1000);
	while(user_input)
	{
		printf("receive:%s\r\n",user_input);
		printf("---<<<:%d\r\n",strlen(user_input));
		delay_ms(1000);delay_ms(1000);
		if(strlen(user_input)>100)
		{
			printf("Receive PassworD & SSID IS\r\n");
			ssid_password_jiequ(code_ssid,"ssid:",5);
			printf("-<<<    SSID: %s\r\n",code_ssid);
			printf("lenth: %d\r\n",strlen(code_ssid));
			memset(buffer1,0,30);
			ssid_password_jiequ(code_password,"password:",9);
			printf("-<<<Password: %s\r\n",code_password);
			printf("lenth: %d\r\n",strlen(code_password));
//			memset(code_password,0,30);
//			memset(code_ssid,0,30);
			GPIO_SetBits(GPIOE, GPIO_Pin_8);
			for(i=0;i<5;i++)
			{
				delay_ms(200);
				GPIO_ResetBits(GPIOE, GPIO_Pin_9);
				delay_ms(200);
				GPIO_SetBits(GPIOE, GPIO_Pin_9);
			}
			  GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			  memset(user_input,0,2048);
			  break;
		}
		    IWDG_Feed();
	}
}
/**************************************************
char *strncpy(char *dest, char *src, int n);

strncpy( strtemp,str+n , m )

--strtemp 字符串变量,截取后的字符串存放的位置   

--str 字符串变量，要截取的字符串

--n ,int 型,

-- str+n, 表示从N位开始截取字符串

--m,int型,表示截取M位

substr(dstr, src, star, len)

***************************************************/

