#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED1_GPIO_PORT    	GPIOE	              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO端口时钟 */
#define LED1_GPIO_PIN			  GPIO_Pin_8			        

#define LED2_GPIO_PORT    	GPIOE			              /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO端口时钟 */
#define LED2_GPIO_PIN	    	GPIO_Pin_9		        

#define    LED3_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED3_GPIO_PORT    GPIOE		   
#define    LED3_GPIO_PIN		 GPIO_Pin_10
/********************************************************************/
#define    LED5_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    LED5_GPIO_PORT    GPIOC		   
#define    LED5_GPIO_PIN		 GPIO_Pin_5

#define    LED6_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    LED6_GPIO_PORT    GPIOD		   
#define    LED6_GPIO_PIN		 GPIO_Pin_5

#define    LED7_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    LED7_GPIO_PORT    GPIOA		   
#define    LED7_GPIO_PIN		 GPIO_Pin_1

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

#define    LED16_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    LED16_GPIO_PORT    GPIOC		   
#define    LED16_GPIO_PIN		  GPIO_Pin_4

#define    LED17_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LED17_GPIO_PORT    GPIOE		   
#define    LED17_GPIO_PIN		  GPIO_Pin_4
/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1
/* 使用标准的固件库控制IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态

/* 定义控制IO的宏 */
#define LED1_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED1_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED2_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */
