#ifndef __IapBootload_H
#define __IapBootload_H

#include "stm32f10x.h"

void JumpToIAP(u32 appxaddr);
void OTA_JUMP_SCOP(void);
void OTA_Init(void);

#endif

