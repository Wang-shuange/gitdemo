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
 ALIENTEKս��STM32������ʵ��47
 IAPʵ�� Bootloader V1.0 ���� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
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
void USART2_IRQHandler(void)    //���жϷ�������У�����������Ӧ�ж�ʱ����֪�����ĸ��ж�Դ�����ж�����
{	
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	OSIntEnter();    
#endif
	//  �ǲ����������б�ġ���������ʲô������������б��Ǹ���ϰ�� 
 if(!ent)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //���������ݼĴ�����
  {   
    uchar ret;		
    ret =USART_ReceiveData(USART2); 
    strcat(user_input,(const char*)&ret);		
	  }
}	
/************/
else
{
		if(USART2->SR&(1<<5))//���յ�����
	{	 
		res=USART2->DR; 
		if(USART_RX_CNT<USART_REC_LEN)
		{
				USART_RX_BUF[USART_RX_CNT]=res;
				USART_RX_CNT++;	       			 
		}		
	}
}	
	#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	OSIntExit();  											 
#endif	
} 
/******************************************************/
int main(void)
{		
			int bin=0;
			u16 oldcount=0;				//�ϵĴ��ڽ�������ֵ
			u16 applenth=0;				//���յ���app���볤��
			u16 applenthx[1];	
			u8 flag=0x00;
			__enable_irq();
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
			uart_init(115200);	//���ڳ�ʼ��Ϊ115200
			USART2_Configuration();
			delay_init();	   	 	//��ʱ��ʼ�� 
			LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
			NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000000);	
/*******************************///APP��־��Ϣ�洢
//	SPI_FLASH_Init();
//  Flash_Test();	
//	Flash_Read();       //��ȡAPP��־��Ϣ
//	printf("--->>>:%d",Flash_Read());
/*********************************************/
if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	 {
		  printf("IWDG REST\r\n");
	 }	
			IWDG_Config(IWDG_Prescaler_256 ,625*5);	//ι��ʱ��Ϊ10��
/*********************************************/
//			RTL_WIFI_Init();
			IWDG_Feed();       //ι��
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
					if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
					{					
							for(bin=0;bin<strlen((char*)USART_RX_BUF);bin++)
							{
								printf("%0x ",USART_RX_BUF[bin]);
							}		
							applenth=USART_RX_CNT;
							oldcount=0;                                 
							USART_RX_CNT=0;
							printf("�û�����������!  1\r\n");
							printf("���볤��:%dBytes  2\r\n",applenth);
							ent=0;
							wifi_send_exit("�û�����������\r\n");
							sprintf(cmdxx,"%d",applenth);
							wifi_send_exit("���볤��");
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
							wifi_send_exit("��ʼ���¹̼�...\r\n");
							printf("��ʼ���¹̼�...  3\r\n");	
							if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
							{	 
									iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
									wifi_send_exit("�̼��������!\r\n");
									printf("�̼��������!  4\r\n");		
									flag=0x02;
									count=0;
							}
							else 
							{   
									wifi_send_exit("��FLASHӦ�ó���!\r\n");
									printf("��FLASHӦ�ó���!  5\r\n");
									NVIC_SystemReset();    //ϵͳ��λ
							}
					}
					else 
					{
							wifi_send_exit("û�п��Ը��µĹ̼�!\r\n");
							printf("û�п��Ը��µĹ̼�!  6\r\n");
							NVIC_SystemReset();    //ϵͳ��λ
					}								 
			}
/****************************/	 
			if(flag==0x02)
			{				
					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
					{	 		 
							wifi_send_exit("--->Upgrade is finished,wait to runing\r\n");
							printf("--->Upgrade is finished,wait to runing\r\n");
							iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
							IWDG_Feed();           //ι��
					}
					else 
					{
							wifi_send_exit("��FLASHӦ�ó���,�޷�ִ��!\r\n");
							printf("��FLASHӦ�ó���,�޷�ִ��!  9\r\n"); 
							NVIC_SystemReset();    //ϵͳ��λ					
					}			
			}
/***************************/			
			IWDG_Feed();       //ι��
			if(xx==1)wifi_send_exit("Wait The App Update\r\n");
			xx=0;			
 }   	   
}


