#ifndef __USER_TASK_H
#define	__USER_TASK_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"	
#ifdef __cplusplus
extern "C" {
#endif
struct __FILE { 
	int handle; 
}; 
extern  FILE __stdout;       
extern u16 usRegInputBuf[50];
//初始任务创建
#define START_TASK_PRIO		1    //任务优先级
#define START_STK_SIZE 		128  //任务堆栈大小	
extern TaskHandle_t StartTask_Handler;//任务句柄
void start_task(void *pvParameters);  //任务函数
//功能任务创建
#define TASK5_TASK_PRIO		6
#define TASK5_STK_SIZE 		128
extern  TaskHandle_t Task5Task_Handler;//配置设备参数
void    task5_task(void *pvParameters);

#define TASK1_TASK_PRIO		2
#define TASK1_STK_SIZE 		128
extern  TaskHandle_t Task1Task_Handler;//以太网数据上传
void    task1_task(void *pvParameters);

#define TASK2_TASK_PRIO		7	
#define TASK2_STK_SIZE 		128  
extern  TaskHandle_t Task2Task_Handler;//读取粉尘数据
void    task2_task(void *pvParameters);

#define TEMP_HUMI_TASK_PRIO 4
#define TEMP_HUMI_TASK_STK_SIZE 128
extern  TaskHandle_t temp_humi_Handler;//读取温湿度数据
void    temp_humi_task(void *pvParameters);

#define LED_TASK_PRIO 5
#define LED_TASK_STK_SIZE 128
extern  TaskHandle_t LED_Handler;//读取噪音及Motion数据
void    LED_task(void *pvParameters);

#define Modbus_TASK_PRIO 3
#define Modbus_TASK_STK_SIZE 128
extern  TaskHandle_t ModbusTask_Handler;//Modbus数据上传
void    Modbus_task(void *pvParameters);
/*************************/
#ifdef __cplusplus
}
#endif
#endif 
