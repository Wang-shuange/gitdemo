#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "iap.h"
#include <string.h>
#include "bsp_RTL_WIFI.h"
#include "bsp_iwdg.h"   
//#include "bsp_spi_flash.h"
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
/***********************/
int ent,stop,count,xx;
u8 zancun[22*1024];
/***********************/
char cmd[400],cmdxx[20];
char user_input[2048];
/******************************************************/	
void USART2_IRQHandler(void)    //在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求
{	
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	OSIntEnter();    
#endif
	//  是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯 
 if(!ent)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {   
    uchar ret;		
    ret =USART_ReceiveData(USART2); 
    strcat(user_input,(const char*)&ret);		
	  }
}	
/************/
else
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
int main(void)
{		
			int bin=0;
			u16 oldcount=0;				//老的串口接收数据值
			u16 applenth=0;				//接收到的app代码长度
			u16 applenthx[1];	
			u8 flag=0x00;
			__enable_irq();
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
			uart_init(115200);	//串口初始化为115200
			USART2_Configuration();
			delay_init();	   	 	//延时初始化 
			LED_Init();		  		//初始化与LED连接的硬件接口
			NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000000);	
/*******************************///APP标志信息存储
//	SPI_FLASH_Init();
//  Flash_Test();	
//	Flash_Read();       //读取APP标志信息
//	printf("--->>>:%d",Flash_Read());
/*********************************************/
if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	 {
		  printf("IWDG REST\r\n");
	 }	
			IWDG_Config(IWDG_Prescaler_256 ,625*5);	//喂狗时间为10秒
/*********************************************/
//			RTL_WIFI_Init();
			IWDG_Feed();       //喂狗
			delay_ms(100);	delay_ms(100);	delay_ms(100);
			wifi_send_exit("------>IAP Bootloader is Running\r\n");
			delay_ms(500);	delay_ms(500);	delay_ms(500);
			printf("------>IAP Bootloader is Running\r\n");
			ent=1;xx=1;
/*********************************************/
	while(1)
 {
	    delay_ms(500);delay_ms(500);delay_ms(500);	
/****************************/	 
			if(USART_RX_CNT&&(USART_RX_BUF[100]!=0)&&(USART_RX_BUF[101]!=0))
			{
					if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
					{					
							for(bin=0;bin<strlen((char*)USART_RX_BUF);bin++)
							{
								printf("%0x ",USART_RX_BUF[bin]);
							}		
							applenth=USART_RX_CNT;
							oldcount=0;                                 
							USART_RX_CNT=0;
							printf("用户程序接收完成!  1\r\n");
							printf("代码长度:%dBytes  2\r\n",applenth);
							ent=0;
							wifi_send_exit("用户程序接收完成\r\n");
							sprintf(cmdxx,"%d",applenth);
							wifi_send_exit("代码长度");
							wifi_send_exit(cmdxx);
							wifi_send_exit("Byte\r\n");
							delay_ms(1000);
							flag=0x01;
					}
					else oldcount=USART_RX_CNT;			
			} 
/****************************/	 
			if(flag==0x01)	
			{
					if(applenthx[0])
					{
							wifi_send_exit("开始更新固件...\r\n");
							printf("开始更新固件...  3\r\n");	
							if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
							{	 
									iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
									wifi_send_exit("固件更新完成!\r\n");
									printf("固件更新完成!  4\r\n");		
									flag=0x02;
									count=0;
							}
							else 
							{   
									wifi_send_exit("非FLASH应用程序!\r\n");
									printf("非FLASH应用程序!  5\r\n");
									NVIC_SystemReset();    //系统复位
							}
					}
					else 
					{
							wifi_send_exit("没有可以更新的固件!\r\n");
							printf("没有可以更新的固件!  6\r\n");
							NVIC_SystemReset();    //系统复位
					}								 
			}
/****************************/	 
			if(flag==0x02)
			{				
					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
					{	 		 
							wifi_send_exit("--->Upgrade is finished,wait to runing\r\n");
							printf("--->Upgrade is finished,wait to runing\r\n");
							iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
							IWDG_Feed();           //喂狗
					}
					else 
					{
							wifi_send_exit("非FLASH应用程序,无法执行!\r\n");
							printf("非FLASH应用程序,无法执行!  9\r\n"); 
							NVIC_SystemReset();    //系统复位					
					}			
			}
/***************************/			
			IWDG_Feed();       //喂狗
			if(xx==1)wifi_send_exit("Wait The App Update\r\n");
			xx=0;			
 }   	   
}


