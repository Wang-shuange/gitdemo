#include "IAP_Bootload.h"
#include <string.h>
#include "delay.h"
#include "./usart/bsp_usart.h"
#include "bsp_RTL_WIFI.h"

int  flag=0;
#define IAP_ADDR		0x8000000  //IAP存储地址
char user_input[2048];
char user_inputx[200];	
/**********************************************************************/
void JumpToIAP(u32 appxaddr)
{
    u32 IapSpInitVal;
    u32 IapJumpAddr;
    void (*pIapFun)(void);
    RCC_DeInit();
    __disable_irq();
    IapSpInitVal = *(vu32*)IAP_ADDR;
		IapJumpAddr = *(vu32*)(IAP_ADDR + 4);
    __set_CONTROL(0);
    __set_MSP (IapSpInitVal);
    pIapFun = (void (*)(void))IapJumpAddr;
    (*pIapFun) ();
}
void OTA_JUMP_SCOP(void)
{
	if(strstr(user_input,"#APP_UPGRADE*"))
	{		
		flag=0x01;
		delay_ms(1000);
		printf("------->> Jump to IAP Bootloader\r\n"); 
		wifi_send_exit("Jump to IAP Bootloader\r\n");
		memset(user_inputx,0,3);
		delay_ms(1000);
		JumpToIAP(IAP_ADDR);	
	}			
}
void OTA_Init(void)
{
		__enable_irq();         //使能中断 “必须”
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8010000);  //在进入APP的第一条语句添加FLASH地址重映射 “必须”
		SCB->VTOR = FLASH_BASE|0x8010000;  
}


