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
/**************����¼������豸�ı��뼰��̫��MAC************/
uint8 mac[6]={0x00,0x08,0xDC,0x22,0xFA,0x6f};//328
char shebei_flag[]="PM25";
char shebeiID[]="#DVT6-328";
/************************************************/
char functionType[]="A";
char IP_Adress[]="172.27.35.12";                                                                                                                          
char Mac[]="34:00:8a:e1:44:2e";
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
char cmd[400],cmdxx[100];
double test_data[20];
extern double PM25,DUST;
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
char hexdata[7] = {0x42, 0x4D, 0xE3, 0x00, 0x00, 0x01, 0x72};
char hexdatax[8] = {0x42, 0x4D, 0xE3, 0x01, 0x20, 0x01, 0x72};
extern unsigned int c;
extern unsigned int filter_buf[28],x;
extern int pm25_cont,Queue_flag,flag_print;
extern char key_test_flag;
char key_test_cont;
extern char ethnet_ok_flag;
extern u8 Device_Mac_Read_buf[13];
float lux1;
/*********************�����ϴ�����*****************************/
void DHCP_get_IP(void)
{     
	 while(!init)
	{
		  do_dhcp();                        /*DHCP���Գ���*/
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
	  set_w5500_ip();                     //�Զ���IP��ַ
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
	  GPIO_SetBits(LED13_GPIO_PORT, LED13_GPIO_PIN);//�۳�DUST_SET
		GPIO_SetBits(LED14_GPIO_PORT, LED14_GPIO_PIN);//�۳�DUST_RES
}
void data_send(void)
{
	  GPIO_SetBits(GPIOB, GPIO_Pin_0);
		socket_tx_buf[0]=DUST;                //�۳�������
	  socket_tx_buf[1]=Nose;                   //����
		socket_tx_buf[2]=lux1;     //����1
		socket_tx_buf[3]=0;                      //����2
		socket_tx_buf[4]=Team_Return();          //�¶�
	  socket_tx_buf[5]=Huy_Return();           //ʪ��
//		socket_tx_buf[6]=0;           //CO2
//		socket_tx_buf[7]=0;           //TVO2
		socket_tx_buf[6]=CO2_Return();           //CO2
		socket_tx_buf[7]=VO2_Return();           //TVO2
    socket_tx_buf[8]=test_data[8];                      //Motion1
    socket_tx_buf[9]=test_data[9];                      //Motion2
//  v18	                                     //�汾��
    socket_tx_buf[10]=PM25;                  //�۳�Ũ��
		socket_tx_buf[11]=0;                     //CO
	  socket_tx_buf[12]=0;                     //�̸�
//xx//xx                                     //Ԥ������
  	sprintf(cmdxx,"%s%s,%s,%s,%s,%.f,%.f,%.f,%.f,%.2f,%.2f,%.f,%.f,%.f,%.f,%s,%.f,%.f,%.f,%s,%s",shebei_flag,
		shebeiID,functionType,espIP,MAC_ID_Buffer,socket_tx_buf[0],socket_tx_buf[1],socket_tx_buf[2],socket_tx_buf[3],
		socket_tx_buf[4],socket_tx_buf[5],socket_tx_buf[6],socket_tx_buf[7],socket_tx_buf[8],socket_tx_buf[9],BanBen,
		socket_tx_buf[10],socket_tx_buf[11],socket_tx_buf[12],yuliu1,yuliu2);
		printf("\r\nData Send:%s\r\n",cmdxx);
		DUST=Nose=lux1=PM25=0;
		do_tcp_client();                  /*TCP_Client �����ϴ�*/
		memset(cmdxx,0,100);
		memset(socket_tx_buf,0,20);
		sprintf(cmdxx,"%s","\r\n--------->>>>>> Ethernet TEST OK\r\n");
		do_tcp_client();                  /*TCP_Client �����ϴ�*/
		memset(cmdxx,0,100);
}
void key_test_scop(void)//���I�yԇ
{
	char cont=10;
  if(strstr(user_input,"TEST#KEY*"))
	{
		printf("--->>>KEY TEST\r\n");
		while(cont--)delay_ms(1000);
		if(key_test_flag!=1&&key_test_flag!=3&&key_test_flag!=7)
		printf("--->>>Key TEST FAIL\r\n");
		key_test_flag=0;
		memset(user_input,0,1024);
	}
}
void Flash_test_scop(void)
{
	if(strstr(user_input,"TEST#FLASH*"))
	{
		printf("\r\n--->>>FLASH TEST\r\n");
	  Flash_Write_SSID_Password(); //FLASH TEST
	}
}
void Usart_test_scop(void)
{
	if(strstr(user_input,"TEST#USART*"))
	{
		printf("\r\n--->>>USART TEST OK\r\n");
	}
}

void BT_test_scop(void)
{
	if(strstr(user_input,"TEST#BT*"))
	{
		printf("\r\n--->>>BT TEST\r\n");
		BT_Init();  
	}
}
void WIFI_test_scop(void)
{
	if(strstr(user_input,"TEST#WIFI*"))
	{
	  printf("\r\n--->>>WIFI TEST\r\n");
		wifi_init("MadeinChina","ZQWQAZ123");
	}
}
void IIC_test_scop(void)
{
	if(strstr(user_input,"TEST#IIC*"))
	{
		printf("\r\n--->>>IIC TEST\r\n");
    Power_Status_Read();     //��ȡ��Դ״̬    
	}
}
void ETHERNET_test_scop(void)
{
	char cont=3;
	if(strstr(user_input,"TEST#ETHERNET*"))
	{
	  printf("\r\n--->>>Ethernet TEST\r\n");
		while(cont--)
    {
		  sprintf(cmdxx,"%s","\r\n--------->>>>>> Ethernet TEST OK\r\n");
		  do_tcp_client();                  /*TCP_Client �����ϴ�*/
			memset(cmdxx,0,1024);
			delay_ms(200);
		}
		if(getSn_SR(SOCK_TCPC)!=23)printf("--->>>Ethernet TEST FAIL\r\n");
		else printf("--->>>Ethernet TEST OK\r\n");
	}
}
void pm25_rest(char *Command,int size)
{
	int i;
	printf("command");
	for(i=0;i<size;++i)
	{
		printf(" %x",Command[i]);
		USART_SendData(UART4,Command[i]);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
	}
	printf("\r\n");
}			
void PM25_TEST_Read(void)
{
  	if(Gap>0&&Gap<=19)
		{
			if(flag_print==53)
			{
				PM25=PM25_Read();	       //�۳�����	
		    DUST=Dust_Return();      //�۳�Ũ��
				DUST_FC=DUST*10;
//				printf("--------->>>>>>:Dust53=%.f\r\n",DUST_FC);
				pm25_rest(hexdata,7);
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
		else 
		{
			if(flag_print==280) 
			{
				PM25=PM25_Read();	       //�۳�����	
		    DUST=Dust_Return();      //�۳�Ũ��
				DUST_FC=DUST*10;
//				printf("--------->>>>>>:Dust280=%.f\r\n",DUST_FC);
				pm25_rest(hexdata,7);
				Queue_flag=0;
				c=0;pm25_cont=0;flag_print=0;
			}
		}
}
void Sensor_Value_Read(void)
{
	char cont=3;
/**********************�������ݴ�ӡ****************************************/	
	if(strstr(user_input,"TEST#SENSOR*"))
	{
		while(cont--)
		{
			PM25=PM25_Read();	       //�۳�����	
			DUST=Dust_Return();      //�۳�Ũ��
			printf("\r\n-------->>>Sensor Value\r\n");
			printf("Temp=%.2f\r\n",socket_tx_buf[4]);
			printf("Humi=%.2f\r\n",socket_tx_buf[5]);
			CO2_Read();              //CO2�y��
			lux1=TSL2561_ReturnL1();
			printf("Lux:%.2f\r\n",lux1);
			printf("PM2.5=%.f\r\n",PM25);
			printf("Dust=%.f\r\n",DUST);
			Nose=noise_test();       //�����y��  
			printf("sound=%.2f\r\n",Nose);
	/*****************Motion_TEST**************************************/		
			if(GPIO_ReadInputDataBit(LED18_GPIO_PORT,LED18_GPIO_PIN) == 1 )  
			{
				printf("Motion1=1\r\n");
				test_data[8]=1;
			}    
			else 
			{
				printf("Motion1=0\r\n");
				test_data[8]=0;			
			}
			if(GPIO_ReadInputDataBit(LED16_GPIO_PORT,LED16_GPIO_PIN) == 1 )  
			{
				printf("Motion2=1\r\n");
				test_data[9]=1;
			}
			else 
			{
				printf("Motion2=0\r\n");
				test_data[9]=0;
			}	 
     data_send();
//		 delay_ms(1000);
	  }
	}
}
void Read_ETHERNET_MAC(void)
{
  if(strstr(user_input,"AT+READ_ETHMAC#"))
	{
		printf("--->>>Read Ethernet MAC\r\n");
    Ethernet_Read_MAC();
	}
}
void other_scop(void)
{
		printf("**************\r\n");
		OTA_JUMP_SCOP();       //��ת��IAP_Bootloader����
		Clear_wifi_password(); //�ƶ�������
		Modify_Server_IP();    //�ƶ��޸�Server_IP��Port
		if(smartconfig_off){smartconfig_IRQ();smartconfig_off=0;}
}
int main(void)
{ 	
		SystemInit();
		USART1_INIT();    //debug serial port initialize		
		USART2_INIT();    //wifi_TX/RX
		USART3_INIT();    //PM2.5__TX/RX
		UART4_INIT();
		UART5_INIT();     //BT module used
	  printf("ID:%s\r\n",shebeiID);
	  printf("--->>>USART TEST OK\r\n");
		systick_init(72); /*��ʼ��Systick����ʱ��*/
		LED_GPIO_Config();	
		EXTI_Key_Config();
		ADCx_Init();
		I2C_EE_Init();	 
		SPI2_FLASH_Init();           //�ⲿFLASH��ʼ��
	  printf("\r\n--->>>FLASH TEST\r\n");
	  Flash_Write_SSID_Password(); //FLASH TEST
		Ethernet_Read_MAC();
//		while(Device_Mac_Read_buf[5]=='0'||Device_Mac_Read_buf[5]==255)
//		{
//		  Config_Ehernet_MAC();
//			printf(">>:Please Config The Ethnet MAC\r\n");
//			delay_ms(500);
//		}
    string_to_hex_ethernet();//for eth mac transmit use
/*************************************************************/
		IWDG_Config(IWDG_Prescaler_256,625*5);	        //ι��ʱ��Ϊ20�룬��IWDG_Config(IWDG_Prescaler_64 ,625);���Ź�Ϊ1s\r\n
		TSL2561_INIT();       //��ʼ��TSL2561����
		CO2_INIT();           //CO2��ʼ��
/****************���ʷ۳���ʼ��**********************/
		pm_reset();              //�۳�sensorӲ����λ
		delay_s(10);
		IWDG_Feed();             //ι��
		USART_Cmd(UART4,ENABLE); //ʹ�ܴ���4
		pm25_rest(hexdata,7);     //�����λ
		USART_Cmd(UART4,DISABLE);//ʧ�ܴ���4
		USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
		USART_Cmd(UART4,ENABLE); //ʹ�ܴ���4
/*********************************************/
//	  printf("\r\n--->>>WIFI TEST\r\n");
//		wifi_init("MadeinChina","ZQWQAZ123");
	  printf("\r\n--->>>BT TEST\r\n");
		BT_Init();  
		printf("\r\n--->>>IIC TEST\r\n");
    Power_Status_Read();     //��ȡ��Դ״̬    
	  printf("\r\n--->>>Ethernet TEST\r\n");
		Ethernet_init();
/*********************************************/
	while(1)                    
	{
		if(ethernet==0)led_cont++;	  //wifi�����ϴ�ָʾ�Ƽ���
		if(led_cont==2&&ethernet==0&&smartconfig_off==0)
    {
		  led_cont=0;
		  flash_green_LED();
		}	
/**************���ܜyԇ����*************************/	
		key_test_scop();
		Flash_test_scop();
		BT_test_scop();
		WIFI_test_scop();
		Usart_test_scop();
		IIC_test_scop();
		ETHERNET_test_scop();
		key_test_scop();
		Sensor_Value_Read();
		Read_ETHERNET_MAC();
		Config_Ehernet_MAC();	
//    printf("<<<:%s\r\n",user_input);
//    printf("ABABABAB");		
//    if(strstr(user_input,"a_a"))				
//    printf("abababab");	
		memset(user_input,0,1024);
		IWDG_Feed();   //ι��
//			printf("Data Send:PM25#DVT6-328,A,192.168.1.110,0008DC22FA6F,47138,59,0,0,26.60,48.35,455,8,0,0,V20,28,0,0,null,null*");
		printf("Wait Receive Test Command\r\n");
		delay_s(1);
	}
} 


