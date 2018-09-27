/**
******************************************************************************
* @file    			main.c
* @author  			MLB2-ART
* @version 			V1.0
* @date    			2018-5-16
* @brief   			hermes program for fatp
* @attention  	
******************************************************************************
*/ 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
#include "tcp_demo.h"
#include "dhcp.h"
#include "bsp_i2c_ee.h"
#include "BT.h"
#include "bsp_iwdg.h" 
#include "TSL2561.h"
#include "TSL2562.h"
#include "IAP_Bootload.h"
#include "string.h"
#include "flash.h"
#include "bsp_led.h"  
#include "bsp_key.h" 
#include "bsp_adc.h"
#include "TSL2561.h"
#include "CO2.h"
#include "Team&Huy.h"
#include "esp8266.h"
#include "CO.h"
#include "BQ24259.h"
#include "PM25.h"
/************************************************/
extern int init,DHCP_flag;
extern float X_;
extern double Y_;
double socket_tx_buf[20];
int cont;
int nx,con;
extern char user_input[2048];
double tcp;
#define uchar unsigned char
/******************************************************/
char wifi_buf[100];
char user_inputcxx[]={125};
u16  USART_RX_CNT=0;			 //���յ��ֽ���
extern uint8_t MAC_ID_Buffer[30];
char UpgradeOk_code[30];
/**************����¼������豸�ı��************/
char shebei_flag[]="PM25";
char shebeiID[]="#DVT6-299";
/************************************************/
char functionType[]="A";
char IP_Adress[]="172.27.35.12";                                                                                                                          
char Mac_ID[]="34:00:8a:e1:44:2e";
char BanBen[]="V20";
char yuliu1[]="null";
char yuliu2[]="null*";
/*****************************/
char Read_IP[30];
extern char code_ssid[30];
extern char code_password[30];
extern uint8_t SSID_Buffer[30];
extern uint8_t Password_Buffer[30];
extern uint8_t Read_IP_Buffer[30];
/*****************************/
char cmd[50],cmdxx[100];
double test_data[20];
double PM25;
double DUST,dust;
double DUST_FC;
extern uint8_t smartconfig_flag_Buffer[10];
extern uint8_t Key_flag_Buffer[10];
extern char smartconfig_off;
extern unsigned int Gap;
extern unsigned int pm_ratex;
/*****************����AD��ر���***********************/
extern __IO uint16_t ADC_ConvertedValue; 
float  ADC_ConvertedValueLocal; 
void wifi_data_send(void);  //�����ϴ�
int bx;
double Nose;
float Lux;
extern long lux;
extern char OPPEN;
char led_cont,iwdg,iv,WIFI,ethernet,ethernet_cont,stop=0;
extern uint8_t Ethernet_Buffer[10];
extern u8 buff[2048*4];
long ivs;
extern uint16 len;
extern char espIP[20];
extern char MAC[30];
char hexdatax[8] = {0x42, 0x4D, 0xE3, 0x01, 0x20, 0x01, 0x72};
extern unsigned int c;
extern unsigned int filter_buf[28],x;
extern int pm25_cont,Queue_flag,flag_print;
int data_send_cont;
int gap,gap_;
long DUST_10;
float CO;
/*********************�����ϴ�����*****************************/
void data_send(void)
{
	  GPIO_SetBits(GPIOB, GPIO_Pin_0);
		socket_tx_buf[0]=dust;                   //�۳�������
	  socket_tx_buf[1]=Nose;                   //����
//		socket_tx_buf[2]=TSL2561_ReturnL1();     //����1
//		socket_tx_buf[3]=TSL2561_ReturnL2();   //����2
		socket_tx_buf[4]=Team_Return();          //�¶�
	  socket_tx_buf[5]=Huy_Return();           //ʪ��
//		socket_tx_buf[6]=0;           //CO2
//		socket_tx_buf[7]=0;           //TVO2
		socket_tx_buf[6]=CO2_Return();           //CO2
		socket_tx_buf[7]=VO2_Return();           //TVO2
    socket_tx_buf[8]=test_data[8];           //Motion1
//    socket_tx_buf[9]=test_data[9];           //Motion2
	  socket_tx_buf[9]=0;                      //Motion2

//  v18	                                     //�汾��
    socket_tx_buf[10]=PM25;                  //�۳�Ũ��
		socket_tx_buf[11]=CO_Read();             //CO
	  socket_tx_buf[12]=test_data[10];         //�̸�
//xx//xx                                     //Ԥ������
  	sprintf(cmdxx,"%s%s,%s,%s,%s,%.f,%.f,%.f,%.f,%.2f,%.2f,%.f,%.f,%.f,%.f,%s,%.f,%.f,%.f,%s,%s",shebei_flag,
		shebeiID,functionType,espIP,MAC,socket_tx_buf[0],socket_tx_buf[1],socket_tx_buf[2],socket_tx_buf[3],
		socket_tx_buf[4],socket_tx_buf[5],socket_tx_buf[6],socket_tx_buf[7],socket_tx_buf[8],socket_tx_buf[9],BanBen,
		socket_tx_buf[10],socket_tx_buf[11],socket_tx_buf[12],yuliu1,yuliu2);
//		printf("\r\nData Send:%s\r\n",cmdxx);
//		do_tcp_client();
//		if(ethernet==1)do_tcp_client();          /*TCP_Client �����ϴ�*/
//	  else wifi_usart2_send(cmdxx);
//    wifi_usart2_send(cmdxx);
    delay_ms(1000);
		wifi_send_data(cmdxx);
		memset(cmdxx,0,100);
}
void DHCP_get_IP(void)
{     
	 while(!init)
	{
		  do_dhcp();                            /*DHCP���Գ���*/
			printf(" %d",nx);
		  nx++;
		  GPIO_ResetBits(GPIOE, GPIO_Pin_8);
		  GPIO_ResetBits(GPIOE, GPIO_Pin_9);
		  GPIO_ResetBits(GPIOE, GPIO_Pin_10);
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
//		DHCP_get_IP();
		GPIO_SetBits(GPIOE, GPIO_Pin_8);GPIO_SetBits(GPIOE, GPIO_Pin_9);GPIO_SetBits(GPIOE, GPIO_Pin_10);
		GPIO_ResetBits(GPIOE, GPIO_Pin_9);
}
void pm_reset()
{
		GPIO_ResetBits(LED13_GPIO_PORT, LED13_GPIO_PIN);//�۳�DUST_SET
		GPIO_ResetBits(LED14_GPIO_PORT, LED14_GPIO_PIN);//�۳�DUST_RES
    delay_ms(500);
	  GPIO_SetBits(LED13_GPIO_PORT, LED13_GPIO_PIN);  //�۳�DUST_SET
		GPIO_SetBits(LED14_GPIO_PORT, LED14_GPIO_PIN);  //�۳�DUST_RES
}
int main(void)
{ 	
		SystemInit();
		USART1_INIT();    //debug serial port initialize		
		USART2_INIT();    //wifi_TX/RX
		USART3_INIT();    //PM2.5__TX/RX
		UART4_INIT();
		UART5_INIT();     //BT module used/CO
	  OTA_Init();
		systick_init(72); /*��ʼ��Systick����ʱ��*/
		LED_GPIO_Config();	
		EXTI_Key_Config();
		ADCx_Init();
	  printf("version is 11\r\n");
		I2C_EE_Init();	
//		BT_Init ();          //close BT                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		SPI2_FLASH_Init();    //�ⲿFLASH��ʼ��
		Flash_Read();         //��ȡAP��Password
/*************************************************************/
		printf("smart: %s\r\n",smartconfig_flag_Buffer);
		Read_smartconfig_flag();
		Flash_Read_MAC();
		Flash_Read_Serve_IP_Port();
		Read_Key_reset_flag();
		Ethernet_Read_flag();
if (Flash_Readx())        //APP������ɷ��ر�־
	 {
			sprintf(UpgradeOk_code,"%s%s%s","UpgradeOk#",MAC_ID_Buffer,"*\r\n");
			printf("%s",UpgradeOk_code);
			delay_ms(1000);
			wifi_usart2_send(UpgradeOk_code);
	 }
			Flash_Writex();
if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!= RESET)    //�豸�쳣״̬��ʾ
	 {
		   if(strstr((char*)Key_flag_Buffer,"KEY_SET")) //��ȥ�������µĸ�λ
				{
					SPI_FLASH_SectorErase(0x04000);
				}
				else
				{
					printf("IWDG RESET/system Error!!\r\n");
					for(iv=0;iv<5;iv++)flash_red_LED();	      //ϵͳ�쳣��˸��
					iwdg=1;		 
				}
	 }		 
if(strstr((char*)smartconfig_flag_Buffer,"SMART_FLAG")&&iwdg!=1)//smartconfig OK״̬��
	 {
	 	  smartconfig_OK_LED();
	 }
else if(!strstr((char*)smartconfig_flag_Buffer,"SMART_FLAG")&&iwdg!=1)//��������ָʾ��
	 {
	    kaiji_LED();
		  while(1)
			{
			  printf("\r\nwait smartconfig set\r\n");
				if(smartconfig_off){smartconfig_IRQ();smartconfig_off=0;}
				IWDG_Feed();
				delay_ms(1000);
			}
	 }
/*************************************************************/
			IWDG_Config(IWDG_Prescaler_256,625*5);	        //ι��ʱ��Ϊ20�룬��IWDG_Config(IWDG_Prescaler_64 ,625);���Ź�Ϊ1s\r\n
/*************************************************************/
//		  wifi_init((char*)SSID_Buffer,(char*)Password_Buffer);
//	    if(strstr((char*)Ethernet_Buffer,"11111111")){Ethernet_init();ethernet=1;}
//			else {wifi_init("MadeinChina","ZQWQAZ123");ethernet=0;}	 
//	    wifi_init("ZQWQAZ","12345678zqwx");
//	    Ethernet_init();
//	   	wifi_init("ARTTEST","12345678");
//	 	  wifi_init("MadeinChinaX","ZQWQAZ123");	
/******************************************************************************/
			TSL2561_INIT();       //��ʼ��TSL2561����
			TSL2561_INIT_2();     //��ʼ��TSL2561����
			CO2_INIT();           //CO2��ʼ��
/****************���ʷ۳���ʼ��**********************/
	    PM25_Init();
/*********************************************/
	while(1)                    
	{
/**********************�������ݴ�ӡ****************************************/	
			socket_tx_buf[4]=Team_Return();          //�¶�
			socket_tx_buf[5]=Huy_Return();           //ʪ��		
			socket_tx_buf[2]=TSL2561_ReturnL1();     //����1
      socket_tx_buf[11]=CO_Read();             //CO�y��
			socket_tx_buf[3]=TSL2561_ReturnL2();     //����2
			CO2_Read();              //CO2�y��
			PM25=PM25_Read();	       //�۳�����	
			dust=Dust_Read();        //�۳�Ũ��
			Nose=noise_test();       //�����y��  
			printf("Lux1:%.2f\r\n",socket_tx_buf[2]);
			printf("Lux2:%.2f\r\n",socket_tx_buf[3]);
			printf("result=%.2f,%.2f;\r\n",X_,Y_);
			printf("Teamp:%.2f\r\n",socket_tx_buf[4]);
			printf("Humi:%.2f\r\n",socket_tx_buf[5]);  
			printf("PM2.5:%.f\r\n",PM25);
			printf("dust:%.f\r\n",dust);
			printf("Noise:%.2f\r\n",Nose);
		  printf("CO:%.2f\r\n",socket_tx_buf[11]);
			Power_Status_Read();     //��ȡ��Դ״̬
/*****************Motion_TEST**************************************/		
				if(GPIO_ReadInputDataBit(LED16_GPIO_PORT,LED16_GPIO_PIN) == 1 )  
				{
					printf("Motion1=1\r\n");
					test_data[8]=1;
				}    
				else 
				{
					printf("Motion1=0\r\n");
					test_data[8]=0;			
				}
				if(GPIO_ReadInputDataBit(LED18_GPIO_PORT,LED18_GPIO_PIN) == 1 )  
				{
					printf("Motion2=0\r\n");
					test_data[9]=1;
				}
				else 
				{
					printf("Motion2=0\r\n");
					test_data[9]=0;
				}	
			  if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 1 )  
				{
					printf("Smoke:1\r\n");
					test_data[10]=1;
				}    
				else 
				{
					printf("Smoke:0\r\n");
					test_data[10]=0;
				}		
/*********************************************************************/		 
			printf("**************\r\n");
			OTA_JUMP_SCOP();       //��ת��IAP_Bootloader����
			Clear_wifi_password(); //�ƶ�������
			Modify_Server_IP();    //�ƶ��޸�Server_IP��Port
			Modify_Server_IP_USART1();
			if(smartconfig_off)
      {
//			  smartconfig_IRQ();
			  smartconfig_off=0;
			}
			memset(user_input,0,1024);
			IWDG_Feed();           //ι��
			printf("*******************************************\r\n");
/********************************************************/
				if(data_send_cont>1)
        {
				  data_send_cont=0;
//					data_send();
					printf("\r\ndata is update\r\n");//�����ϴ�
				}
				data_send_cont++;
/*********************************************************************/	
			if(ethernet==0)led_cont++;	  //wifi�����ϴ�ָʾ�Ƽ���
			if(led_cont==6&&ethernet==0&&smartconfig_off==0){led_cont=0;flash_green_LED();}	
			delay_ms(500);
	}
} 


