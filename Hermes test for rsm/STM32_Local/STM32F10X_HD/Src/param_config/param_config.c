#include "param_config.h"
#include "string.h"
#include "bsp_usart1.h"
#include "flash.h"
#include "FreeRTOS.h"
#include "user_task.h"
#include "task.h"
#include "delay.h"
#include "stdlib.h"
#include "bsp_led.h"  
#include "w5500_conf.h"
#include "tcp_demo.h"
#include "mb.h"
/******************************************/
u16 MAC[]={0x00,0x08,0xDC,0x22,0xFA,0xA3};
u16 device_addr=0x12;
int sw_verion=1;
char tcp_tx_buff[60];
char BUFF[100];
extern char usart1_rx_buf[60];
u8 device_addr_read_buf,device_addr_write_buf;
#define DEVICE_ADDR_LOCATION 0x08000
#define DEVICE_ID_READ_BUF_LEN 17
#define DEVICE_MAC_READ_BUF_LEN 13
#define DEVICE_ID 0x00000
#define DEVICE_MAC 0x00020
u8 Device_ID_Read_buf[DEVICE_ID_READ_BUF_LEN],Device_ID_Write_buf[]="HSE2ZK1807190004";
u8 Device_Mac_Read_buf[DEVICE_MAC_READ_BUF_LEN],Device_Mac_Write_buf[]="0008DC22FA6F";
u8 des[3]; //max value is 254
int k=0,T;
extern uint8 mac[6];// for W5500 MAC use
#define MAC_LEN 12
#define WRITE_CONFIG "AT+WRITE_DEVICE_CONF#"
#define READ_CONFIG "AT+READ_DEVICE_CONF#"
extern float Temp,Humi;
extern int Motion1,Motion2;
extern long DUST;
extern int PM25;
extern int  Lux;
extern int Noise;
extern uint8 buff[1024];/*定义一个1KB的缓存*/
char ethernet_test_buff[50]="HERMES_RECEIVE_DATA_OK";
/***************************************************/
void Delay_s(void)
{
 while(T--)
 {
   vTaskDelay(1000); 
 }
}
/***************************************************/
void string_to_hex_ethernet(void)//for eth mac transmit use
{
	 char code[MAC_LEN];//0008DC22FA6F
    int i;
		for(i=0;i<MAC_LEN;i++)
	{
		if(Device_Mac_Read_buf[i]<='9')
			code[i]=Device_Mac_Read_buf[i]-0x30; //0x30 
		else 
			code[i]=Device_Mac_Read_buf[i]-0x37;
	}
		for(i=0;i<6;i++)
		{
			mac[i]=code[2*i]<<4|code[2*i+1];
		}
}
/***************************************************/
void Device_Parameter_Config(void)//WRITE DEVICE CONFIG
{
		memset(Device_ID_Write_buf,0,DEVICE_ID_READ_BUF_LEN);
		memset(Device_Mac_Write_buf,0,DEVICE_MAC_READ_BUF_LEN);
 if(strstr(usart1_rx_buf,WRITE_CONFIG))//check user input 
 {
		strncpy((char*)Device_ID_Write_buf,usart1_rx_buf+21,16); //截取设备ID
	  strncpy((char*)Device_Mac_Write_buf,usart1_rx_buf+38,12);//截取以太网MAC
	  SPI_FLASH_SectorErase(DEVICE_ID);
		SPI_FLASH_BufferWrite(Device_ID_Write_buf, DEVICE_ID, 16);
		SPI_FLASH_BufferWrite(Device_Mac_Write_buf, DEVICE_MAC, 12);	
    vTaskDelay(1000);
	  str_sub();   //截取设备地址   
    vTaskDelay(1000);
	  SPI_FLASH_SectorErase(DEVICE_ADDR_LOCATION);
		SPI_FLASH_BufferWrite(&device_addr_write_buf, DEVICE_ADDR_LOCATION, 1);	
		memset(usart1_rx_buf,0,60);
	  printf("Write_Device_ID:%s\r\n",Device_ID_Write_buf);	
		printf("Write_Device_MAC:%s\r\n",Device_Mac_Write_buf);	
		string_to_hex_ethernet();
		NVIC_SystemReset();    //系统复位
 }
}
void Read_Device_Parameter(void)
{
	if(strstr(usart1_rx_buf,READ_CONFIG))//check user input 
	{
		printf("***************DEVICE INFO********************\r\n");
		delay_ms(50);
		read_device_addr_direct();
		SPI_FLASH_BufferRead(Device_ID_Read_buf, DEVICE_ID, 16);
		SPI_FLASH_BufferRead(Device_Mac_Read_buf, DEVICE_MAC, 12);
		printf("*  Device    ID:%s                             \r\n",Device_ID_Read_buf);
		printf("*  Device  ADDR:%d                             \r\n",device_addr_read_buf);
		printf("*  Ethernet MAC:%s                             \r\n",Device_Mac_Read_buf);
		delay_ms(50);
		vTaskSuspend(Task5Task_Handler);//挂起参数配置任务							
	}
}
void Read_Device_Parameter_Direct(void)
{
	read_device_addr_direct();
  SPI_FLASH_BufferRead(Device_ID_Read_buf, DEVICE_ID, 16);
	Device_ID();//将设备ID存入Modbus数组中
	SPI_FLASH_BufferRead(Device_Mac_Read_buf, DEVICE_MAC, 12);
	string_to_hex_ethernet();
  printf("Device    ID:%s\r\n",Device_ID_Read_buf);
	printf("Device  ADDR:%d\r\n",device_addr_read_buf);
	printf("Ethernet MAC:%s\r\n",Device_Mac_Read_buf);
}
void Device_ID(void)//将设备ID存入Modbus数组中
{
	int i;
	for(i=0;i<16;i++)
	{
		usRegInputBuf[i+28] = Device_ID_Read_buf[i];
	}
}
void Ehernet_MAC(void)
{
	int i;
	mac[0]=(Device_Mac_Read_buf[0]-30)<<8|(Device_Mac_Read_buf[1]-30);
	mac[1]=(Device_Mac_Read_buf[2]-30)<<8|(Device_Mac_Read_buf[3]-30);
	mac[2]=(Device_Mac_Read_buf[4]-30)<<8|(Device_Mac_Read_buf[5]-30);
	mac[3]=(Device_Mac_Read_buf[6]-30)<<8|(Device_Mac_Read_buf[7]-30);
	mac[4]=(Device_Mac_Read_buf[8]-30)<<8|(Device_Mac_Read_buf[9]-30);
	mac[5]=(Device_Mac_Read_buf[10]-49)<<4|(Device_Mac_Read_buf[11]-49);
	printf("MACx:");
	for(i=0;i<12;i++)printf(" %0x",Device_Mac_Read_buf[i]-48);
	printf("\r\n");
	printf("MAC:");
	for(i=0;i<6;i++)printf(" %0x",mac[i]);
	printf("\r\n");
}
/***************************************************/
void Write_Device_ID(void)
{
	if(strstr((char*)usart1_rx_buf,"AT+WRITE_DEVICE_ID")) //除去按键导致的复位
	{
		strncpy((char*)Device_ID_Write_buf,usart1_rx_buf+18,16);
		SPI_FLASH_SectorErase(DEVICE_ID);
		SPI_FLASH_BufferWrite(Device_ID_Write_buf, DEVICE_ID, 16);	
		printf("Write_Device_ID:%s\r\n",Device_ID_Write_buf);
		vTaskSuspend(Task5Task_Handler);//挂起参数配置任务							
	}
}
void Read_Device_ID(void)
{
  SPI_FLASH_BufferRead(Device_ID_Read_buf, DEVICE_ID, 16);
	SPI_FLASH_BufferRead(Device_Mac_Read_buf, DEVICE_MAC, 12);
  printf("Read_Device_ID:%s\r\n",Device_ID_Read_buf);
	printf("Read_Device_MAC:%s\r\n",Device_Mac_Read_buf);
}
void Read_Device_ID_Direct(void)
{
	if(strstr((char*)usart1_rx_buf,"AT+READ_DEVICE_ID")) //除去按键导致的复位
	{
		SPI_FLASH_BufferRead(Device_ID_Read_buf, DEVICE_ID, 16);	
		printf("Read_Device_ID:%s\r\n",Device_ID_Read_buf);
		vTaskSuspend(Task5Task_Handler);//挂起参数配置任务							
	}
}
/***************************************************/
void str_sub(void)//设备地址解析
{
	int ii;
	for(ii=51,k=0;usart1_rx_buf[ii]!=';';ii++,k++)
	{
		des[k]=usart1_rx_buf[ii]-'0';
	}
	switch(k)
	{
		case 1:
			device_addr_write_buf=des[0];
			break;
		case 2:
			device_addr_write_buf=des[0]*10+des[1];
			break;
		case 3 :
			device_addr_write_buf=des[0]*100+des[1]*10+des[2];
			break;
		default:
			break;
	}
	printf("device_addr_write_buf=%d\r\n",device_addr_write_buf);
}
void device_addr_str_sub(void)//设备地址解析
{
	int ii;
	for(ii=18,k=0;usart1_rx_buf[ii]!=';';ii++,k++)
	{
		des[k]=usart1_rx_buf[ii]-'0';
	}
	switch(k)
	{
		case 1:
			device_addr_write_buf=des[0];
			break;
		case 2:
			device_addr_write_buf=des[0]*10+des[1];
			break;
		case 3 :
			device_addr_write_buf=des[0]*100+des[1]*10+des[2];
			break;
		default:
			break;
	}
	printf("device_addr_write_buf=%d\r\n",device_addr_write_buf);
	eMBInit(MB_RTU, device_addr_write_buf, 1, 9600, MB_PAR_NONE);//初始化 FreeModbus 为RTU模式 从机地址为 Uart3 115200 无校验
	eMBEnable();		   //启动FreeModbus 
}
void read_device_addr(void)
{
	 if(strstr((char*)usart1_rx_buf,"AT+READ_DEVICE_ADDR")) 
	{
		printf("read device addr\r\n");
		SPI_FLASH_BufferRead(&device_addr_read_buf, DEVICE_ADDR_LOCATION,1);
		printf("device addr：%d\r\n",device_addr_read_buf);
		memset(usart1_rx_buf,0,60);
		vTaskSuspend(Task5Task_Handler);//挂起参数配置任务							
	}
}
void read_device_addr_direct(void)
{
		SPI_FLASH_BufferRead(&device_addr_read_buf, DEVICE_ADDR_LOCATION,1);
		memset(usart1_rx_buf,0,60);	
}
void write_device_addr(void)
{
		if(strstr((char*)usart1_rx_buf,"AT+WRITE_DEV_ADDR#")) 
		{
			printf("--->write device addr:%s\r\n",usart1_rx_buf);
			device_addr_str_sub();
			SPI_FLASH_SectorErase(DEVICE_ADDR_LOCATION);
			SPI_FLASH_BufferWrite(&device_addr_write_buf, DEVICE_ADDR_LOCATION, 1);	
			printf("HERMES_ADDR_WRITE_OK\r\n");
			memset(usart1_rx_buf,0,60);
//			NVIC_SystemReset();    //系统复位
		}
}
/***************************************************/
void int_16_2_u8(u16* data,int siz) //transmit 16bit to u8 atuthor wang shuange
{
	int i;
	for(i=1;i<siz;++i)
	{
			tcp_tx_buff[2*i-1]=usRegInputBuf[i]>>8;	
			tcp_tx_buff[2*i]=usRegInputBuf[i]&0x00ff;	
	}
}
void data_send(void)
{
	usRegInputBuf[1]=0;  //reserve;
	usRegInputBuf[2]=(u16)'A';  //func type
	usRegInputBuf[3]=192;//ip-1
	usRegInputBuf[4]=168;  //ip-2
	usRegInputBuf[5]=1;//ip-3
	usRegInputBuf[6]=15;  //ip-4
	usRegInputBuf[7]=mac[0];  //mac-1
	usRegInputBuf[8]=mac[1];  //mac-2
	usRegInputBuf[9]=mac[2];  //mac-3
	usRegInputBuf[10]=mac[3];  //mac-4
	usRegInputBuf[11]=mac[4];  //mac-5
	usRegInputBuf[12]=mac[5];  //mac-6
	usRegInputBuf[13]=DUST>>16;  //pm25 粉尘颗粒高位
	usRegInputBuf[14]=DUST&0x0000FFFF;//pm25低16位
	usRegInputBuf[15]=PM25; //粉尘浓度
	usRegInputBuf[16]=Noise;//noise
	usRegInputBuf[17]=Lux;      //光照1;//light1
	usRegInputBuf[18]=0;//light2
	usRegInputBuf[19]=Temp*100;//温度
	usRegInputBuf[20]=Humi*100;//湿度
	usRegInputBuf[21]=0;//CO2   RSM not used
	usRegInputBuf[22]=0;//TVOC  RSM not used
	usRegInputBuf[23]=Motion1;//Motion1
	usRegInputBuf[24]=Motion2;//Motion2
	usRegInputBuf[25]=0;  //CO
	usRegInputBuf[26]=0;  //烟感
	usRegInputBuf[27]=sw_verion;   //version
	//DEVICE_ID已經在Device_ID()函數中被寫入28~43位中共16位
	usRegInputBuf[44]=device_addr_read_buf; //for moudbus used
	int_16_2_u8(usRegInputBuf,44);
	flash_green_LED();      //状态指示绿灯
}
void sprintf_scop_send(void)
{
	{	
		  sprintf(BUFF,"%s,%s,%s,%s,%ld,%d,%d,%d,%.2f,%.2f,%d,%d,%d,%d,%s,%d,%d,%d,%s,%s",
		          "PM25#EVT2-18","A","10.195.145.210","00:08:DC:22:FA:98",DUST,Noise,Lux,0,
		          Temp,Humi,0,0,Motion1,Motion2,"V18",PM25,0,0,"null","null*");
		  printf(">>>send:%s\r\n",BUFF);
//			ethernet_send(BUFF);
		  memset(BUFF,0,100);
//		  do_tcp_client();
		  printf("Receive:%s\r\n",buff);
		  memset(buff,0,1024);
		}
}
void Ethernet_test_scop(void)
{
	if(strstr((char*)buff,"SERVER_SEND_DATA"))
	{
		printf("\r\nSend:%s\r\n",ethernet_test_buff);
		ethernet_send(ethernet_test_buff);
		memset(buff,0,1024);
	}
}

