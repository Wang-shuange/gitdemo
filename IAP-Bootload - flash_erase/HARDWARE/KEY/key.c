/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void EXTI15_10_IRQHandler(void)
{
  int i;
	//确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{ 
    LED_All_OFF();
		  delay_ms(1000);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)//检测按键是否按下
			{
				 delay_ms(1000);delay_ms(1000);
				 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)
					{   
						  Key_Set_Status=10;
//							Blue_LED_ON();delay_ms(1000);Blue_LED_OFF();
						  for(i=0;i<3;i++){Green_Red_LED_ON();delay_ms(200);Green_Red_LED_OFF();delay_ms(200);}
							if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)//检测按键是否松开
							 {
								  Key_Set_Status=0;
								  printf("\r\n\r\n");
								  SPI_FLASH_SectorErase(0x01000); //擦除FLASH APP更新标志，运行IAP
								  NVIC_SystemReset();
									key_flag=1;    //smartconfig开启标志	
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
											 printf("\r\n恢复出厂设置\r\n");
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
												Write_Key_reset_flag();//非看门狗复位标志
												NVIC_SystemReset();
										 }
							     }
							  }
					  }
			  }
			else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) != 0)
			{
				printf("\r\n按键复位\r\n"); 
				Write_Key_reset_flag();//非看门狗复位标志
				NVIC_SystemReset();
			}
			if(Key_Set_Status==10)  //smartconfig
			{
				printf("\r\nWait enter while\r\n");
				key_flag=1;    //smartconfig开启标志	
			}
			if(Key_Set_Status==20)  //恢复出厂设置
			{
//			  printf("------>>>system_init will doing\r\n");
//				printf("\r\n恢复出厂设置\r\n");
//			  SPI_FLASH_SectorErase(0x00000);
//			  SPI_FLASH_SectorErase(0x03000);
//			  Write_Key_reset_flag();//非看门狗复位标志
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
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;

  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  配置 IO为EXTI中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO),ENABLE);
												
	/* 配置 NVIC 中断*/
	NVIC_Configuration();
	
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}
/*********************************************END OF FILE**********************/
