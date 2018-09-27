/******************************************************************************
* @file    CO.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   CO测量
*******************************************************************************/ 
#include <stm32f10x.h>
#include "CO.h"
#include "./usart/bsp_usart.h"

int s;
u8 USART_REC_LENX=0;
u8 USART_RX_BUFX[100]; 
float CO_Value;
/*************Read_CO**************************/
float CO_Read(void)
{
	if(FunCheckSum(USART_RX_BUFX,9)==USART_RX_BUFX[8])
	{
		CO_Value=(USART_RX_BUFX[4]<<8|USART_RX_BUFX[5])*0.1;
  }
	return CO_Value;
}	
int CO_Return(void)
{
	return (USART_RX_BUFX[4]<<8|USART_RX_BUFX[5])*0.1;
}
/*************USART4中断接收粉尘数据****************/
void UART5_IRQHandler(void)
{  
	if(USART_GetFlagStatus(UART5,USART_FLAG_RXNE)!=RESET)
	{
			USART_ClearFlag(UART5,USART_FLAG_RXNE);
			USART_RX_BUFX[USART_REC_LENX]=USART_ReceiveData(UART5);
			USART_REC_LENX++;
	}
	if(USART_REC_LENX>8)
	{
			USART_REC_LENX=0;
	}
}
/***********CO取反校验*******************************************/
unsigned char FunCheckSum(unsigned char *i,unsigned char ln)
{
unsigned char j,tempq=0;
	   i+=1;
	for(j=0;j<(ln-2);j++)
	{
     tempq+=*i;
     i++;		
	}
		 tempq=(~tempq)+1;
		 return(tempq);
}


