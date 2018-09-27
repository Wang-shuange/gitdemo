/******************************************************************************
* @file    PM25.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   PM2.5测量
*******************************************************************************/ 
#include "PM25.h"
#include "bsp_usart.h"
#include <string.h> 
#include "utility.h"
/***************************************************/
#define FILTER_N 27
int filter_buf[FILTER_N + 1];
int shuzu[]={22,25,20,21,23,21,24,22,24,21,22,23};//just use to test
char hexdata[7] = {0x42, 0x4D, 0xE3, 0x00, 0x00, 0x01, 0x72};

u8 USART_REC_LEN=0;
u8 USART_RX_BUF[100];               
/***************************************************/
u8 pm_rateH,pm_rateL;
float pm_rate;
unsigned int pm_ratex,c,hx;
u8 flag_stop=0;
long dust_value; 
/***********************************************/
void PM25_Init(void)
{
	printf("dust init about 10 sec\r\n");
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
/************************************************/
void pm_reset_hd()//hard reset
{
	int count=0;//粉尘延时时间计数
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);//粉尘DUST_SET
	GPIO_ResetBits(GPIOD, GPIO_Pin_10);//粉尘DUST_RES
	delay_ms(500);
	GPIO_SetBits(GPIOC, GPIO_Pin_6);//粉尘DUST_SET
	GPIO_SetBits(GPIOD, GPIO_Pin_10);//粉尘DUST_RES
	while(count++<5)
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
/*************读取粉尘数据**************************/
int PM25_Read(void)
{
	 return pm_rate;
}
/************************************************/
long Dust_Read(void)
{
	 return dust_value;     
}
/************************************************/
long Dust_Return(void)
{
   return dust_value;
}
/************************************************/
//滑动滤波算法(此处只做堆帐使用）
int Filter() 
{
  int i;
  long filter_sum = 0;
  filter_buf[FILTER_N-1] = pm_ratex;
  for(i = 0; i < FILTER_N; i++) 
	{
    filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，扔掉低位
    filter_sum += filter_buf[i];
  }
	return (long)(filter_sum*10 );
}
/*************UART4中断接收粉尘数据****************/
void UART4_IRQHandler(void)
{ 
	int temp;	
	while(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)!=RESET)
	{
			USART_ClearFlag(UART4,USART_FLAG_RXNE);
			USART_RX_BUF[USART_REC_LEN]=USART_ReceiveData(UART4);
			USART_REC_LEN++;
	}
	if(USART_REC_LEN>31)
	{
			USART_REC_LEN=0;
			flag_stop=1;
	}
	if(flag_stop==1)
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
		  pm_ratex=USART_RX_BUF[18]<<8|USART_RX_BUF[19];
      dust_value=Filter();	
		  c++;if(c>27)c=0;
	}	
	}
}


