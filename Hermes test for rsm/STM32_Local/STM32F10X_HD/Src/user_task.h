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
//��ʼ���񴴽�
#define START_TASK_PRIO		1    //�������ȼ�
#define START_STK_SIZE 		128  //�����ջ��С	
extern TaskHandle_t StartTask_Handler;//������
void start_task(void *pvParameters);  //������
//�������񴴽�
#define TASK5_TASK_PRIO		6
#define TASK5_STK_SIZE 		128
extern  TaskHandle_t Task5Task_Handler;//�����豸����
void    task5_task(void *pvParameters);

#define TASK1_TASK_PRIO		2
#define TASK1_STK_SIZE 		128
extern  TaskHandle_t Task1Task_Handler;//��̫�������ϴ�
void    task1_task(void *pvParameters);

#define TASK2_TASK_PRIO		7	
#define TASK2_STK_SIZE 		128  
extern  TaskHandle_t Task2Task_Handler;//��ȡ�۳�����
void    task2_task(void *pvParameters);

#define TEMP_HUMI_TASK_PRIO 4
#define TEMP_HUMI_TASK_STK_SIZE 128
extern  TaskHandle_t temp_humi_Handler;//��ȡ��ʪ������
void    temp_humi_task(void *pvParameters);

#define LED_TASK_PRIO 5
#define LED_TASK_STK_SIZE 128
extern  TaskHandle_t LED_Handler;//��ȡ������Motion����
void    LED_task(void *pvParameters);

#define Modbus_TASK_PRIO 3
#define Modbus_TASK_STK_SIZE 128
extern  TaskHandle_t ModbusTask_Handler;//Modbus�����ϴ�
void    Modbus_task(void *pvParameters);
/*************************/
#ifdef __cplusplus
}
#endif
#endif 
