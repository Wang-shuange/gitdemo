/******************************************************************************
* @file    CO2.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   CO2测量
*******************************************************************************/  
#include "CO2.h"
#include "stm32f10x.h"
#include "./i2c/bsp_i2c_ee.h"
#include "./usart/bsp_usart.h"
#include "utility.h"

int I2c_Buf_CO2[]={0};
int I2c_Buf_VO2[]={0};

uint8_t I2c_Buf_Read1xx[256];
uint8_t I2c_Buf_Write1[]={0};
uint8_t I2c_Buf_Readx[]={0};
uint8_t I2c_Buf_Write2[]={0x10};
//uint8_t I2c_Buf_Write1x[]={0};
//uint8_t I2c_Buf_Write2x[]={0x10};
/***********************************************************/		
void CO2_INIT(void)
{
			I2C_EE_PageWrite(I2c_Buf_Readx,0xB4, 0xF4,0);	//CO2初始化(双重)
			I2C_EE_ByteWrite(I2c_Buf_Write2,0xB4 , 0x01);
			delay_ms(200);
     if(I2c_Buf_Readx[0]&0x10) 	
	   {		
			I2C_EE_PageWrite(I2c_Buf_Write1,0xB4, 0xF4,0);
	   }
			I2C_EE_BufferRead(I2c_Buf_Readx,0xB4 , 0x00, 1);
			I2C_EE_BufferRead(I2c_Buf_Readx,0xB4 , 0x01, 1);
			I2C_EE_ByteWrite(I2c_Buf_Write2,0xB4 , 0x01);
			delay_ms(500);	
      I2C_EE_BufferRead(I2c_Buf_Readx,0xB4 , 0x20, 1);
	    printf("CO2_Code:%0x",I2c_Buf_Readx[0]);
			I2C_EE_BufferRead(I2c_Buf_Readx,0xB4 , 0x23, 1);
			printf(" %0x",I2c_Buf_Readx[0]);
			I2C_EE_BufferRead(I2c_Buf_Readx,0xB4 , 0x24, 1);
			printf(" %0x",I2c_Buf_Readx[0]);
			I2C_EE_BufferRead(I2c_Buf_Readx,0xB4 , 0x00, 1);
			printf(" %0x\r\n",I2c_Buf_Readx[0]);
						
}
/***********************************************************/			
void CO2_Read(void)
{
	   I2C_EE_BufferRead(I2c_Buf_Read1xx,0XB4, 0x02, 4);
	   I2c_Buf_CO2[0]=(I2c_Buf_Read1xx[0]*256) + I2c_Buf_Read1xx[1];
		 printf("CO2=%d\r\n",I2c_Buf_CO2[0]);
	   I2c_Buf_VO2[0]=(I2c_Buf_Read1xx[2]*256) + I2c_Buf_Read1xx[3];
		 printf("TVO2=%d\r\n",VO2_Return());
}	
int VO2_Read(void)
{
	   I2C_EE_BufferRead(I2c_Buf_Read1xx,0XB4, 0x02, 4);
	   I2c_Buf_VO2[0]=(I2c_Buf_Read1xx[2]*256) + I2c_Buf_Read1xx[3];
	   return I2c_Buf_VO2[0];
}	
int CO2_Return(void)
{
     return I2c_Buf_CO2[0];
}
int VO2_Return(void)
{
	   return I2c_Buf_VO2[0];
}


