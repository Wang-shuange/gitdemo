#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "iap.h"
#include <string.h>
#include "esp8266.h"
#include "bsp_iwdg.h"   
#include "bsp_spi_flash.h"
#include "key.h"  
/************************************************
 ALIENTEK战舰STM32开发板实验47
 IAP实验 Bootloader V1.0 代码 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
#define uchar unsigned char

char user_inputx[1];	
uint8_t data[]={0};
uint8_t data_[]={0};

int ent,usart1,stop,count,Wait_cont,conter;

extern char MAC[30];
extern char MAC_ID_Buffer[30];
char ReadyUpgrade_code[30];
extern uint8_t SSID_Buffer[30];
extern uint8_t Password_Buffer[30];
extern uint8_t Read_IP_Buffer[30];
extern uint8_t Server_IP[30];
extern uint8_t Server_Port[10];

extern char Read_IP[30];

char cmdxx[20];
char cmd[400];
char user_input[2048];
void jumtoAPP_OR_Not(void);
unsigned int calculateCRC(unsigned char* _regs,unsigned int arraySize) ;
unsigned int CRC_Value,i,j;
extern uint8_t Ethernet_Buffer[10];
#define INPUT_COMMAND_LEN 100
char INPUT_COMMAND[INPUT_COMMAND_LEN];
typedef enum
{
	MODE_OTA=0,
	MODE_NORMAL
}TRANS_MODE;
TRANS_MODE usart1_mode=MODE_NORMAL,usart2_mode=MODE_NORMAL,usart3_mode=MODE_NORMAL;
/******************************************************/	
void USART3_IRQHandler(void)//串口1中断，用来接模式切换命令；//在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求
{	
		u8 res;	
	 if(usart3_mode==MODE_NORMAL) //check if ota mode
	{
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
		{   
			uchar ret;		
			ret =USART_ReceiveData(USART3); 
			strcat(INPUT_COMMAND,(const char*)&ret);		
		 }
	}	
	if(usart3_mode==MODE_OTA) //check if ota mode
	{
			if(USART3->SR&(1<<5))//接收到数据
		{	 
			res=USART3->DR; 
			if(USART_RX_CNT<USART_REC_LEN)
			{
				USART_RX_BUF[USART_RX_CNT]=res;
				USART_RX_CNT++;	       			 
			}		
		}
	}	
} 
/******************************************************/	
void USART2_IRQHandler(void)    //在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求
{	
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntEnter();    
#endif
	//  是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯 
 if(usart2_mode==MODE_NORMAL)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {   
    uchar ret;		
    ret =USART_ReceiveData(USART2); 
    strcat(user_input,(const char*)&ret);		
	  }
}	
/************/
if(usart2_mode==MODE_OTA)
{
		if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR; 
		if(USART_RX_CNT<USART_REC_LEN)
		{
				USART_RX_BUF[USART_RX_CNT]=res;
				USART_RX_CNT++;	       			 
		}		
	}
}	
	#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntExit();  											 
#endif	
} 
/******************************************************/	
void USART1_IRQHandler(void)    //在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求
{	
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntEnter();    
#endif
	//  是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯 
 if(usart1_mode==MODE_NORMAL)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {   
    uchar ret;		
    ret =USART_ReceiveData(USART1); 
    strcat(user_input,(const char*)&ret);		
	  }
}	
/************/
if(usart1_mode==MODE_OTA)
{
		if(USART1->SR&(1<<5))//接收到数据
	{	 
		res=USART1->DR; 
		if(USART_RX_CNT<USART_REC_LEN)
		{
				USART_RX_BUF[USART_RX_CNT]=res;
				USART_RX_CNT++;	       			 
		}		
	}
}	
	#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntExit();  											 
#endif	
} 
/******************************************************/
int main(void)
{	
  int count;	
	u16 oldcount=0;				//老的串口接收数据值
	u16 applenth=0;				//接收到的app代码长度
  u8 flag=0x00;
	__enable_irq();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	  //串口初始化为115200
	USART2_Configuration();
  EXTI_Key_Config();
	USART3_INIT();
	delay_init();	   	 	  //延时初始化 
 	LED_Init();		  		  //初始化与LED连接的硬件接口
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000000);	
/*******************************/
	SPI_FLASH_Init();
  Flash_Test();
	Flash_Read_AP_Pass();
	Flash_Read_Serve_IP_Port();
	Ethernet_Read_flag();
