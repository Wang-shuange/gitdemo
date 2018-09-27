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
void USART3_IRQHandler(void)//����1�жϣ�������ģʽ�л����//���жϷ�������У�����������Ӧ�ж�ʱ����֪�����ĸ��ж�Դ�����ж�����
{	
		u8 res;	
	 if(usart3_mode==MODE_NORMAL) //check if ota mode
	{
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //���������ݼĴ�����
		{   
			uchar ret;		
			ret =USART_ReceiveData(USART3); 
			strcat(INPUT_COMMAND,(const char*)&ret);		
		 }
	}	
	if(usart3_mode==MODE_OTA) //check if ota mode
	{
			if(USART3->SR&(1<<5))//���յ�����
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
void USART2_IRQHandler(void)    //���жϷ�������У�����������Ӧ�ж�ʱ����֪�����ĸ��ж�Դ�����ж�����
{	
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	OSIntEnter();    
#endif
	//  �ǲ����������б�ġ���������ʲô������������б��Ǹ���ϰ�� 
 if(usart2_mode==MODE_NORMAL)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //���������ݼĴ�����
  {   
    uchar ret;		
    ret =USART_ReceiveData(USART2); 
    strcat(user_input,(const char*)&ret);		
	  }
}	
/************/
if(usart2_mode==MODE_OTA)
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
void USART1_IRQHandler(void)    //���жϷ�������У�����������Ӧ�ж�ʱ����֪�����ĸ��ж�Դ�����ж�����
{	
	u8 res;	
#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	OSIntEnter();    
#endif
	//  �ǲ����������б�ġ���������ʲô������������б��Ǹ���ϰ�� 
 if(usart1_mode==MODE_NORMAL)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //���������ݼĴ�����
  {   
    uchar ret;		
    ret =USART_ReceiveData(USART1); 
    strcat(user_input,(const char*)&ret);		
	  }
}	
/************/
if(usart1_mode==MODE_OTA)
{
		if(USART1->SR&(1<<5))//���յ�����
	{	 
		res=USART1->DR; 
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
  int count;	
	u16 oldcount=0;				//�ϵĴ��ڽ�������ֵ
	u16 applenth=0;				//���յ���app���볤��
  u8 flag=0x00;
	__enable_irq();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	  //���ڳ�ʼ��Ϊ115200
	USART2_Configuration();
  EXTI_Key_Config();
	USART3_INIT();
	delay_init();	   	 	  //��ʱ��ʼ�� 
 	LED_Init();		  		  //��ʼ����LED���ӵ�Ӳ���ӿ�
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000000);	
/*******************************/
	SPI_FLASH_Init();
  Flash_Test();
	Flash_Read_AP_Pass();
	Flash_Read_Serve_IP_Port();
	Ethernet_Read_flag();
//  SPI_FLASH_SectorErase(0x01000); //����FLASH APP���±�־������IAP
//	SPI_FLASH_SectorErase(0x00000);//�������c
//	SPI_FLASH_SectorErase(0x00020);//�����ܴa
//	SPI_FLASH_SectorErase(0x06000);//����IP
//	SPI_FLASH_SectorErase(0x07000);//�����˿�
//	SPI_FLASH_SectorErase(0x00040);
/*********************************************/
if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!= RESET)
	 {
		  printf("IWDG REST\r\n");
	 }	
	IWDG_Config(IWDG_Prescaler_256 ,625*5);	//ι��ʱ��Ϊ10��
	jumtoAPP_OR_Not();//�ж��Ƿ�ֱ�ӽ���APP
//����IAP_BootLoaderָʾ��
	for(count=0;count<3;count++)Green_Red_LED_ON();
	delay_ms(500);
	for(count=0;count<3;count++)Green_Red_LED_ON();
	delay_ms(500);
	for(count=0;count<3;count++)Green_Red_LED_ON();
	LED_All_OFF();
/*********************************************/
//	wifi_init((char*)SSID_Buffer,(char*)Password_Buffer);
//	wifi_init("MadeinChinaX","ZQWQAZ123");	
//	if(strstr((char*)Ethernet_Buffer,"11111111")){printf("\r\n---->>>��������APP\r\n");}
//	else {wifi_init("MadeinChina","ZQWQAZ123");}	
//	wifi_init("ZQWQAZ","12345678zqwx");
//	wifi_init("MadeinChina","12345678zqwx");
//	wifi_init("DEMO-X","yang20031142600");
/*********************************************/
	Flash_Write_MAC();
	IWDG_Feed();                                //ι��
	delay_ms(100);	delay_ms(100);	delay_ms(100);
	wifi_send_exit("------>IAP Bootload er is Running\r\n");
	delay_ms(500);	delay_ms(500);	delay_ms(500); 
	printf("------>IAP Bootloader is Running\r\n");
  Wait_cont=1;  //�O��while()����ѭ�h�Δ�
	usart1_mode=MODE_OTA;
	printf("------>Usart1 IAP Bootloader is Running\r\n");
	usart2_mode=MODE_OTA;	 
	printf("------>Usart2 IAP Bootloader is Running\r\n");
	GPIO_SetBits(GPIOB,GPIO_Pin_0);   //�ƽ���߰l��(RS485)
	Usart_SendString( USART3,"Allow Update The Program Through The RS485\r\n");
  GPIO_ResetBits(GPIOB,GPIO_Pin_0); //�ƽ���ͽ���(RS485)
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
				  if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
					{						
						  applenth=USART_RX_CNT;
//						  for(i=0;i<1024;i++)printf("%0x ",USART_RX_BUF[i]);
							oldcount=0;                                  
							USART_RX_CNT=0;
							printf("�û�����������!\r\n");
							printf("���볤��:%dBytes\r\n",applenth-24);
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
/******************************************///CRCУ��	 
//							printf("\r\n--->>>Begin The CRC Check\r\n");        				
//							CRC_Value=calculateCRC((unsigned char*)USART_RX_BUF,applenth);
//			        IWDG_Feed();       //ι��
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
							if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
							{	 
								iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth-2);//����FLASH����   
								wifi_send_exit("Upgrade is finished!\r\n");
								printf("------>Upgrade is finished\r\n");		
								flag=0x02;
								count=0;
							}
							else 
							{   
								wifi_send_exit("��FLASHӦ�ó���!\r\n");
								printf("��FLASHӦ�ó���!  5\r\n");
								Write_Key_reset_flag();
								NVIC_SystemReset();     //ϵͳ��λ
							}
					}
					else 
					{
							wifi_send_exit("û�п��Ը��µĹ̼�!\r\n");
							printf("û�п��Ը��µĹ̼�!  6\r\n");
						  Write_Key_reset_flag();
							NVIC_SystemReset();    //ϵͳ��λ
					}								 
			}
