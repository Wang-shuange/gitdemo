#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
//RGB1
#define   B_GPIO_PORT    	GPIOE	              
#define   B_GPIO_CLK 	    RCC_APB2Periph_GPIOE		
#define   B_GPIO_PIN			GPIO_Pin_8			        

#define   G_GPIO_PORT    	GPIOE			              
#define   G_GPIO_CLK 	    RCC_APB2Periph_GPIOE		
#define   G_GPIO_PIN	    GPIO_Pin_9		        

#define   R_GPIO_CLK      RCC_APB2Periph_GPIOE
#define   R_GPIO_PORT     GPIOE		   
#define   R_GPIO_PIN		  GPIO_Pin_10
//RGB2
//#define LED1_GPIO_PORT    GPIOC	              
//#define LED1_GPIO_CLK 	  RCC_APB2Periph_GPIOC		
//#define LED1_GPIO_PIN			GPIO_Pin_7			        

//#define LED2_GPIO_PORT    GPIOC			             
//#define LED2_GPIO_CLK 	  RCC_APB2Periph_GPIOC		
//#define LED2_GPIO_PIN	    GPIO_Pin_8		        

//#define LED3_GPIO_CLK     RCC_APB2Periph_GPIOC
//#define LED3_GPIO_PORT    GPIOC		   
//#define LED3_GPIO_PIN		  GPIO_Pin_9
//RGB3
//#define LED1_GPIO_PORT    	GPIOD	              /* GPIO端口 */
//#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOD		/* GPIO端口时钟 */
//#define LED1_GPIO_PIN			  GPIO_Pin_13			        

//#define LED2_GPIO_PORT    	GPIOD			              /* GPIO端口 */
//#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOD		/* GPIO端口时钟 */
//#define LED2_GPIO_PIN	    	GPIO_Pin_14		        

//#define    LED3_GPIO_CLK     RCC_APB2Periph_GPIOD
//#define    LED3_GPIO_PORT    GPIOD		   
//#define    LED3_GPIO_PIN		 GPIO_Pin_15
/********************************************************/
#define    motion1_GPIO_CLK     RCC_APB2Periph_GPIOC//Motion
#define    motion1_GPIO_PORT    GPIOC		   
#define    motion1_GPIO_PIN		  GPIO_Pin_4
#define    motion2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    motion2_GPIO_PORT    GPIOC		   
#define    motion2_GPIO_PIN		  GPIO_Pin_5

void RGB_Motion_GPIO_Config(void);
int Motion1_Test(void);
int Motion2_Test(void);
void kaiji_LED(void);
void smartconfig_LED(void);
void smartconfig_OK_LED(void);
void flash_green_LED(void);
void system_reset_LED(void);
void flash_red_LED(void);
void LED_All_ON(void);
void LED_All_OFF(void);
void Green_Red_LED_OFF(void);
void Green_Red_LED_ON(void);
/************************************/
void Blue_LED_OFF(void); //蓝灯开
void Blue_LED_ON(void);  //蓝灯关
void Green_LED_OFF(void);//绿灯开
void Green_LED_ON(void); //绿灯关
void Red_LED_OFF(void);  //红灯开
void Red_LED_ON(void);   //红灯关


#endif /* __LED_H */
