#ifndef __PM25_H
#define __PM25_H

#include "stm32f10x.h"

void USART3_INIT(void);
extern float pm_rate;
int PM25_Read(void);
float PM25_Return(void);
long Dust_Return(void);
int Filter(void);
void pm25_reset_sw(void);
void pm_reset_hd(void);  
void PM25_Init(void);
long Dust_Read(void);
#endif

