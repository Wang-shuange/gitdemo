#ifndef __CO_H
#define __CO_H

#include "stm32f10x.h"

void UART4_INIT(void);
void CO_Read(void);
int CO_Return(void);
unsigned char FunCheckSum(unsigned char *i,unsigned char ln);

#endif


