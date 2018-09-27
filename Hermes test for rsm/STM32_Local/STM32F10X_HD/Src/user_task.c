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
	taskENTER_CRITICAL();           //进入临界区
	//创建TASK1任务-------------用于数据上传Ethnet方式
	xTaskCreate((TaskFunction_t )task1_task,             
							(const char*    )"task1_task",           
							(uint16_t       )TASK1_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )TASK1_TASK_PRIO,        
							(TaskHandle_t*  )&Task1Task_Handler);  
  //创建TASK1任务-------------用于数据上传Modbus方式
	xTaskCreate((TaskFunction_t )Modbus_task,             
							(const char*    )"Modbus_task",           
							(uint16_t       )Modbus_TASK_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )Modbus_TASK_PRIO,        
							(TaskHandle_t*  )&ModbusTask_Handler);  
   //创建TASK2任务------------用于读取粉尘浓度PM25、粉尘颗粒数DUST
	xTaskCreate((TaskFunction_t )task2_task,     
							(const char*    )"task2_task",   
							(uint16_t       )TASK2_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )TASK2_TASK_PRIO,
							(TaskHandle_t*  )&Task2Task_Handler);
	 //创建TASK3任务------------用于读取温度、湿度、光照、电源管理CO2/TVOC
	xTaskCreate((TaskFunction_t )temp_humi_task,             
							(const char*    )"task3_task",           
							(uint16_t       )TEMP_HUMI_TASK_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )TEMP_HUMI_TASK_PRIO,        
							(TaskHandle_t*  )&temp_humi_Handler); 				
	 //创建TASK4任务------------用于读取噪音、Motion
	xTaskCreate((TaskFunction_t )LED_task,             
							(const char*    )"task4_task",           
							(uint16_t       )LED_TASK_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )LED_TASK_PRIO,        
							(TaskHandle_t*  )&LED_Handler); 
	 //创建TASK5任务------------用于修改设备ID，设备地址，Ethnet MAC
	xTaskCreate((TaskFunction_t )task5_task,             
							(const char*    )"task5_task",           
							(uint16_t       )TASK5_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )TASK5_TASK_PRIO,        
							(TaskHandle_t*  )&Task5Task_Handler); 
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}
//task1任务函数
extern void task1_task(void *pvParameters)
{
	while(1)
	{
		printf("以太网数据上传任务\r\n");
		sprintf_scop_send();   //以太网数据上传
		IWDG_Feed();
    vTaskDelay(2000);                           //延时1s，也就是1000个时钟节拍	
	}
}
//task2任务函数
void task2_task(void *pvParameters)
{
	int pm25=0,dust=0;
	while(1)
	{
//		printf("粉尘测试任务\r\n");
		gap=dust-gap_;         //计算单次粉尘增量
	  gap_=dust;  
		pm25=PM25_Read();      //粉尘浓度
		dust=Dust_Read();      //累加粉尘颗粒数
		DUST=DUST_Calculate(); //粉尘颗粒数乘10
		PM25=pm25;
		printf("pm25:%d  dust:%d  DUST*10:%ld,cont:%d,GAP:%d,GAP_Per%d\r\n",pm25,dust,DUST,pm25_cont,Gap,gap);
    vTaskDelay(800);      //延时1s，也就是1000个时钟节拍	
	}
}
//task3任务函数
void temp_humi_task(void *pvParameters)
{
	float temp=0,humi=0;
	int lux=0;
	while(1)
	{
		taskENTER_CRITICAL(); //进入临界区
//		printf("温湿度测量任务\r\n");
		temp=Team_Return();
		humi=Huy_Return();
		lux=TSL2561_ReturnL1()*1.145+46.07;
		if(lux<=46.07)lux=0;  //强制归零
		printf("temp:%.2f\r\n",temp);
		printf("humi:%.2f\r\n",humi);
		printf("xlux:%d\r\n",lux);
/*********************///用于数据上传
		Temp=temp;
		Humi=humi;
		Lux=lux;
/*********************/
		vTaskDelay(500);  
		IWDG_Feed();
		taskEXIT_CRITICAL(); //退出临界区
	}
}
//task4任务函数
void LED_task(void *pvParameters)
{
	char noise=0;
	int motion1,motion2;
	while(1)
	{
//		printf("噪音Motion测试任务\r\n");
		noise=noise_test();
		motion1=Motion1_Test();
		motion2=Motion2_Test();
		printf("Noise:%d\r\n",noise);
		printf("motion1:%d\r\n",motion1);
		printf("motion2:%d\r\n",motion2);
/*********************///用于数据上传
		Noise=noise;  
    Motion1=motion1;
		Motion2=motion2;
		IWDG_Feed();
/*********************/
		vTaskDelay(500);  
	}
}
//task5任务函数
void task5_task(void *pvParameters)
{
	while(1)
	{
		printf("HERMES_IS_READY\r\n");
		printf("参数配置任务已启动\r\n");
		read_device_addr();       //读取设备地址
		write_device_addr();      //重置设备地址
		Read_Device_Parameter();  //读取设备参数
		Write_Device_ID();        //重置设备ID
		Device_Parameter_Config();//重置设备参数
		Ethernet_test_scop();     //以太網測試
		memset(usart1_rx_buf,0,60);
    vTaskDelay(2000);         //1000延时1s，也就是1000个时钟节拍	
	}
}
//Modbus任务函数
void Modbus_task(void *pvParameters)
{
	while(1)
	{
		printf("执行Modbus数据上传任务\r\n");
    data_send();
		eMBPoll();								   //不断的解析协议栈
    vTaskDelay(500);             //1000延时1s，也就是1000个时钟节拍	
	}
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){};//循环发送,直到发送完毕   
  USART1->DR = (u8) ch;      
	return ch;
}
