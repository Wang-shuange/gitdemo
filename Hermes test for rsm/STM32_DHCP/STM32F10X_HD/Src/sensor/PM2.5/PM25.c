/******************************************************************************
* @file    PM25.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   PM2.5测量
*******************************************************************************/ 
#include "PM25.h"
#include "bsp_usart1.h"
#include <string.h> 
#include "delay.h"
#include <math.h>
/***************************************************/
#define FILTER_N 27
unsigned int filter_buf[FILTER_N + 1],x;
int shuzu[]={22,25,20,21,23,21,24,22,24,21,22,23};//just use to test
double receive_buffer;
unsigned int Gap;
int test[10]={1,2,3,4,5,6,7,8};
u8 USART_REC_LEN=0;
long USART_RX_BUF[32]; 
char hexdata[7] = {0x42, 0x4D, 0xE3, 0x00, 0x00, 0x01, 0x72};
long DUST;
/***************************************************/
u8 pm_rateH,pm_rateL;
float pm_rate;
unsigned int pm_ratex,c,hx;
u8 flag_stop=0,flag_stopx=0;
extern char hexdata[7];
int pm25_cont,Queue_flag,flag_print;
/***********************************************/
void PM25_Init(void)
{
  pm_reset_hd();             //粉尘sensor硬件复位（含10s延时）
	USART_Cmd(UART4,ENABLE);   //使能串口4
	pm25_reset_sw();           //软件复位
	USART_Cmd(UART4,DISABLE);  //失能串口4
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//开启串口接收中断
	USART_Cmd(UART4,ENABLE);   //使能串口4
}
void enable_5v(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOD,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);	
		GPIO_SetBits(GPIOD,GPIO_Pin_3); //5V电源使能公板	
}
/*************读取粉尘数据**************************/
int PM25_Read(void)
{
	return pm_rate;
}
/************************************************/
long Dust_Read(void)
{
	return pm_ratex; 
}
/************************************************/
long DUST_Calculate(void)
{
/**********************************************/
  	if(Gap>0&&Gap<=2)
		{
			if(flag_print==100)
			{
				DUST=pm_ratex*10;
				printf("--------->>>>>>:Dust53=%ld\r\n",DUST);
				pm25_reset_sw();
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
		if(Gap>2&&Gap<=10)
		{
			if(flag_print==145)
			{
				DUST=pm_ratex*10;
				printf("--------->>>>>>:Dust53=%ld\r\n",DUST);
				pm25_reset_sw();
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
		if(Gap>10&&Gap<=30)
		{
			if(flag_print==200)
			{
				DUST=pm_ratex*10;
				printf("--------->>>>>>:Dust53=%ld\r\n",DUST);
				pm25_reset_sw();
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
		if(Gap>30&&Gap<=100)
		{
			if(flag_print==220)
			{
				DUST=pm_ratex*10;
				printf("--------->>>>>>:Dust53=%ld\r\n",DUST);
				pm25_reset_sw();
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
		if(Gap>100&&Gap<=200)
		{
			if(flag_print==240)
			{
				DUST=pm_ratex*10;
				printf("--------->>>>>>:Dust53=%ld\r\n",DUST);
				pm25_reset_sw();
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
/**********************************************/		
    if(Gap>200)
		{
			if(flag_print==280) 
			{
				DUST=pm_ratex*10;
				printf("--------->>>>>>:Dust280=%ld\r\n",DUST);
				pm25_reset_sw();
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
		return DUST;
}
/************************************************/
void pm_reset_hd()//hard reset
{
		int count=0;//粉尘延时时间计数
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);//粉尘DUST_SET
		GPIO_ResetBits(GPIOD, GPIO_Pin_10);//粉尘DUST_RES
    delay_ms(500);
	  GPIO_SetBits(GPIOC, GPIO_Pin_6);//粉尘DUST_SET
		GPIO_SetBits(GPIOD, GPIO_Pin_10);//粉尘DUST_RES
		while(count++<10)
		{
			delay_ms(1000);
		}
}
void pm25_reset_sw() //soft reset
{
	int i;
	for(i=0;i<7;++i)
	{
		printf("command[%d]:%x\r\n",i,hexdata[i]);
		USART_SendData(UART4,hexdata[i]);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
	}
}		
/************************************************/
int Filter() 
{
	int temp;	
/*********************************/	
	if(USART_REC_LEN>31)
	{
			USART_REC_LEN=0;
			flag_stop=1;
	}
	if(flag_stop==1&&USART_RX_BUF[0]==0x42&&USART_RX_BUF[1]==0x4d)
	{
		  flag_stop=0;
			temp=USART_RX_BUF[0]+USART_RX_BUF[1]\
			+USART_RX_BUF[2]+USART_RX_BUF[3]\
			+USART_RX_BUF[4]+USART_RX_BUF[5]\
			+USART_RX_BUF[6]+USART_RX_BUF[7]\
			+USART_RX_BUF[8]+USART_RX_BUF[9]\
			+USART_RX_BUF[10]+USART_RX_BUF[11]\
			+USART_RX_BUF[12]+USART_RX_BUF[13]\
			+USART_RX_BUF[14]+USART_RX_BUF[15]\
			+USART_RX_BUF[16]+USART_RX_BUF[17]\
			+USART_RX_BUF[18]+USART_RX_BUF[19]\
			+USART_RX_BUF[20]+USART_RX_BUF[21]\
			+USART_RX_BUF[22]+USART_RX_BUF[23]\
			+USART_RX_BUF[24]+USART_RX_BUF[25]\
			+USART_RX_BUF[26]+USART_RX_BUF[27]\
			+USART_RX_BUF[28]+USART_RX_BUF[29];
	  if(temp==((USART_RX_BUF[30]<<8)|USART_RX_BUF[31]))
	  {
			pm_rateH=USART_RX_BUF[12];
			pm_rateL=USART_RX_BUF[13];
			pm_rate=pm_rateH<<8|pm_rateL;
			pm_ratex=(USART_RX_BUF[18]<<8)|USART_RX_BUF[19];
			memset(USART_RX_BUF,0,32);
			filter_buf[c] = pm_ratex;
			if(c<28)c++;	  
			if(pm25_cont==100)flag_print=100;
			if(pm25_cont==145)flag_print=145;
			if(pm25_cont==200)flag_print=200;
			if(pm25_cont==220)flag_print=220;
			if(pm25_cont==240)flag_print=240;
			if(pm25_cont==280)flag_print=280;
			pm25_cont++;
		if(c==28&&Queue_flag==0)
		{	
			Queue_flag=1;
			Gap=0;
			for(x=0;x<c-1;x++)
			{
				Gap+=fabs(filter_buf[x+1]-filter_buf[x]);
			}
			memset(filter_buf,0,28);	
			Gap=Gap/27;
		}
	 }	
 }
	return (long)pm_ratex;
}
/*************UART4中断接收粉尘数据****************/
void UART4_IRQHandler(void)
{ 
	while(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)!=RESET)
	{
			USART_ClearFlag(UART4,USART_FLAG_RXNE);
			USART_RX_BUF[USART_REC_LEN]=USART_ReceiveData(UART4);
			USART_REC_LEN++;
	}
      Filter();
}

