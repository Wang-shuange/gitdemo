#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LED1_GPIO_PORT    	GPIOE	              /* GPIO�˿� */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN			  GPIO_Pin_8			        

#define LED2_GPIO_PORT    	GPIOE			              /* GPIO�˿� */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOE		/* GPIO�˿�ʱ�� */
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
/* ʹ�ñ�׼�Ĺ̼������IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬

/* �������IO�ĺ� */
#define LED1_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED1_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED2_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */
