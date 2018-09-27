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
#include "utility.h"
#include "./usart/bsp_usart.h"
#include "esp8266.h"
#include "flash.h"
#include "bsp_iwdg.h" 
#include "bsp_led.h" 
#include "IAP_Bootload.h"
#include "string.h"
#include "flash.h"

char smartconfig_off,OPPEN;
extern uint8_t Ethernet_Buffer[10];
char Key_Set_Status,system_init_flag,update_mode_switch;
char key_test_flag;
/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void EXTI15_10_IRQHandler(void)
{
  int i;
	//ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{ 
      LED_All_OFF();
		  delay_ms(1000);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//��ⰴ���Ƿ���
			{
				 delay_ms(1000);delay_ms(1000);
				 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)
					{   
						  Key_Set_Status=10;
						  for(i=0;i<3;i++){Green_Red_LED_ON();delay_ms(200);Green_Red_LED_OFF();delay_ms(200);}
							if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)//��ⰴ���Ƿ��ɿ�
							 {
								  Key_Set_Status=0;
								 	printf("\r\n--->>>Key Three Second Test OK\r\n");
								  key_test_flag=3;
									smartconfig_off=1;    //smartconfig������־	
							 }
							 for(i=0;i<3;i++)
							 {
								  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)break;					 
								  delay_ms(1000);
							 }
							if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//Reset
								{      
									     Key_Set_Status=20;
									     for(i=0;i<3;i++){Green_LED_ON();delay_ms(200);Green_LED_OFF();delay_ms(200);}
									 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
										{					   
                       Key_Set_Status=0;  
											 printf("\r\n--->>>Key Seven Second Test OK\r\n");
											 key_test_flag=7;
											 delay_ms(1000);
										}
								   else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//Reset
								   {   
										 for(i=0;i<2;i++)
										 {
												 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)break;					 
												 delay_ms(1000);
										 }
										 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)
										 {						   
                        for(i=0;i<3;i++){Red_LED_ON();delay_ms(200);Red_LED_OFF();delay_ms(200);}
												printf("\r\n--->>>Key Nine Second Test OK\r\n");
												key_test_flag=9;
										 }
							     }
							  }
					  }
			  }
			else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) != 0)
			{
				printf("\r\n--->>>Key One Second Test OK\r\n");
				key_test_flag=1;
			}
			if(Key_Set_Status==10)  //smartconfig
			{
				printf("\r\n--->>>Key Three Second Test OK\r\n");
				key_test_flag=3;
				smartconfig_off=1;    //smartconfig������־	
			}
			if(Key_Set_Status==20)  //�ָ���������
			{
				printf("\r\n--->>>Key Seven Second Test OK\r\n");
				key_test_flag=7;
			  delay_ms(1000);
			}
			Key_Set_Status=0;
			EXTI_ClearITPendingBit(EXTI_Line11);     
	 }  
}	
void smartconfig_IRQ(void)
{
		int i;
	  smartconfig();
		Flash_Write_SSID_Password();
		Write_smartconfig_flag();
		for(i=0;i<3;i++)flash_green_LED();
		OPPEN=1;IWDG_Feed();
		Write_Key_reset_flag();//�ǿ��Ź���λ��־
		NVIC_SystemReset();				
}
/**********************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;

  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
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
/*********************************************END OF FILE**********************/
