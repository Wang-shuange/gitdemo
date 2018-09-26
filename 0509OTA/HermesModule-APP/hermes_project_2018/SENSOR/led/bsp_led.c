/******************************************************************************
* @file    bsp_led.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   led应用函数接口
*******************************************************************************/  
#include "bsp_led.h"   
/**
* @brief  初始化控制LED的IO
* @param  无
* @retval 无
*/
void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK|LED6_GPIO_CLK|LED7_GPIO_CLK|LED13_GPIO_CLK , ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);			
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
/**********************************************************/		
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
    GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;	
		GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;
		GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED7_GPIO_PIN;     //Wifi使能引腳
		GPIO_Init(LED7_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED8_GPIO_PIN;     
		GPIO_Init(LED8_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED9_GPIO_PIN;     
		GPIO_Init(LED9_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED10_GPIO_PIN;     
		GPIO_Init(LED10_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED11_GPIO_PIN;     
		GPIO_Init(LED11_GPIO_PORT, &GPIO_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = LED13_GPIO_PIN;     
		GPIO_Init(LED13_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED14_GPIO_PIN;     
		GPIO_Init(LED14_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED15_GPIO_PIN;     
		GPIO_Init(LED15_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED16_GPIO_PIN;     
		GPIO_Init(LED16_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED17_GPIO_PIN;     
		GPIO_Init(LED17_GPIO_PORT, &GPIO_InitStructure);
/********************公板RGB使能***********************************/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
/******************Hermes公板使能电压******************************/		
//		GPIO_SetBits(LED5_GPIO_PORT, LED5_GPIO_PIN);
		GPIO_SetBits(LED6_GPIO_PORT, LED6_GPIO_PIN);
		GPIO_ResetBits(LED9_GPIO_PORT, LED9_GPIO_PIN); //光照INT_LIGHT
		GPIO_ResetBits(LED8_GPIO_PORT, LED8_GPIO_PIN);
		
		GPIO_SetBits(LED10_GPIO_PORT, LED10_GPIO_PIN); //CO2 CCS_RST
		GPIO_SetBits(LED11_GPIO_PORT, LED11_GPIO_PIN); //CO2 CCS_INT
	
//		GPIO_SetBits(LED13_GPIO_PORT, LED13_GPIO_PIN);//粉尘DUST_SET
//		GPIO_SetBits(LED14_GPIO_PORT, LED14_GPIO_PIN);//粉尘DUST_RES
		
//		GPIO_ResetBits(LED7_GPIO_PORT, LED7_GPIO_PIN);  //wifi失能（WiFi若想正常工作则需屏蔽）
//    GPIO_SetBits(LED15_GPIO_PORT, LED15_GPIO_PIN);
    GPIO_SetBits(LED17_GPIO_PORT, LED17_GPIO_PIN); 
}



/*********************************************END OF FILE**********************/
