/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
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
CONFIG_MSG  ConfigMsg;															/*���ýṹ��*/
II2C_MSG_STR II2C_MSG;															/*II2C�洢��Ϣ�ṹ��*/
/*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
//00:08:DC:22:FA:6F ~ 00:08:DC:22:FA:A2 ��52��
uint8 mac[6]={0x00,0x08,0xDC,0x22,0xFA,0xFE};//326


/*����Ĭ��IP��Ϣ*/
uint8 local_ip[4]={192,168,1,119};											/*����W5500Ĭ��IP��ַ*/
//uint8 local_ip[4]={10,199,28,152};									  /*����W5500Ĭ��IP��ַ*/
uint8 subnet[4]={255,255,255,0};												/*����W5500Ĭ����������*/
//uint8 gateway[4]={10,199,28,1};												/*����W5500Ĭ������*/
uint8 gateway[4]={192,168,1,1};													/*����W5500Ĭ������*/
uint8 dns_server[4]={114,114,114,114};	       					/*����W5500Ĭ��DNS*/
//uint8 dns_server[4]={10,195,227,1};	       					  /*����W5500Ĭ��DNS*/
uint16 local_port=5000;	                       				  /*���屾�ض˿�*/

/*����Զ��IP��Ϣ*/
//uint8  remote_ip[4]={10,199,61,250};			      		  /*Զ��IP��ַ*/
uint8  remote_ip[4]={10,199,28,156};			      		  /*Զ��IP��ַ*/
//uint8  remote_ip[4]={192,168,1,110};			      			  /*Զ��IP��ַ*/
uint16 remote_port=8443;																/*Զ�˶˿ں�*/

/*IP���÷���ѡ��������ѡ��*/
//uint8	ip_from=IP_FROM_DEFINE;			
uint8	ip_from=IP_FROM_DHCP;

uint8 dhcp_ok=0;																				/*dhcp�ɹ���ȡIP*/
uint32	ms=0;																						/*�������*/
uint32	dhcp_time= 0;																		/*DHCP���м���*/
vu8	ntptimer = 0;																				/*NPT�����*/
extern int init,DHCP_flag;
int nx,con;
void DHCP_get_IP(void)
{ 
  Green_Red_LED_ON();	//��̫�W�B��ָʾ��
	while(!init)
	{
		do_dhcp();                            /*DHCP���Գ���*/
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
		gpio_for_w5500_config();	         	/*��ʼ��MCU�������*/
		reset_w5500();					            /*Ӳ��λW5500*/
		set_w5500_mac();                    /*����MAC��ַ*/
//	  set_w5500_ip();                   //�Զ���IP��ַ
		socket_buf_init(txsize, rxsize);    /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
		printf("\r\n������ΪTCP������,��W5500��Ϊ TCP�ͻ���ȥ����\r\n");
		printf("������IP:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
		printf("�����˿�:%d \r\n",remote_port);
		printf("���ӳɹ��󣬷������������ݸ�W5500��W5500�����ض�Ӧ����\r\n");
		printf("Ӧ�ó���ִ����\r\n"); 
		printf("Please Wait about 33 second\r\n"); 
		DHCP_get_IP();
}
/**
*@brief		����W5500��IP��ַ
*@param		��
*@return	��
*/
void set_w5500_ip(void)
{	        
 /*���ƶ����������Ϣ�����ýṹ��*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	if(ip_from==IP_FROM_DEFINE)	
	printf(" ʹ�ö����IP��Ϣ����W5500\r\n");
	/*ʹ��II2C�洢��IP����*/	
	if(ip_from==IP_FROM_II2C)
	{
		/*��II2C�ж�ȡIP������Ϣ*/
		read_config_from_II2C();		
		/*�����ȡII2C��MAC��Ϣ,��������ã����ʹ��*/		
		if( *(II2C_MSG.mac)==0x00&& *(II2C_MSG.mac+1)==0x08&&*(II2C_MSG.mac+2)==0xdc)		
		{
			printf(" IP from II2C\r\n");
			/*����II2C������Ϣ�����õĽṹ�����*/
			memcpy(ConfigMsg.lip,II2C_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,II2C_MSG.sub, 4);
			memcpy(ConfigMsg.gw, II2C_MSG.gw, 4);
		}
		else
		{
			printf(" II2Cδ����,ʹ�ö����IP��Ϣ����W5500,��д��II2C\r\n");
			write_config_to_II2C();	/*ʹ��Ĭ�ϵ�IP��Ϣ������ʼ��II2C������*/
		}			
	}
	/*ʹ��DHCP��ȡIP�����������DHCP�Ӻ���*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		/*����DHCP��ȡ��������Ϣ�����ýṹ��*/
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
			printf(" XXXDHCP�ӳ���δ����,���߲��ɹ�\r\n");
			printf(" XXXX ʹ�ö����IP��Ϣ����W5500\r\n");
		}
	}
	/*����������Ϣ��������Ҫѡ��*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	
	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	getSIPR (local_ip);			
	printf(" W5500 IP��ַ   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 �������� : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 ����     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}
/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void gpio_for_w5500_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_FLASH_Init();																			 /*��ʼ��STM32 SPI1�ӿ�*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG, ENABLE);
	/*����RESET����*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;						 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz*/		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*��������ģʽΪͨ���������*/		
	GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
	GPIO_SetBits(GPIOE, GPIO_Pin_15);		
	/*����INT����*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;						 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz */		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*��������ģʽΪͨ������ģ����������*/	
	GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
}
/**
*@brief		W5500Ƭѡ�ź����ú���
*@param		val: Ϊ��0����ʾƬѡ�˿�Ϊ�ͣ�Ϊ��1����ʾƬѡ�˿�Ϊ��
*@return	��
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
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}
/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}
/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
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
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_FLASH_SendByte(dat));
}

/**
*@brief		д��һ��8λ���ݵ�W5500
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data��д���8λ����
*@return	��
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
*@brief		��W5500����һ��8λ����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data����д��ĵ�ַ����ȡ����8λ����
*@return	��
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
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
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
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
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
*@brief		д������Ϣ��II2C
*@param		��
*@return	��
*/
void write_config_to_II2C(void)
{
//	uint16 dAddr=0;
	delay_ms(10);																							
}
/**
*@brief		��II2C��������Ϣ
*@param		��
*@return	��
*/
void read_config_from_II2C(void)
{
	delay_us(10);
}
/**
*@brief		STM32��ʱ��2��ʼ��
*@param		��
*@return	��
*/
void timer2_init(void)
{
	TIM2_Configuration();																		/* TIM2 ��ʱ���� */
	TIM2_NVIC_Configuration();															/* ��ʱ�����ж����ȼ� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);		/* TIM2 ���¿�ʱ�ӣ���ʼ��ʱ */
}
/**
*@brief		dhcp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void dhcp_timer_init(void)
{
	timer2_init();																	
}
/**
*@brief		ntp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void ntp_timer_init(void)
{
	timer2_init();																	
}
/**
*@brief		��ʱ��2�жϺ���
*@param		��
*@return	��
*/
void timer2_isr(void)
{
	ms++;	
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;																					/*DHCP��ʱ��1S*/
		#ifndef	__NTP_H__
		ntptimer++;																						/*NTP����ʱ���1S*/
		#endif
  }
}
/**
*@brief		STM32ϵͳ��λ����
*@param		��
*@return	��
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  printf(" ϵͳ�����С���\r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

