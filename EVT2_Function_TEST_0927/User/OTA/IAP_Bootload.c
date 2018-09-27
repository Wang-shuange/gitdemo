#include "IAP_Bootload.h"
#include <string.h>
#include "utility.h"
#include "bsp_usart.h"
#include "flash.h"
#include "esp8266.h"
/*************************************************/
extern char MAC[30];
extern uint8_t MAC_ID_Buffer[30];
char ReadyUpgrade_code[30];
int  flag=0;
#define IAP_ADDR		0x08000000  //IAP存储地址
char user_input[2048];
char user_inputx[200];	
char IP_PASSWORD_Buffer[60];	
char Modify_IP_PASSWORD[40];
char Local_Buffer[40];
char IP_ADDR[10];
char PASSWORD[20];
extern char shebeiID[10];
char Clear_Password_Buffer[50];

uint8_t Server_IP[30];
uint8_t Server_Port[10];
int ip_cont=31,ServerLen;
char Server_Modify_OK[30];
char Wifi_Password_Clear_OK[30];
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
/**********************************************************************/
void OTA_JUMP_SCOP(void)
{	
	  printf("receive: %s\r\n",user_input);
		sprintf(ReadyUpgrade_code,"%s%s","NeedUpgrade#",MAC_ID_Buffer);
		if(strstr(user_input,ReadyUpgrade_code))
	{		
    Flash_Write();	//更新APP_FLAG标志"APP IS ERASE OK"	
		flag=0x01;
		delay_ms(1000);
		printf("------->> Jump to IAP Bootloader\r\n"); 
		wifi_send_exit("Jump to IAP Bootloader\r\n");
		memset(user_inputx,0,3);
		delay_ms(1000);
		Write_Key_reset_flag();//非看门狗复位标志
		NVIC_SystemReset();				
		JumpToIAP(IAP_ADDR);
	}		
}
/**********************************************************************/
void OTA_Init(void)
{
		__enable_irq();         //使能中断 “必须”
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8010000);  //在进入APP1的第一条语句添加FLASH地址重映射 “必须”
		SCB->VTOR = FLASH_BASE|0x8010000;  
	//		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8030000);  //在进入APP2的第一条语句添加FLASH地址重映射 “必须”
//		SCB->VTOR = FLASH_BASE|0x8030000;  
}
void Clear_wifi_password(void)
{
	sprintf(Clear_Password_Buffer,"%s%s,%s%s","HermesCommand",shebeiID,MAC_ID_Buffer,"*");
	if(strstr(user_input,Clear_Password_Buffer))
	 {
		 SPI_FLASH_SectorErase(0x00000); //清空
		 SPI_FLASH_SectorErase(0x03000); //清Smartconfig标志
		 sprintf(Wifi_Password_Clear_OK,"%s%s,%s","CLEARPW",shebeiID,"OK*");
		 delay_ms(1000);
		 wifi_send_exit(Wifi_Password_Clear_OK);
		 Write_Key_reset_flag();         //非看门狗复位标志
		 NVIC_SystemReset();		 
	 }
}
void Modify_Server_IP(void)
{
   strncpy(Modify_IP_PASSWORD,user_input,30);//存储接收到的修改指令及MAC
	 sprintf(Local_Buffer,"%s","MACID#");//用于对比的指令
   if(strstr(Modify_IP_PASSWORD,Local_Buffer))
	 {
		 while(user_input[ip_cont]!=',')
		 {
		   Server_IP[ip_cont-6]=user_input[ip_cont];
			 ip_cont++;
		 }
			 ip_cont=0;
			 ServerLen=strlen((char*)Server_IP);
			 printf("ServerLen:%d\r\n",ServerLen);
		 while(user_input[5+ServerLen+2]!='*')
		 {
		   Server_Port[ip_cont]=user_input[5+ServerLen+2];
			 ServerLen++;ip_cont++;
		 }
		 printf("Server_IP:%s\r\n",Server_IP);
		 printf("Server_Port:%s\r\n",Server_Port);
		 Flash_Write_Serve_IP_Port();
		 sprintf(Server_Modify_OK,"%s%s,%s","ChangeServer#",MAC_ID_Buffer,"OK*");
		 delay_ms(1000);
		 wifi_send_exit(Server_Modify_OK);
		 Write_Key_reset_flag();           //非看门狗复位标志
		 NVIC_SystemReset();				
	 }
}

