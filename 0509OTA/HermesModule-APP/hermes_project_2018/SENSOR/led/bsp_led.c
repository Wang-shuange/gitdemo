/******************************************************************************
* @file    bsp_led.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   ledӦ�ú����ӿ�
*******************************************************************************/  
#include "bsp_led.h"   
/**
* @brief  ��ʼ������LED��IO
* @param  ��
* @retval ��
*/
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK|LED6_GPIO_CLK|LED7_GPIO_CLK|LED13_GPIO_CLK , ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);			
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
/**********************************************************/		
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
    GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;	
		GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;
		GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED7_GPIO_PIN;     //Wifiʹ�����_
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
/********************����RGBʹ��***********************************/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
/******************Hermes����ʹ�ܵ�ѹ******************************/		
//		GPIO_SetBits(LED5_GPIO_PORT, LED5_GPIO_PIN);
		GPIO_SetBits(LED6_GPIO_PORT, LED6_GPIO_PIN);
		GPIO_ResetBits(LED9_GPIO_PORT, LED9_GPIO_PIN); //����INT_LIGHT
		GPIO_ResetBits(LED8_GPIO_PORT, LED8_GPIO_PIN);
		
		GPIO_SetBits(LED10_GPIO_PORT, LED10_GPIO_PIN); //CO2 CCS_RST
		GPIO_SetBits(LED11_GPIO_PORT, LED11_GPIO_PIN); //CO2 CCS_INT
	
//		GPIO_SetBits(LED13_GPIO_PORT, LED13_GPIO_PIN);//�۳�DUST_SET
//		GPIO_SetBits(LED14_GPIO_PORT, LED14_GPIO_PIN);//�۳�DUST_RES
		
//		GPIO_ResetBits(LED7_GPIO_PORT, LED7_GPIO_PIN);  //wifiʧ�ܣ�WiFi�������������������Σ�
//    GPIO_SetBits(LED15_GPIO_PORT, LED15_GPIO_PIN);
    GPIO_SetBits(LED17_GPIO_PORT, LED17_GPIO_PIN); 
}



/*********************************************END OF FILE**********************/
