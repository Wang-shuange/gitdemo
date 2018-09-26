/******************************************************************************
* @file    BQ24259.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   ��Դ����
*******************************************************************************/ 
#include "BQ24259.h"
#include "stm32f10x.h"
#include "./i2c/bsp_i2c_ee.h"
#include <string.h>
#include "./usart/bsp_usart.h"
/**********************************************************/
extern int i;
uint8_t input_source_control[]={0x37};
uint8_t input_source_control1[]={0x34};
uint8_t power_on_control[]={0x1b};
uint8_t charge_current_control[]={0x60};
uint8_t pre_charge_termination_control[]={0x11};
uint8_t charge_voltage_control[]={0xb2};
uint8_t charge_termination_timer_control[]={0x9c};
uint8_t boost_voltage_thermal_regulation_control[]={0x73};
uint8_t misc_operation_control[]={0x4b};
uint8_t Buffer_Read_Status[3];
uint8_t Buffer_Read_Write[12];
/**************��ȡ��Դ״ֵ̬********************************/
void Power_Status_Read()
{
  I2C_EE_BufferRead(Buffer_Read_Status,0xd6 , 0x08, 3);   //ϵͳ״̬�Ĵ��������ϼĴ�����
	printf("Power Test");
	for(i=0;i<3;i++)  printf(" %x",Buffer_Read_Status[i]);
	printf("\r\n");
	memset(Buffer_Read_Status,0,3);	
}
/********************��ʼ������******************************/
void Power_Manage_Init()
{
  I2C_EE_ByteWrite(input_source_control,0xd6, 0x00);            //�����������ѹ����
	I2C_EE_ByteWrite(power_on_control,0xd6, 0x01);                //������ü���Сϵͳ��ѹ����
	I2C_EE_ByteWrite(charge_current_control,0xd6, 0x02);          //����������
	I2C_EE_ByteWrite(pre_charge_termination_control,0xd6, 0x03);  //Ԥ����������ֹ��������
	I2C_EE_ByteWrite(charge_voltage_control,0xd6, 0x04);          //��س���ѹ����
	I2C_EE_ByteWrite(charge_termination_timer_control,0xd6, 0x05);//�����ֹʹ�ܡ�I2C��ʱ�����á���簲ȫ��ʱ��ʹ�ܡ����ʱ������
	I2C_EE_ByteWrite(boost_voltage_thermal_regulation_control,0xd6, 0x06);
	I2C_EE_ByteWrite(misc_operation_control,0xd6, 0x07);          //����DPDM��⼰
}