//  SPI_FLASH_SectorErase(0x01000); //擦除FLASH APP更新标志，运行IAP
//	SPI_FLASH_SectorErase(0x00000);//擦除熱點
//	SPI_FLASH_SectorErase(0x00020);//擦除密碼
//	SPI_FLASH_SectorErase(0x06000);//擦除IP
//	SPI_FLASH_SectorErase(0x07000);//擦除端口
//	SPI_FLASH_SectorErase(0x00040);
/*********************************************/
if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!= RESET)
	 {
		  printf("IWDG REST\r\n");
	 }	
	IWDG_Config(IWDG_Prescaler_256 ,625*5);	//喂狗时间为10秒
	jumtoAPP_OR_Not();//判断是否直接进入APP
//进入IAP_BootLoader指示灯
	for(count=0;count<3;count++)Green_Red_LED_ON();
	delay_ms(500);
	for(count=0;count<3;count++)Green_Red_LED_ON();
	delay_ms(500);
	for(count=0;count<3;count++)Green_Red_LED_ON();
	LED_All_OFF();
/*********************************************/
//	wifi_init((char*)SSID_Buffer,(char*)Password_Buffer);
//	wifi_init("MadeinChinaX","ZQWQAZ123");	
//	if(strstr((char*)Ethernet_Buffer,"11111111")){printf("\r\n---->>>串口升级APP\r\n");}
//	else {wifi_init("MadeinChina","ZQWQAZ123");}	
//	wifi_init("ZQWQAZ","12345678zqwx");
//	wifi_init("MadeinChina","12345678zqwx");
//	wifi_init("DEMO-X","yang20031142600");
/*********************************************/
	Flash_Write_MAC();
	IWDG_Feed();                                //喂狗
	delay_ms(100);	delay_ms(100);	delay_ms(100);
	wifi_send_exit("------>IAP Bootload er is Running\r\n");
	delay_ms(500);	delay_ms(500);	delay_ms(500); 
	printf("------>IAP Bootloader is Running\r\n");
  Wait_cont=1;  //設定while()函數循環次數
	usart1_mode=MODE_OTA;
	printf("------>Usart1 IAP Bootloader is Running\r\n");
	usart2_mode=MODE_OTA;	 
	printf("------>Usart2 IAP Bootloader is Running\r\n");
	GPIO_SetBits(GPIOB,GPIO_Pin_0);   //電平拉高發送(RS485)
	Usart_SendString( USART3,"Allow Update The Program Through The RS485\r\n");
  GPIO_ResetBits(GPIOB,GPIO_Pin_0); //電平拉低接收(RS485)
	usart3_mode=MODE_OTA;	 
	printf("------>Usart3 IAP Bootloader is Running\r\n");
