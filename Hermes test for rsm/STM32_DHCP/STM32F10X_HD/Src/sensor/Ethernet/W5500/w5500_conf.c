/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		配置MCU，移植W5500程序需要修改的文件，配置W5500的MAC和IP地址
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>
#include "bsp_iwdg.h" 
#include "w5500_conf.h"
#include "bsp_spi_flash.h"
#include "delay.h"
#include "w5500.h"
#include "dhcp.h"
#include "bsp_TiMbase.h"
#include "bsp_led.h"  
CONFIG_MSG  ConfigMsg;															/*配置结构体*/
II2C_MSG_STR II2C_MSG;															/*II2C存储信息结构体*/
/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
//00:08:DC:22:FA:6F ~ 00:08:DC:22:FA:A2 共52个
uint8 mac[6]={0x00,0x08,0xDC,0x22,0xFA,0xFE};//326


/*定义默认IP信息*/
uint8 local_ip[4]={192,168,1,119};											/*定义W5500默认IP地址*/
//uint8 local_ip[4]={10,199,28,152};									  /*定义W5500默认IP地址*/
uint8 subnet[4]={255,255,255,0};												/*定义W5500默认子网掩码*/
//uint8 gateway[4]={10,199,28,1};												/*定义W5500默认网关*/
uint8 gateway[4]={192,168,1,1};													/*定义W5500默认网关*/
uint8 dns_server[4]={114,114,114,114};	       					/*定义W5500默认DNS*/
//uint8 dns_server[4]={10,195,227,1};	       					  /*定义W5500默认DNS*/
uint16 local_port=5000;	                       				  /*定义本地端口*/

/*定义远端IP信息*/
//uint8  remote_ip[4]={10,199,61,250};			      		  /*远端IP地址*/
uint8  remote_ip[4]={10,199,28,156};			      		  /*远端IP地址*/
//uint8  remote_ip[4]={192,168,1,110};			      			  /*远端IP地址*/
uint16 remote_port=8443;																/*远端端口号*/

/*IP配置方法选择，请自行选择*/
//uint8	ip_from=IP_FROM_DEFINE;			
uint8	ip_from=IP_FROM_DHCP;

