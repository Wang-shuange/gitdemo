#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_0;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;	    		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);	 
 GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
 GPIO_SetBits(GPIOE, GPIO_Pin_8);
 GPIO_SetBits(GPIOE, GPIO_Pin_9);	
 GPIO_SetBits(GPIOE, GPIO_Pin_10);
	
}
void flash_green_LED(void)
{
  GPIO_SetBits(GPIOE, GPIO_Pin_8);  //蓝
	GPIO_ResetBits(GPIOE, GPIO_Pin_9);//绿
	GPIO_SetBits(GPIOE, GPIO_Pin_10);  //红
  delay_ms(200);	
	GPIO_SetBits(GPIOE, GPIO_Pin_9);//红
	delay_ms(200);
}
void flash_red_LED(void)
{
  GPIO_SetBits(GPIOE, GPIO_Pin_8);  //蓝
	GPIO_SetBits(GPIOE, GPIO_Pin_9);  //绿
	GPIO_ResetBits(GPIOE, GPIO_Pin_10);//红
  delay_ms(200);	
	GPIO_SetBits(GPIOE, GPIO_Pin_10);//红
	delay_ms(200);
}
void LED_All_ON(void)
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_8);//蓝
		GPIO_ResetBits(GPIOE, GPIO_Pin_9);//绿	
    GPIO_ResetBits(GPIOE, GPIO_Pin_10);//红
}
void LED_All_OFF(void)
{
    GPIO_SetBits(GPIOE, GPIO_Pin_8);//蓝
		GPIO_SetBits(GPIOE, GPIO_Pin_9);//绿	
    GPIO_SetBits(GPIOE, GPIO_Pin_10);//红
}
void Green_Red_LED_ON(void)//橙黄
{
   GPIO_ResetBits(GPIOE, GPIO_Pin_9);//绿	
   GPIO_ResetBits(GPIOE, GPIO_Pin_10);//红
	 delay_ms(200);
	 LED_All_OFF();
	 delay_ms(200);
}
void Green_Red_LED_OFF(void)//橙黄
{
   GPIO_SetBits(GPIOE, GPIO_Pin_9);//绿	
   GPIO_SetBits(GPIOE, GPIO_Pin_10);//红
}
/*************************************************************/
void Blue_LED_ON(void)//蓝灯开
{
   GPIO_ResetBits(GPIOE, GPIO_Pin_8);//蓝
	 GPIO_SetBits(GPIOE, GPIO_Pin_9);  //绿	
   GPIO_SetBits(GPIOE, GPIO_Pin_10);  //红
}
void Blue_LED_OFF(void)//蓝灯关
{
   LED_All_OFF();      //关闭RGB
}
void Green_LED_ON(void)//绿灯开
{
   GPIO_SetBits(GPIOE, GPIO_Pin_8);  //蓝
	 GPIO_ResetBits(GPIOE, GPIO_Pin_9);//绿	
   GPIO_SetBits(GPIOE, GPIO_Pin_10);  //红
}
void Green_LED_OFF(void)//绿灯关
{
   LED_All_OFF();       //关闭RGB	
}
void Red_LED_ON(void)   //红灯开
{
   GPIO_SetBits(GPIOE, GPIO_Pin_8);  //蓝
	 GPIO_SetBits(GPIOE, GPIO_Pin_9);  //绿	
   GPIO_ResetBits(GPIOE, GPIO_Pin_10);//红
}
void Red_LED_OFF(void)//红灯关
{
   LED_All_OFF();     //关闭RGB	
} 
