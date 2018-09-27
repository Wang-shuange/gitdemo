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
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable ) //���ƴ��ڵ��շ��ж�
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
* ���ô��� Ŀǰ���˲���������������Ч 
* Usart1 9600-8-N-1
*****************************************/
BOOL 
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		USART_ClockInitTypeDef USART_ClockInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* ʹ�ܴ���3��PA��AFIO���� */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
//            RCC_APB2Periph_AFIO |
//            RCC_APB2Periph_USART3 , 
//            ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO ,ENABLE);

    /* B-Pin_10 USART3_Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�������-TX
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* B-Pin_10 USART3_Rx  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������-RX
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = ulBaudRate;     //Ŀǰֻ�޸Ĳ�����
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

	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //����ռ�� �˼�������
	  USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
	  USART_ClearITPendingBit(USART3, USART_IT_TC);
		/* Enable USART3_IRQn  Channel */
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
//		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    /* ENABLE the USARTx */
    USART_Cmd(USART3, ENABLE);
		
		//�������485���ͺͽ���ģʽ
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		//GPIOF.10	GPIOF.11
		/*����LED��ص�GPIO����ʱ��*/
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* ��� */
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
// ���ڷ�
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    USART_SendData(USART3, ucByte);

    return TRUE;
}

// ������
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

