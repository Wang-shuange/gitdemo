/******************************************************************************
* @file    main.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   基于ST平台的新一代Hermes开发
* @attention  无
*******************************************************************************/ 
#include "stm32f10x.h"
#include "bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./i2c/bsp_i2c_ee.h"
#include "PM25.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_led.h"  
#include "bsp_key.h" 
#include "bsp_adc.h"
#include "delay.h"
#include "TSL2561.h"
#include "CO2.h"
#include "Team&Huy.h"
#include "bsp_RTL_WIFI.h"
#include "CO.h"
#include "bsp_iwdg.h" 
#include "BQ24259.h"
#include "bsp_spi_flash.h"
#include "IAP_Bootload.h"
#include "BT.h"
/******************************************************/
#define uchar unsigned char
/******************************************************/
char wifi_buf[100];
char user_inputcxx[]={125};
u16  USART_RX_CNT=0;			 //接收的字节数
extern char user_input_BT[2048];
/******************************************************/
char shebeiID[]="PM25#DVT5-1000";
char functionType[]="A";
char IP_Adress[]="172.27.35.12";
char Mac[]="34:00:8a:e1:44:2e";
char BanBen[]="V18";
char yuliu1[]="null";
char yuliu2[]="null*";
/*****************************/
extern uint8_t SSID_Buffer[50];
extern uint8_t Password_Buffer[50];
extern char Read_IP[30];
/*****************************/
char cmd[400],cmdxx[100];
double test_data[20];
uint8_t I2c_Buf_Write1x[]={0};
uint8_t I2c_Buf_Write2x[]={0x10};
/*****************定义AD相关变量***********************/
extern __IO uint16_t ADC_ConvertedValue; 
float  ADC_ConvertedValueLocal; 
void wifi_data_send(void);  //数据上传
/***********跳转至IAP_BooTLoader***********************/
void delayc(uint32_t x)
{
  while(x--);
}
/**********************************************************************
* @brief  主函数
* @param  无
* @retval 无 
***********************************************************************/
int main(void)
{ 	
			SystemInit();
			USART1_INIT();
			USART2_INIT();    //wifi_TX/RX
			USART3_INIT();    //PM2.5__TX/RX
	    UART4_INIT();
	    USART5_Config();
			delay_init();	   	//延时初始化 
			LED_GPIO_Config();	
//  	Key_GPIO_Config();
			ADCx_Init();
	    I2C_EE_Init();	
		  printf("APP is Running\r\n");
if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!= RESET)
	 {
		  printf("IWDG REST\r\n");
	 }	
			IWDG_Config(IWDG_Prescaler_256 ,625*5);	//喂狗时间为10秒
			OTA_Init();         //OTA_Init
/*************************************************************/
			SPI_FLASH_Init();
//	    SPI_FLASH_SectorErase(FLASH_SectorToErase);
		  Flash_Read();				 
if(SSID_Buffer[0]!='A'&&SSID_Buffer[0]!='T')
		{
			SmartConfig();
			SSID_Password_Read();				 
			Flash_Test();	
			IWDG_Feed(); 	
		}
		  delay_ms(500);
/*************************************************************/		
		  RTL_WIFI_Init();      //wifi_Init
			wifi_send_exit(" The App Update Ok \r\n");
        BT_Init ();
/******************************************************************************/
//			I2C_EE_PageWrite(I2c_Buf_Write1x,0xB4, 0xF4,0);	 //CO2初始化(双重)
//			I2C_EE_ByteWrite(I2c_Buf_Write2x,0xB4 , 0x01);
			  delay_ms(200);
//			TSL2561_INIT();   //初始化TSL2561光照
		
		
//			CO2_INIT();       //CO2初始化
		  printf("IP_Address: %s\r\n",Read_IP);
/******************************************************************************/
		while (1)
{
	//		  __WFI();           //睡眠/中断唤醒
//		  __WFE();           //睡眠/事件唤醒
/******************************************************************/
//		  CO2_Read();          //CO2測量
		  Team_Huy_Read();	   //温湿度测量
//      TSL2561_Read();           //光照測量		
//		  PM25_Read();	       //粉尘测量	
//      printf("%ld",Dust_Return());		
//			CO_Read();           //CO测量
//			Power_Status_Read(); //读取电源状态
//		  printf("Dust:%ld\r\n",Dust_Return());
			wifi_data_send();    //数据上传
/*****************Motion_TEST**************************************/		
//		if(GPIO_ReadInputDataBit(LED5_GPIO_PORT,LED5_GPIO_PIN) == 1 )  
//		{
//      printf(",1");
//		}
//		else printf(",0");	
//		if(GPIO_ReadInputDataBit(LED16_GPIO_PORT,LED16_GPIO_PIN) == 1 )  
//		{
//      printf(",1");
//		}
//		else printf(",0");	
    printf(";\r\n");	   	
/**************Sound_TEST(ADC)******************************************/		
//			ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 
//			printf("The Volt value = %f V \r\n",ADC_ConvertedValueLocal);		
//			 printf("********************************\r\n");
			 delay_ms(500);
			 OTA_JUMP_SCOP();   //跳转至IAP_Bootloader程序
			 IWDG_Feed();       //喂狗
  }
}
/**********数据上传函数****************************************/
void wifi_data_send(void)
{
		test_data[0]=Dust_Return();          //粉尘颗粒数
	  test_data[1]=ADC_ConvertedValueLocal;//噪音
		test_data[2]=TSL2561_ReturnL1();     //光照1
		test_data[3]=0;                      //光照2
		test_data[4]=Team_Return();          //温度
	  test_data[5]=Huy_Return();           //湿度
		test_data[6]=CO2_Return();           //CO2
		test_data[7]=VO2_Return();           //TVO2
    test_data[8]=0;                      //Motion1
    test_data[9]=0;                      //Motion2
//  v18	                                       //版本号
    test_data[10]=PM25_Return();         //粉尘浓度
		test_data[11]=CO_Return();           //CO
	  test_data[12]=0;                     //烟感
	                                       //预留两个
  	sprintf(cmdxx,"%s,%s,%s,%s,%.f,%.f,%.f,%.f,%.2f,%.2f,%.f,%.f,%.f,%.f,%s,%.f,%.f,%.f,%s,%s",
		shebeiID,functionType,Read_IP,Mac,test_data[0],test_data[1],test_data[2],test_data[3],
		test_data[4],test_data[5],test_data[6],test_data[7],test_data[8],test_data[9],BanBen,
		test_data[10],test_data[11],test_data[12],yuliu1,yuliu2);
//		printf("-----------------%s",cmdxx);
		wifi_send_exit(cmdxx);
		wifi_send_exit("\r\n");	
}


/*************************END**************************************/		


