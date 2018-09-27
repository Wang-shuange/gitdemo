#include "user_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "mb.h"
#include "temp_humi.h"
#include "TSL2561.h"
#include "PM25.h"
#include "BT.h"
#include "bsp_adc.h"
#include "tcp_demo.h"
#include "bsp_led.h"  
#include "IAP_Bootload.h"
#include "bsp_key.h"  
#include "bsp_iwdg.h" 
#include "string.h"
#include "flash.h"
#include "stdlib.h"
#include "param_config.h"
#include <math.h>
/***************************************************/
extern char usart1_rx_buf[60];
extern long DUST;
int PM25;
int Lux;
float Temp,Humi;
int Noise;
int Motion1,Motion2;
extern int pm25_cont;
int gap_;
int gap;
extern unsigned int Gap; 
/***************************************************/
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //�����ٽ���
	//����TASK1����-------------���������ϴ�Ethnet��ʽ
	xTaskCreate((TaskFunction_t )task1_task,             
							(const char*    )"task1_task",           
							(uint16_t       )TASK1_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )TASK1_TASK_PRIO,        
							(TaskHandle_t*  )&Task1Task_Handler);  
  //����TASK1����-------------���������ϴ�Modbus��ʽ
	xTaskCreate((TaskFunction_t )Modbus_task,             
							(const char*    )"Modbus_task",           
							(uint16_t       )Modbus_TASK_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )Modbus_TASK_PRIO,        
							(TaskHandle_t*  )&ModbusTask_Handler);  
   //����TASK2����------------���ڶ�ȡ�۳�Ũ��PM25���۳�������DUST
	xTaskCreate((TaskFunction_t )task2_task,     
							(const char*    )"task2_task",   
							(uint16_t       )TASK2_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )TASK2_TASK_PRIO,
							(TaskHandle_t*  )&Task2Task_Handler);
	 //����TASK3����------------���ڶ�ȡ�¶ȡ�ʪ�ȡ����ա���Դ����CO2/TVOC
	xTaskCreate((TaskFunction_t )temp_humi_task,             
							(const char*    )"task3_task",           
							(uint16_t       )TEMP_HUMI_TASK_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )TEMP_HUMI_TASK_PRIO,        
							(TaskHandle_t*  )&temp_humi_Handler); 				
	 //����TASK4����------------���ڶ�ȡ������Motion
	xTaskCreate((TaskFunction_t )LED_task,             
							(const char*    )"task4_task",           
							(uint16_t       )LED_TASK_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )LED_TASK_PRIO,        
							(TaskHandle_t*  )&LED_Handler); 
	 //����TASK5����------------�����޸��豸ID���豸��ַ��Ethnet MAC
	xTaskCreate((TaskFunction_t )task5_task,             
							(const char*    )"task5_task",           
							(uint16_t       )TASK5_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )TASK5_TASK_PRIO,        
							(TaskHandle_t*  )&Task5Task_Handler); 
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
}
//task1������
extern void task1_task(void *pvParameters)
{
	while(1)
	{
		printf("��̫�������ϴ�����\r\n");
		sprintf_scop_send();   //��̫�������ϴ�
		IWDG_Feed();
    vTaskDelay(2000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}
//task2������
void task2_task(void *pvParameters)
{
	int pm25=0,dust=0;
	while(1)
	{
//		printf("�۳���������\r\n");
		gap=dust-gap_;         //���㵥�η۳�����
	  gap_=dust;  
		pm25=PM25_Read();      //�۳�Ũ��
		dust=Dust_Read();      //�ۼӷ۳�������
		DUST=DUST_Calculate(); //�۳���������10
		PM25=pm25;
		printf("pm25:%d  dust:%d  DUST*10:%ld,cont:%d,GAP:%d,GAP_Per%d\r\n",pm25,dust,DUST,pm25_cont,Gap,gap);
    vTaskDelay(800);      //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}
//task3������
void temp_humi_task(void *pvParameters)
{
	float temp=0,humi=0;
	int lux=0;
	while(1)
	{
		taskENTER_CRITICAL(); //�����ٽ���
//		printf("��ʪ�Ȳ�������\r\n");
		temp=Team_Return();
		humi=Huy_Return();
		lux=TSL2561_ReturnL1()*1.145+46.07;
		if(lux<=46.07)lux=0;  //ǿ�ƹ���
		printf("temp:%.2f\r\n",temp);
		printf("humi:%.2f\r\n",humi);
		printf("xlux:%d\r\n",lux);
/*********************///���������ϴ�
		Temp=temp;
		Humi=humi;
		Lux=lux;
/*********************/
		vTaskDelay(500);  
		IWDG_Feed();
		taskEXIT_CRITICAL(); //�˳��ٽ���
	}
}
//task4������
void LED_task(void *pvParameters)
{
	char noise=0;
	int motion1,motion2;
	while(1)
	{
//		printf("����Motion��������\r\n");
		noise=noise_test();
		motion1=Motion1_Test();
		motion2=Motion2_Test();
		printf("Noise:%d\r\n",noise);
		printf("motion1:%d\r\n",motion1);
		printf("motion2:%d\r\n",motion2);
/*********************///���������ϴ�
		Noise=noise;  
    Motion1=motion1;
		Motion2=motion2;
		IWDG_Feed();
/*********************/
		vTaskDelay(500);  
	}
}
//task5������
void task5_task(void *pvParameters)
{
	while(1)
	{
		printf("HERMES_IS_READY\r\n");
		printf("������������������\r\n");
		read_device_addr();       //��ȡ�豸��ַ
		write_device_addr();      //�����豸��ַ
		Read_Device_Parameter();  //��ȡ�豸����
		Write_Device_ID();        //�����豸ID
		Device_Parameter_Config();//�����豸����
		Ethernet_test_scop();     //��̫�W�yԇ
		memset(usart1_rx_buf,0,60);
    vTaskDelay(2000);         //1000��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}
//Modbus������
void Modbus_task(void *pvParameters)
{
	while(1)
	{
		printf("ִ��Modbus�����ϴ�����\r\n");
    data_send();
		eMBPoll();								   //���ϵĽ���Э��ջ
    vTaskDelay(500);             //1000��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){};//ѭ������,ֱ���������   
  USART1->DR = (u8) ch;      
	return ch;
}
