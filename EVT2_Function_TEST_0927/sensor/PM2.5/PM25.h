#ifndef __PM25_H
#define __PM25_H

#include "stm32f10x.h"

void USART3_INIT(void);
extern float pm_rate;
int PM25_Read(void);
float PM25_Return(void);
long Dust_Return(void);
int Filter(void) ;
//void Read_PM25(void);
#endif

