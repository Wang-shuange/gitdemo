/******************************************************************************
* @file    CO.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   CO测量
*******************************************************************************/ 
#include "CO.h"
#include "./usart/bsp_usart.h"

int s;
u8 USART_REC_LENX=0;
u8 USART_RX_BUFX[100]; 
/*************Read_CO**************************/
void CO_Read(void)
{
	if(FunCheckSum(USART_RX_BUFX,9)==USART_RX_BUFX[8])
	{
		printf("CO Value: %.2f\r\n",(USART_RX_BUFX[4]<<8|USART_RX_BUFX[5])*0.1);
  }
}	
int CO_Return(void)
{
	return (USART_RX_BUFX[4]<<8|USART_RX_BUFX[5])*0.1;
}
/*************USART4中断接收粉尘数据****************/
void USART3_IRQHandler(void)
{  
	if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)!=RESET)
	{
			USART_ClearFlag(UART4,USART_FLAG_RXNE);
			USART_RX_BUFX[USART_REC_LENX]=USART_ReceiveData(UART4);
			USART_REC_LENX++;
	}
	if(USART_REC_LENX>8)
	{
			USART_REC_LENX=0;
	}
	printf("xxxxxxxxxxxxxxxxxxx");
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


