#ifndef __PM25_H
#define __PM25_H

#include "stm32f10x.h"

void USART3_INIT(void);
extern float pm_rate;
int PM25_Read(void);
long Dust_Read(void);
int Filter(void) ;
void pm25_reset_sw(void);
void pm_reset_hd(void);  
void enable_5v(void);
long DUST_Calculate(void);
void PM25_Init(void);
#endif

