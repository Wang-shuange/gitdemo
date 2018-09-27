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
#include "key.h"  
#include "delay.h"
#include "usart.h"
#include "esp8266.h"
#include "bsp_spi_flash.h"
#include "bsp_iwdg.h" 
#include "led.h"
#include "string.h"
char key_flag,OPPEN;
extern uint8_t Ethernet_Buffer[10];
char Key_Set_Status,system_init_flag,update_mode_switch;
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
//							Blue_LED_ON();delay_ms(1000);Blue_LED_OFF();
						  for(i=0;i<3;i++){Green_Red_LED_ON();delay_ms(200);Green_Red_LED_OFF();delay_ms(200);}
							if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)//��ⰴ���Ƿ��ɿ�
							 {
								  Key_Set_Status=0;
								  printf("\r\n\r\n");
								  SPI_FLASH_SectorErase(0x01000); //����FLASH APP���±�־������IAP
								  NVIC_SystemReset();
									key_flag=1;    //smartconfig������־	
							 }
							 for(i=0;i<3;i++)
							 {
								  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)break;					 
								  delay_ms(1000);
							 }
							if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//Reset
								{      
									     Key_Set_Status=20;
//											 Green_LED_ON();delay_ms(1000);Green_LED_OFF();
									     for(i=0;i<3;i++){Green_LED_ON();delay_ms(200);Green_LED_OFF();delay_ms(200);}
									 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
										{					   
                       Key_Set_Status=0;  
											 printf("\r\n�ָ���������\r\n");
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
//												Red_LED_ON();delay_ms(1000);Red_LED_OFF();
                        for(i=0;i<3;i++){Red_LED_ON();delay_ms(200);Red_LED_OFF();delay_ms(200);}
												Write_Key_reset_flag();//�ǿ��Ź���λ��־
												NVIC_SystemReset();
										 }
							     }
							  }
					  }
			  }
			else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) != 0)
			{
				printf("\r\n������λ\r\n"); 
				Write_Key_reset_flag();//�ǿ��Ź���λ��־
				NVIC_SystemReset();
			}
			if(Key_Set_Status==10)  //smartconfig
			{
				printf("\r\nWait enter while\r\n");
				key_flag=1;    //smartconfig������־	
			}
			if(Key_Set_Status==20)  //�ָ���������
			{
//			  printf("------>>>system_init will doing\r\n");
//				printf("\r\n�ָ���������\r\n");
//			  SPI_FLASH_SectorErase(0x00000);
//			  SPI_FLASH_SectorErase(0x03000);
//			  Write_Key_reset_flag();//�ǿ��Ź���λ��־
//			  delay_ms(1000);
//			  NVIC_SystemReset();
			}
			Key_Set_Status=0;
			EXTI_ClearITPendingBit(EXTI_Line11);     
	 }  
}	
/**********************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;

  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
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
