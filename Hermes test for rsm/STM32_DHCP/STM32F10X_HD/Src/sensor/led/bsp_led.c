/******************************************************************************
* @file    bsp_led.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   ledÓ¦ÓÃº¯Êý½Ó¿Ú
*******************************************************************************/  
#include "bsp_led.h"  
#include "delay.h"
#include "bsp_usart1.h"
char motion1,motion2;
/**
* @brief  ³õÊ¼»¯¿ØÖÆLEDµÄIO
* @param  ÎÞ
* @retval ÎÞ
*/
void RGB_Motion_GPIO_Config(void)
{		
		/*¶¨ÒåÒ»¸öGPIO_InitTypeDefÀàÐÍµÄ½á¹¹Ìå*/
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE);
		GPIO_InitStructure.GPIO_Pin = motion1_GPIO_PIN|motion2_GPIO_PIN;	//motionÒý½Å
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOC , &GPIO_InitStructure);	
/////////////	
	  GPIO_InitStructure.GPIO_Pin = B_GPIO_PIN|G_GPIO_PIN|R_GPIO_PIN;	  //RGBÒý½Å
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOE , &GPIO_InitStructure);	
/********************¹«°åRGBÊ¹ÄÜ***************************/
		GPIO_SetBits(B_GPIO_PORT, B_GPIO_PIN);
		GPIO_SetBits(G_GPIO_PORT, G_GPIO_PIN);	
    GPIO_SetBits(R_GPIO_PORT, R_GPIO_PIN);
/******************MotionÒý½Å******************************/	
    GPIO_ResetBits(GPIOC, motion1_GPIO_PIN);
    GPIO_ResetBits(GPIOC, motion2_GPIO_PIN);
}
int Motion1_Test(void)
{
/*****************Motion_TEST**************************************/		
		if(GPIO_ReadInputDataBit(motion1_GPIO_PORT,motion1_GPIO_PIN) == 1 )  
		{
			motion1=1;
		}    
		else 
		{
			motion1=0;			
		}
		return motion1;
}
int Motion2_Test(void)	
{
    if(GPIO_ReadInputDataBit(motion2_GPIO_PORT,motion2_GPIO_PIN) == 1 )  
		{
			motion2=1;
		}
		else 
		{
			motion2=0;
		}	 
    return motion2;
}
/******************************************************************/
void kaiji_LED(void)
{
  	GPIO_ResetBits(GPIOE, B_GPIO_PIN);//À¶
		GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
    GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
	  delay_ms(1000);
	  GPIO_ResetBits(GPIOE, B_GPIO_PIN);
		GPIO_SetBits(GPIOE, G_GPIO_PIN);	
    GPIO_SetBits(GPIOE, R_GPIO_PIN);
}
void smartconfig_LED(void)
{
  	int i;
	  GPIO_SetBits(GPIOE, B_GPIO_PIN);//À¶
	  GPIO_SetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
	  for(i=0;i<5;i++)
	{
    GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
	  delay_ms(400);
    GPIO_SetBits(GPIOE, R_GPIO_PIN);//ºì
		delay_ms(400);
	}
    GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
}
void smartconfig_OK_LED(void)
{
    GPIO_ResetBits(GPIOE, B_GPIO_PIN);//À¶
		GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
    GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
	  delay_ms(6000);
	  GPIO_SetBits(GPIOE, B_GPIO_PIN);//À¶
		GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
    GPIO_SetBits(GPIOE, R_GPIO_PIN);//ºì
	  delay_ms(1000);
	  GPIO_SetBits(GPIOE, R_GPIO_PIN);//ºì
}
void flash_green_LED(void)
{
  GPIO_SetBits(GPIOE, B_GPIO_PIN);  //À¶
	GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ
	GPIO_SetBits(GPIOE, R_GPIO_PIN);  //ºì
  delay_ms(500);	
	GPIO_SetBits(GPIOE, G_GPIO_PIN);//ºì
	delay_ms(500);

}
void flash_red_LED(void)
{
  GPIO_SetBits(GPIOE, B_GPIO_PIN);  //À¶
	GPIO_SetBits(GPIOE, G_GPIO_PIN);  //ÂÌ
	GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
  delay_ms(200);	
	GPIO_SetBits(GPIOE, R_GPIO_PIN);//ºì
	delay_ms(200);

}
void system_reset_LED(void)
{
  	GPIO_ResetBits(GPIOE, B_GPIO_PIN);//À¶
		GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
    GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
	  delay_ms(1000);delay_ms(2000);
	  GPIO_SetBits(GPIOE, B_GPIO_PIN);
		GPIO_SetBits(GPIOE, G_GPIO_PIN);	
    GPIO_SetBits(GPIOE, R_GPIO_PIN);
}
void LED_All_ON(void)
{
    GPIO_ResetBits(GPIOE, B_GPIO_PIN);//À¶
		GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
    GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
}
void LED_All_OFF(void)
{
    GPIO_SetBits(GPIOE, B_GPIO_PIN);//À¶
		GPIO_SetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
    GPIO_SetBits(GPIOE, R_GPIO_PIN);//ºì
}
void Green_Red_LED_ON(void)//³È»Æ
{
  LED_All_OFF();
	GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
  GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
}
void Green_Red_LED_OFF(void)//³È»Æ
{
  LED_All_OFF();
  GPIO_SetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
  GPIO_SetBits(GPIOE, R_GPIO_PIN);//ºì
}
/*************************************************************/
void Blue_LED_ON(void)//À¶µÆ¿ª
{
   GPIO_ResetBits(GPIOE, B_GPIO_PIN);//À¶
	 GPIO_SetBits(GPIOE, G_GPIO_PIN);  //ÂÌ	
   GPIO_SetBits(GPIOE, R_GPIO_PIN);  //ºì
}
void Blue_LED_OFF(void)//À¶µÆ¹Ø
{
   LED_All_OFF();      //¹Ø±ÕRGB
}
void Green_LED_ON(void)//ÂÌµÆ¿ª
{
   GPIO_SetBits(GPIOE, B_GPIO_PIN);  //À¶
	 GPIO_ResetBits(GPIOE, G_GPIO_PIN);//ÂÌ	
   GPIO_SetBits(GPIOE, R_GPIO_PIN);  //ºì
}
void Green_LED_OFF(void)//ÂÌµÆ¹Ø
{
   LED_All_OFF();       //¹Ø±ÕRGB	
}
void Red_LED_ON(void)   //ºìµÆ¿ª
{
   GPIO_SetBits(GPIOE, B_GPIO_PIN);  //À¶
	 GPIO_SetBits(GPIOE, G_GPIO_PIN);  //ÂÌ	
   GPIO_ResetBits(GPIOE, R_GPIO_PIN);//ºì
}
void Red_LED_OFF(void)//ºìµÆ¹Ø
{
   LED_All_OFF();     //¹Ø±ÕRGB	
}
/*********************************************END OF FILE**********************/
