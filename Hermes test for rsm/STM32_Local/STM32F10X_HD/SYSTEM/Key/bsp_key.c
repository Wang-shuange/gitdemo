/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-MINI STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "bsp_key.h"  
#include "delay.h"
#include "bsp_usart1.h"
#include "flash.h"
#include "bsp_iwdg.h" 
#include "bsp_led.h" 
#include "IAP_Bootload.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"

char smartconfig_off,OPPEN;
extern uint8_t Ethernet_Buffer[10];
char Key_Set_Status,system_init_flag,update_mode_switch;
void delay(double t)
{
  while(t--);
}
//delay(0x4ffff);��Լ��ʱ1s
/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{
		delay(0x2ffff);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1) 
		{
			printf("\r\n������λ\r\n"); 
		}
		else
		{
			delay(0x4ffff);delay(0x3ffff);
      delay(0x4ffff);delay(0x4ffff);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0) 
			{
			  printf("\r\n��ת��IAP����BootLoader����\r\n");
				Ethernet_Write_flag();   //д����̫����־
				Flash_Write();	         //����APP_FLAG��־"APP IS ERASE OK"														 
			}	
		}
	}
	NVIC_SystemReset();
	EXTI_ClearITPendingBit(EXTI_Line11);  
}	
/**********************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}

 /**
  * @brief  ���� IOΪEXTI�жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO),ENABLE);
												
	/* ���� NVIC �ж�*/
	NVIC_Configuration();
	
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}


