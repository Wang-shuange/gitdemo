/***********************************************/
/*  Project��hermes for modbus 485             */
/*   Author��wangshuange                       */
/*     Date��2018-6-13                           */
/*    Brief��add modbus  protocal               */
/***********************************************/
/* Includes ------------------------------------------------------------------*/
#include "user_task.h"
#include "stm32f10x.h"
#include "mb.h"
#include "TSL2561.h"
#include "bsp_i2c_ee.h"
#include "bsp_usart1.h"
#include "temp_humi.h"
#include "delay.h"
#include "pm25.h"
#include "bsp_adc.h"
#include "BT.h"
#include "bsp_iwdg.h" 
#include "bsp_led.h"  
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "tcp_demo.h"
#include "dhcp.h"
#include "IAP_Bootload.h"
#include "bsp_key.h"  
#include "flash.h"
#include "param_config.h"
FILE __stdout;  
//������
TaskHandle_t StartTask_Handler;
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;
TaskHandle_t temp_humi_Handler;
TaskHandle_t LED_Handler;
TaskHandle_t Task5Task_Handler;
TaskHandle_t ModbusTask_Handler;
extern u16 device_addr;
extern u8 device_addr_read_buf;
extern int iic_error_flag;
#define DEVICE_ADDR_LOCATION 0x08000   
/****************************************/	
int main()
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	uart_init(115200);	     //���ڳ�ʼ��Ϊ115200
	printf("--->freemodbus 10\r\n");
	UART4_INIT();
  OTA_Init();
	delay_init();	    			 //��ʱ������ʼ��	
	EXTI_Key_Config();       //������ʼ��
	RGB_Motion_GPIO_Config();//RGB&Motion��ʼ��
/****************************************/	
	SPI2_FLASH_Init();
	kaiji_LED();
	printf("************************************************************\r\n");
	printf("*     PROJECT: HERMES                                      *\r\n");
	printf("*                                                          *\r\n");
	printf("*     AUTHOR : MLB-AE                                      *\r\n");
  printf("*                                                          *\r\n"); 
	printf("*     DATE   : 2018-7-20                                   *\r\n");
  printf("*                                                          *\r\n");
	printf("************************************************************\r\n");
  Read_Device_Parameter_Direct();
/****************************************/	
	printf("device_addr_read_buf===%d\r\n",device_addr_read_buf);
	eMBInit(MB_RTU, device_addr_read_buf, 1, 9600, MB_PAR_NONE);//��ʼ�� FreeModbus ΪRTUģʽ �ӻ���ַΪ Uart3 115200 ��У��
	eMBEnable();		   //����FreeModbus 
	enable_5v();       //enable 5v power
	delay_ms(1000);
	I2C_EE_Init();     //I2C��ʼ��
	TSL2561_INIT();    //���ճ�ʼ��   
	delay_ms(1000);
	USART5_Config();   //BT module used
//	BT_Init();         //�ر�BT
	ADCx_Init();       //for noise use
	IWDG_Config(IWDG_Prescaler_256,625*5);	 //ι��ʱ��Ϊ20�룬��IWDG_Config(IWDG_Prescaler_64 ,625);���Ź�Ϊ1s\r\n
//	Ethernet_init();   //init ethernet
	PM25_Init();       //�۳���ʼ����0.78w��
	IWDG_Feed();       //ι������˴���ʱʱ��̫��
/*********************************************/
//������ʼ����
	xTaskCreate((TaskFunction_t )start_task,            //������
							(const char*    )"start_task",          //��������
							(uint16_t       )START_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                  //���ݸ��������Ĳ���
							(UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&StartTask_Handler);   //������              
	vTaskStartScheduler();      //�����������
} 
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/