//	memset(USART_RX_BUF,0,52*1024);
//	usart2_mode=MODE_OTA;
/*******************************************/
	while(1)
 {
	    delay_ms(500);delay_ms(500);delay_ms(500);	
/******************************************/	 
			if(USART_RX_CNT&&(USART_RX_BUF[100]!=0)&&(USART_RX_BUF[101]!=0))
			{
				  if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
					{						
						  applenth=USART_RX_CNT;
//						  for(i=0;i<1024;i++)printf("%0x ",USART_RX_BUF[i]);
							oldcount=0;                                  
							USART_RX_CNT=0;
							printf("用户程序接收完成!\r\n");
							printf("代码长度:%dBytes\r\n",applenth-24);
							usart1_mode=MODE_NORMAL;
						  usart2_mode=MODE_NORMAL;
						  usart3_mode=MODE_NORMAL;
							wifi_send_exit("Bin File Received Ok\r\n");
							sprintf(cmdxx,"%d",applenth-24);
							wifi_send_exit("Code Lenth");
							wifi_send_exit(cmdxx);
							wifi_send_exit("Byte\r\n");
							delay_ms(1000);
							flag=0x01;
/******************************************///CRC校验	 
//							printf("\r\n--->>>Begin The CRC Check\r\n");        				
//							CRC_Value=calculateCRC((unsigned char*)USART_RX_BUF,applenth);
//			        IWDG_Feed();       //喂狗
//						  printf("--->>>CRC_Cal:%0x\r\n",CRC_Value);
//						  CRC_Value=calculateCRC((unsigned char*)USART_RX_BUF,applenth-2);
//						  IWDG_Feed(); 
//							printf("--->>>CRC_Cal-2:%0x\r\n",CRC_Value);
//						  printf("--->>>CRC:%0x\r\n",(USART_RX_BUF[applenth-1]<<8|USART_RX_BUF[applenth-2]));
//							if((USART_RX_BUF[applenth-1]<<8|USART_RX_BUF[applenth-2])==CRC_Value)
//							{
//							  printf("--->>>Bin File Check Normal\r\n");
//							  flag=0x01;
//							}
//							else 
//							{
//                printf("--->>>Bin File Check Error\r\n");
//							}
/******************************************/	 
					}
					else oldcount=USART_RX_CNT;			
			} 
/******************************************/	 
			if(flag==0x01)	
			{
					if(applenth)
					{
							wifi_send_exit("Beigin to Upgrade\r\n");
							printf("------>Beigin to Upgrade\r\n");	
							if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
							{	 
								iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth-2);//更新FLASH代码   
								wifi_send_exit("Upgrade is finished!\r\n");
								printf("------>Upgrade is finished\r\n");		
								flag=0x02;
								count=0;
							}
							else 
							{   
								wifi_send_exit("非FLASH应用程序!\r\n");
								printf("非FLASH应用程序!  5\r\n");
								Write_Key_reset_flag();
								NVIC_SystemReset();     //系统复位
							}
					}
					else 
					{
							wifi_send_exit("没有可以更新的固件!\r\n");
							printf("没有可以更新的固件!  6\r\n");
						  Write_Key_reset_flag();
							NVIC_SystemReset();    //系统复位
					}								 
			}
/******************************************/	 
			if(flag==0x02)
			{				
						if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
						{	 		 
							  wifi_send_exit("APP wait to running\r\n");
								printf("------>APP wait to running\r\n");
							  Flash_Write();//更新APP_FLAG标志位"APP UPDATA IS OK"
							  delay_ms(100);
							  Flash_Writex();//upgrade ok标志
							  Write_Key_reset_flag();
								iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
								IWDG_Feed();       //喂狗
						}
						else 
						{
								wifi_send_exit("非FLASH应用程序,无法执行!\r\n");
								printf("非FLASH应用程序,无法执行!  9\r\n"); 
							  Flash_Write(); //更新APP_FLAG标志位"APP UPDATA IS OK"（此时重新开始运行原程序）
							  delay_ms(100);
							  Flash_Writex();//upgrade ok标志
							  Write_Key_reset_flag();
								NVIC_SystemReset();    //系统复位					
						}			
			}
/****************************///确定真正进入BootLoader循环中
			if(Wait_cont==1)        //等待接收更新文件，超過循環次數則重啟
			{
				sprintf(ReadyUpgrade_code,"%s%s%s","ReadyUpgrade#",MAC_ID_Buffer,"*\r\n");
				wifi_send_exit(ReadyUpgrade_code);
				printf("------>%s\r\n",ReadyUpgrade_code);
			  Wait_cont=0;			
			}
/****************************/
			conter++;
			if(conter==20)NVIC_SystemReset();    //系统复位	
			IWDG_Feed();       //喂狗
 }   	   
}
void jumtoAPP_OR_Not(void)//判断是否直接运行APP程序
{
			if(Flash_Read())	
			{
				printf("------>Enter The APP Program\r\n");
				if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
						{	 
							iap_load_app(FLASH_APP1_ADDR); //执行FLASH APP代码						
						}			
			}	
}
//CRC校验函数
//参数1：待校验数组的起始地址
//参数2：待校验数组的长度
//返回值CRC校验结果，16位，低字节在前
unsigned int calculateCRC(unsigned char* _regs,unsigned int arraySize) 
{
		unsigned int temp, temp2, flag;
		temp = 0xFFFF;
		for (i = 0; i < arraySize; i++)
		 {
			temp = temp ^ *(_regs+i);
		for (j = 1; j <= 8; j++)
		 {
		 flag = temp & 0x0001;
			temp >>= 1;
			 if (flag)
			temp ^= 0xA001;
			}
		 }
		temp2 = temp >> 8;
		temp = (temp << 8) | temp2;
		temp &= 0xFFFF; 
		return temp; 
}

