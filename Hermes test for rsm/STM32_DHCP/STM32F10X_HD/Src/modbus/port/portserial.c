/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include <string.h>
#define uchar unsigned char
int i;
extern char user_input[20];
char Modbus_usart_buff[50];
/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable ) //控制串口的收发中断
{
	if(TRUE==xRxEnable)
	{
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}
	else
	{
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		GPIO_SetBits(GPIOB,GPIO_Pin_0);		
	}

	if(TRUE==xTxEnable)
	{
		USART_ITConfig(USART3, USART_IT_TC, ENABLE);
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
	}
	else
	{
	   USART_ITConfig(USART3, USART_IT_TC, DISABLE);
		 GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}
}
/*****************************************
* 配置串口 目前除了波特率其他参数无效 
* Usart1 9600-8-N-1
*****************************************/
BOOL 
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		USART_ClockInitTypeDef USART_ClockInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* 使能串口3，PA，AFIO总线 */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
//            RCC_APB2Periph_AFIO |
//            RCC_APB2Periph_USART3 , 
//            ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO ,ENABLE);

    /* B-Pin_10 USART3_Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//推挽输出-TX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* B-Pin_10 USART3_Rx  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入-RX
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = ulBaudRate;     //目前只修改波特率
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	  USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	  USART_ClockInit(USART3, &USART_ClockInitStructure);
    USART_Init(USART3, &USART_InitStructure);

	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //两级占先 八级副优先
	  USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
	  USART_ClearITPendingBit(USART3, USART_IT_TC);
		/* Enable USART3_IRQn  Channel */
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
//		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
    /* ENABLE the USARTx */
    USART_Cmd(USART3, ENABLE);
		
		//最后配置485发送和接收模式
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		//GPIOF.10	GPIOF.11
		/*开启LED相关的GPIO外设时钟*/
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* 输出 */
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		printf("usart3 init ok\r\n");
		
	return TRUE;
}
//void USART3_IRQHandler(void)
//{
//  uchar ucTemp;
//	while(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
//	{		
//		USART_ClearFlag(USART1,USART_FLAG_RXNE);
//		ucTemp = USART_ReceiveData(USART1);
//		strcat(Modbus_usart_buff,(const char*)&ucTemp);		
//	}
//}
// 串口发
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    USART_SendData(USART3, ucByte);

    return TRUE;
}

// 串口收
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    *pucByte = USART_ReceiveData(USART3);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
 void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{

     pxMBFrameCBByteReceived(  );
}

