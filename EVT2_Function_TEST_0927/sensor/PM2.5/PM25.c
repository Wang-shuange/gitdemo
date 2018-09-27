/******************************************************************************
* @file    PM25.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   PM2.5测量
*******************************************************************************/ 
#include "PM25.h"
#include "./usart/bsp_usart.h"
#include <string.h> 
#include "utility.h"
/***************************************************/
#define FILTER_N 27
unsigned int filter_buf[FILTER_N + 1],x;
int shuzu[]={22,25,20,21,23,21,24,22,24,21,22,23};//just use to test
double receive_buffer;
unsigned int Gap;
int test[10]={1,2,3,4,5,6,7,8};
u8 USART_REC_LEN=0;
long USART_RX_BUF[32]; 
/***************************************************/
u8 pm_rateH,pm_rateL;
float pm_rate;
unsigned int pm_ratex,c,hx;
u8 flag_stop=0,flag_stopx=0;

double PM25,DUST;
extern char hexdata[7];
int pm25_cont,Queue_flag,flag_print;
/*************读取粉尘数据**************************/
int PM25_Read(void)
{
	 return pm_rate;
}
/************************************************/
float PM25_Return(void)
{
	 return pm_rate;
}
/************************************************/
long Dust_Return(void)
{
    return Filter();
}
/************************************************/
//滑动滤波算法(此处只做堆帐使用）
//int Filter() 
//{
//  int i;
//  long filter_sum = 0;
//  filter_buf[FILTER_N] = pm_ratex;
//	x++;if(x>28)x=0;
//  for(i = 0; i < FILTER_N; i++) 
//	{
//    filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，扔掉低位
//    filter_sum += filter_buf[i];
//  }
////	printf("sdsdsd %.2f\r\n",(float)((float)filter_sum));
////  return (int)(filter_sum / FILTER_N);
//	return (long)(filter_sum*10 );
//}
int Filter() 
{
	int temp;	
////////////////////////////////////////	
	if(USART_REC_LEN>31)
	{
			USART_REC_LEN=0;
			flag_stop=1;
	}
	if(USART_RX_BUF[0]!=0x42&&USART_RX_BUF[1]!=0x4d)
	{
	    flag_stopx=1;
		  flag_stop=0;
	}
	if(flag_stop==1)
	{
		  flag_stop=0;
		  flag_stopx=0;
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
			PM25=pm_rate;	       //粉尘测量	
			DUST=pm_ratex;       //粉尘浓度
			if(pm25_cont==53)flag_print=53;
			if(pm25_cont==280)flag_print=280;
			pm25_cont++;
		if(c==28&&Queue_flag==0)
		{	
			Queue_flag=1;
			for(x=0;x<c-1;x++)
			{
				Gap+=filter_buf[x+1]-filter_buf[x];
			}
			memset(filter_buf,0,28);	
			Gap=Gap/(c+1);
			printf("\r\nGAP:%d\r\n",Gap);
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
//void UART4_IRQHandler(void)
//{ 
//	int temp;	
//	while(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)!=RESET)
//	{
//			USART_ClearFlag(UART4,USART_FLAG_RXNE);
//			USART_RX_BUF[USART_REC_LEN]=USART_ReceiveData(UART4);
//			USART_REC_LEN++;
//	}
//	if(USART_REC_LEN>31)
//	{
//			USART_REC_LEN=0;
//			flag_stop=1;
//	}
//	if(flag_stop==1)
//	{
//		  flag_stop=0;
//			temp=USART_RX_BUF[0]+USART_RX_BUF[1]\
//			+USART_RX_BUF[2]+USART_RX_BUF[3]\
//			+USART_RX_BUF[4]+USART_RX_BUF[5]\
//			+USART_RX_BUF[6]+USART_RX_BUF[7]\
//			+USART_RX_BUF[8]+USART_RX_BUF[9]\
//			+USART_RX_BUF[10]+USART_RX_BUF[11]\
//			+USART_RX_BUF[12]+USART_RX_BUF[13]\
//			+USART_RX_BUF[14]+USART_RX_BUF[15]\
//			+USART_RX_BUF[16]+USART_RX_BUF[17]\
//			+USART_RX_BUF[18]+USART_RX_BUF[19]\
//			+USART_RX_BUF[20]+USART_RX_BUF[21]\
//			+USART_RX_BUF[22]+USART_RX_BUF[23]\
//			+USART_RX_BUF[24]+USART_RX_BUF[25]\
//			+USART_RX_BUF[26]+USART_RX_BUF[27]\
//			+USART_RX_BUF[28]+USART_RX_BUF[29];
//	if(temp==((USART_RX_BUF[30]<<8)|USART_RX_BUF[31]))
//	{
//			pm_rateH=USART_RX_BUF[12];
//			pm_rateL=USART_RX_BUF[13];
//			pm_rate=pm_rateH<<8|pm_rateL;
//		  pm_ratex=USART_RX_BUF[18]<<8|USART_RX_BUF[19];
//		  c++;	  
//	}	
//	}
//}