/******************************************/	 
			if(flag==0x02)
			{				
						if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
						{	 		 
							  wifi_send_exit("APP wait to running\r\n");
								printf("------>APP wait to running\r\n");
							  Flash_Write();//����APP_FLAG��־λ"APP UPDATA IS OK"
							  delay_ms(100);
							  Flash_Writex();//upgrade ok��־
							  Write_Key_reset_flag();
								iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
								IWDG_Feed();       //ι��
						}
						else 
						{
								wifi_send_exit("��FLASHӦ�ó���,�޷�ִ��!\r\n");
								printf("��FLASHӦ�ó���,�޷�ִ��!  9\r\n"); 
							  Flash_Write(); //����APP_FLAG��־λ"APP UPDATA IS OK"����ʱ���¿�ʼ����ԭ����
							  delay_ms(100);
							  Flash_Writex();//upgrade ok��־
							  Write_Key_reset_flag();
								NVIC_SystemReset();    //ϵͳ��λ					
						}			
			}
/****************************///ȷ����������BootLoaderѭ����
			if(Wait_cont==1)        //�ȴ����ո����ļ������^ѭ�h�Δ��t�؆�
			{
				sprintf(ReadyUpgrade_code,"%s%s%s","ReadyUpgrade#",MAC_ID_Buffer,"*\r\n");
				wifi_send_exit(ReadyUpgrade_code);
				printf("------>%s\r\n",ReadyUpgrade_code);
			  Wait_cont=0;			
			}
/****************************/
			conter++;
			if(conter==20)NVIC_SystemReset();    //ϵͳ��λ	
			IWDG_Feed();       //ι��
 }   	   
}
void jumtoAPP_OR_Not(void)//�ж��Ƿ�ֱ������APP����
{
			if(Flash_Read())	
			{
				printf("------>Enter The APP Program\r\n");
				if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
						{	 
							iap_load_app(FLASH_APP1_ADDR); //ִ��FLASH APP����						
						}			
			}	
}
//CRCУ�麯��
//����1����У���������ʼ��ַ
//����2����У������ĳ���
//����ֵCRCУ������16λ�����ֽ���ǰ
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

