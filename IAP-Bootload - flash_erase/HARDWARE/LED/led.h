#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

void LED_Init(void);//��ʼ��
void LED_GPIO_Config(void);

void flash_green_LED(void);
void system_reset_LED(void);
void LED_All_ON(void);
void LED_All_OFF(void);
void Green_Red_LED_OFF(void);
void Green_Red_LED_ON(void);
/************************************/
void Blue_LED_OFF(void); //���ƿ�
void Blue_LED_ON(void);  //���ƹ�
void Green_LED_OFF(void);//�̵ƿ�
void Green_LED_ON(void); //�̵ƹ�
void Red_LED_OFF(void);  //��ƿ�
void Red_LED_ON(void);   //��ƹ�

		 				    
#endif
