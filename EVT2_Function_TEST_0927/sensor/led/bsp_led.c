/******************************************************************************
* @file    bsp_led.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   ledӦ�ú����ӿ�
*******************************************************************************/  
#include "bsp_led.h"  
#include "utility.h"
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
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK| LED5_GPIO_CLK|LED2_GPIO_CLK| LED7_GPIO_CLK|LED13_GPIO_CLK|RCC_APB2Periph_GPIOB  , ENABLE);
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);	
		//Motion���_
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;
		GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED7_GPIO_PIN|LEDx_GPIO_PIN;     //LED7 Wifiʹ�����_(A0) //LEDx   WIFI RST (A8)
		GPIO_Init(LED7_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED8_GPIO_PIN|LED9_GPIO_PIN;
		GPIO_Init(LED8_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED10_GPIO_PIN|LED11_GPIO_PIN|GPIO_Pin_3|LED17_GPIO_PIN;
		GPIO_Init(LED8_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
/********************����RGBʹ��***********************************/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
/******************Hermes����ʹ�ܵ�ѹ******************************/		
		GPIO_SetBits(LED15_GPIO_PORT, LED15_GPIO_PIN); //5V��Դʹ�ܹ���
		GPIO_SetBits(LED6_GPIO_PORT, LED6_GPIO_PIN);
		GPIO_ResetBits(LED9_GPIO_PORT, LED9_GPIO_PIN); //����INT_LIGHT
		GPIO_ResetBits(LED8_GPIO_PORT, LED8_GPIO_PIN);
		
		GPIO_SetBits(LED10_GPIO_PORT, LED10_GPIO_PIN); //CO2 CCS_RST
		GPIO_SetBits(LED11_GPIO_PORT, LED11_GPIO_PIN); //CO2 CCS_INT
		GPIO_ResetBits(GPIOE, GPIO_Pin_3);               //CO2 CCS_INT
	
		GPIO_SetBits(LED13_GPIO_PORT, LED13_GPIO_PIN);//�۳�DUST_SET
		GPIO_SetBits(LED14_GPIO_PORT, LED14_GPIO_PIN);//�۳�DUST_RES
		
		GPIO_SetBits(LED7_GPIO_PORT, LED7_GPIO_PIN);  //wifi ʹ��
//    GPIO_SetBits(LED15_GPIO_PORT, LED15_GPIO_PIN);
//    GPIO_SetBits(LED17_GPIO_PORT, LED17_GPIO_PIN); 

    GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
void kaiji_LED(void)
{
  	GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
		GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
	  delay_ms(1000);delay_s(2);
	  GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}
void smartconfig_LED(void)
{
  	int i;
	  GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
	  GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
	  for(i=0;i<5;i++)
	{
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
	  delay_ms(400);
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
		delay_ms(400);
	}
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
}
void smartconfig_OK_LED(void)
{
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
		GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
	  delay_s(6);
	  GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
		GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
	  delay_s(1);
	  GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��
}
void flash_green_LED(void)
{
  GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);  //��
	GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��
	GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);  //��
  delay_ms(500);	
	GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��
	delay_ms(500);

}
void flash_red_LED(void)
{
  GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);  //��
	GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);  //��
	GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
  delay_ms(200);	
	GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
	delay_ms(200);

}
void system_reset_LED(void)
{
  	GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
		GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
	  delay_ms(1000);delay_s(2);
	  GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}
void LED_All_ON(void)
{
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
		GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
}
void LED_All_OFF(void)
{
    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
}
void Green_Red_LED_ON(void)//�Ȼ�
{
   GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
   GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
}
void Green_Red_LED_OFF(void)//�Ȼ�
{
   GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
   GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
}
/*************************************************************/
void Blue_LED_ON(void)//���ƿ�
{
   GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);//��
	 GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);  //��	
   GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);  //��
}
void Blue_LED_OFF(void)//���ƹ�
{
   LED_All_OFF();      //�ر�RGB
}
void Green_LED_ON(void)//�̵ƿ�
{
   GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);  //��
	 GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);//��	
   GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);  //��
}
void Green_LED_OFF(void)//�̵ƹ�
{
   LED_All_OFF();       //�ر�RGB	
}
void Red_LED_ON(void)   //��ƿ�
{
   GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);  //��
	 GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);  //��	
   GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);//��
}
void Red_LED_OFF(void)//��ƹ�
{
   LED_All_OFF();     //�ر�RGB	
}
/*********************************************END OF FILE**********************/
