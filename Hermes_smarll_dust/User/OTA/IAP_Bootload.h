#ifndef __IapBootload_H
#define __IapBootload_H

#include "stm32f10x.h"

void JumpToIAP(u32 appxaddr);
void OTA_JUMP_SCOP(void);
void OTA_Init(void);
void Clear_wifi_password(void);
void Modify_Server_IP(void);
void Modify_Server_IP_USART1(void);
#endif