uint8 dhcp_ok=0;																				/*dhcp成功获取IP*/
uint32	ms=0;																						/*毫秒计数*/
uint32	dhcp_time= 0;																		/*DHCP运行计数*/
vu8	ntptimer = 0;																				/*NPT秒计数*/
extern int init,DHCP_flag;
int nx,con;
void DHCP_get_IP(void)
{ 
  Green_Red_LED_ON();	//以太網連接指示燈
	while(!init)
	{
		do_dhcp();                            /*DHCP测试程序*/
		printf(" %d",nx);
		nx++;
		IWDG_Feed();
		delay_ms(1000);
		if(nx>36)
		{
			printf("\r\nDHCP GET IP Fail,Wait Retry\r\n");
			nx=0;con++;
			if(con>2)
			{
				 printf("System Reset\r\n");
				 NVIC_SystemReset();
			}
		}	 
	}
	printf("\r\n");
	LED_All_OFF();
}
void Ethernet_init(void)
{
		gpio_for_w5500_config();	         	/*初始化MCU相关引脚*/
		reset_w5500();					            /*硬复位W5500*/
		set_w5500_mac();                    /*配置MAC地址*/
//	  set_w5500_ip();                   //自定义IP地址
		socket_buf_init(txsize, rxsize);    /*初始化8个Socket的发送接收缓存大小*/
		printf("\r\n电脑作为TCP服务器,让W5500作为 TCP客户端去连接\r\n");
		printf("服务器IP:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
		printf("监听端口:%d \r\n",remote_port);
		printf("连接成功后，服务器发送数据给W5500，W5500将返回对应数据\r\n");
		printf("应用程序执行中\r\n"); 
		printf("Please Wait about 33 second\r\n"); 
		DHCP_get_IP();
}
/**
*@brief		配置W5500的IP地址
*@param		无
*@return	无
*/
void set_w5500_ip(void)
{	        
 /*复制定义的配置信息到配置结构体*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	if(ip_from==IP_FROM_DEFINE)	
	printf(" 使用定义的IP信息配置W5500\r\n");
	/*使用II2C存储的IP参数*/	
	if(ip_from==IP_FROM_II2C)
	{
		/*从II2C中读取IP配置信息*/
		read_config_from_II2C();		
		/*如果读取II2C中MAC信息,如果已配置，则可使用*/		
		if( *(II2C_MSG.mac)==0x00&& *(II2C_MSG.mac+1)==0x08&&*(II2C_MSG.mac+2)==0xdc)		
		{
			printf(" IP from II2C\r\n");
			/*复制II2C配置信息到配置的结构体变量*/
			memcpy(ConfigMsg.lip,II2C_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,II2C_MSG.sub, 4);
			memcpy(ConfigMsg.gw, II2C_MSG.gw, 4);
		}
		else
		{
			printf(" II2C未配置,使用定义的IP信息配置W5500,并写入II2C\r\n");
			write_config_to_II2C();	/*使用默认的IP信息，并初始化II2C中数据*/
		}			
	}
	/*使用DHCP获取IP参数，需调用DHCP子函数*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		/*复制DHCP获取的配置信息到配置结构体*/
		if(dhcp_ok==1)
		{
			printf(" IP from DHCP\r\n");		 
			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
		else
		{
			printf(" XXXDHCP子程序未运行,或者不成功\r\n");
			printf(" XXXX 使用定义的IP信息配置W5500\r\n");
		}
	}
	/*以下配置信息，根据需要选用*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	
	/*将IP配置信息写入W5500相应寄存器*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	getSIPR (local_ip);			
	printf(" W5500 IP地址   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 子网掩码 : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 网关     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}
/**
*@brief		配置W5500的MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		配置W5500的GPIO接口
*@param		无
*@return	无
*/
void gpio_for_w5500_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_FLASH_Init();																			 /*初始化STM32 SPI1接口*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG, ENABLE);
	/*定义RESET引脚*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;						 /*选择要控制的GPIO引脚*/		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*设置引脚速率为50MHz*/		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*设置引脚模式为通用推挽输出*/		
	GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*调用库函数，初始化GPIO*/
	GPIO_SetBits(GPIOE, GPIO_Pin_15);		
	/*定义INT引脚*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;						 /*选择要控制的GPIO引脚*/		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*设置引脚速率为50MHz */		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*设置引脚模式为通用推挽模拟上拉输入*/	
	GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*调用库函数，初始化GPIO*/
}
/**
*@brief		W5500片选信号设置函数
*@param		val: 为“0”表示片选端口为低，为“1”表示片选端口为高
*@return	无
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
		GPIO_ResetBits(GPIOA, WIZ_SCS); 
	}
	else if (val == HIGH)
	{
		GPIO_SetBits(GPIOA, WIZ_SCS); 
	}
}
/**
*@brief		设置W5500的片选端口SCSn为低
*@param		无
*@return	无
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}
/**
*@brief		设置W5500的片选端口SCSn为高
*@param		无
*@return	无
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}
/**
*@brief		W5500复位设置函数
*@param		无
*@return	无
*/
void reset_w5500(void)
{
	#ifdef STM32F103VET6
		GPIO_ResetBits(GPIOE, WIZ_RESET);
		delay_us(2);  
		GPIO_SetBits(GPIOE, WIZ_RESET);
		delay_ms(1600);	
	#endif
}
/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_FLASH_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址
*@param   data：写入的8位数据
*@return	无
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
   iinchip_cson();                            
}
/**
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@param   data：从写入的地址处读取到的8位数据
*@return	无
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                            
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);            
   iinchip_cson();   
	 return data;    
}
/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                               
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();                           
   return len;  
}
/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  iinchip_csoff();                                
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return len;
}
/**
*@brief		写配置信息到II2C
*@param		无
*@return	无
*/
void write_config_to_II2C(void)
{
//	uint16 dAddr=0;
	delay_ms(10);																							
}
/**
*@brief		从II2C读配置信息
*@param		无
*@return	无
*/
void read_config_from_II2C(void)
{
	delay_us(10);
}
/**
*@brief		STM32定时器2初始化
*@param		无
*@return	无
*/
void timer2_init(void)
{
	TIM2_Configuration();																		/* TIM2 定时配置 */
	TIM2_NVIC_Configuration();															/* 定时器的中断优先级 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);		/* TIM2 重新开时钟，开始计时 */
}
/**
*@brief		dhcp用到的定时器初始化
*@param		无
*@return	无
*/
void dhcp_timer_init(void)
{
	timer2_init();																	
}
/**
*@brief		ntp用到的定时器初始化
*@param		无
*@return	无
*/
void ntp_timer_init(void)
{
	timer2_init();																	
}
/**
*@brief		定时器2中断函数
*@param		无
*@return	无
*/
void timer2_isr(void)
{
	ms++;	
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;																					/*DHCP定时加1S*/
		#ifndef	__NTP_H__
		ntptimer++;																						/*NTP重试时间加1S*/
		#endif
  }
}
/**
*@brief		STM32系统软复位函数
*@param		无
*@return	无
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf(" 系统重启中……\r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

