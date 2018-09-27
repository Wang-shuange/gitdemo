#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"
#include "W5500_conf.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
//RGB1
#define LED1_GPIO_PORT    	GPIOE	              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO端口时钟 */
#define LED1_GPIO_PIN			  GPIO_Pin_8			        

#define LED2_GPIO_PORT    	GPIOE			              /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO端口时钟 */
#define LED2_GPIO_PIN	    	GPIO_Pin_9		        

#define    LED3_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED3_GPIO_PORT    GPIOE		   
#define    LED3_GPIO_PIN		 GPIO_Pin_10

//RGB2
//#define LED1_GPIO_PORT    	GPIOC	              /* GPIO端口 */
//#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
//#define LED1_GPIO_PIN			  GPIO_Pin_7			        

//#define LED2_GPIO_PORT    	GPIOC			              /* GPIO端口 */
//#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
//#define LED2_GPIO_PIN	    	GPIO_Pin_8		        

//#define    LED3_GPIO_CLK     RCC_APB2Periph_GPIOC
//#define    LED3_GPIO_PORT    GPIOC		   
//#define    LED3_GPIO_PIN		 GPIO_Pin_9

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

/********************************************************************/
#define    LED5_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    LED5_GPIO_PORT    GPIOD		   
#define    LED5_GPIO_PIN		 GPIO_Pin_3

#define    LED6_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    LED6_GPIO_PORT    GPIOD		   
#define    LED6_GPIO_PIN		 GPIO_Pin_5
/********************************************************/
#define    LED7_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    LED7_GPIO_PORT    GPIOA		   
#define    LED7_GPIO_PIN		 GPIO_Pin_0
#define    LEDx_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    LEDx_GPIO_PORT    GPIOA		   
#define    LEDx_GPIO_PIN		 GPIO_Pin_8
/********************************************************/

#define    LED8_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED8_GPIO_PORT    GPIOE		   
#define    LED8_GPIO_PIN		 GPIO_Pin_1

#define    LED9_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED9_GPIO_PORT    GPIOE		   
#define    LED9_GPIO_PIN		 GPIO_Pin_11

#define    LED10_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED10_GPIO_PORT    GPIOE		   
#define    LED10_GPIO_PIN		  GPIO_Pin_12

#define    LED11_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED11_GPIO_PORT    GPIOE		   
#define    LED11_GPIO_PIN		  GPIO_Pin_13

#define    LED13_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    LED13_GPIO_PORT    GPIOC		   
#define    LED13_GPIO_PIN		  GPIO_Pin_6

#define    LED14_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    LED14_GPIO_PORT    GPIOD		   
#define    LED14_GPIO_PIN		  GPIO_Pin_10

#define    LED15_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    LED15_GPIO_PORT    GPIOD		   
#define    LED15_GPIO_PIN		  GPIO_Pin_3

#define    LED16_GPIO_CLK     RCC_APB2Periph_GPIOC//Motion
#define    LED16_GPIO_PORT    GPIOC		   
#define    LED16_GPIO_PIN		  GPIO_Pin_4
#define    LED18_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    LED18_GPIO_PORT    GPIOC		   
#define    LED18_GPIO_PIN		  GPIO_Pin_5

#define    LED17_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED17_GPIO_PORT    GPIOE		   
#define    LED17_GPIO_PIN		  GPIO_Pin_4
/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
//#define ON  0
//#define OFF 1

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_3)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_0)
#define LED1_OFF		digitalHi(GPIOB,GPIO_Pin_0)
#define LED1_ON			digitalLo(GPIOB,GPIO_Pin_0)

#define LED2_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_4)
#define LED2_OFF		digitalHi(GPIOC,GPIO_Pin_4)
#define LED2_ON			digitalLo(GPIOC,GPIO_Pin_4)

#define LED3_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_3)
#define LED3_OFF		digitalHi(GPIOC,GPIO_Pin_3)
#define LED3_ON			digitalLo(GPIOC,GPIO_Pin_3)

void LED_GPIO_Config(void);
void kaiji_LED(void);
void smartconfig_LED(void);
void smartconfig_OK_LED(void);
void flash_green_LED(void);
void system_reset_LED(void);
void flash_red_LED(void);
#endif /* __LED_H */
