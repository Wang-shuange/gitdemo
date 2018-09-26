/******************************************************************************
* @file    Team_Huy.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   ÎÂÊª¶È²âÁ¿
*******************************************************************************/  
#include "Team&Huy.h"
#include "stm32f10x.h"
#include "./i2c/bsp_i2c_ee.h"
#include "./usart/bsp_usart.h"

uint8_t I2c_Buf_Read1x[256];
uint8_t I2c_Buf_Read2x[256];
uint8_t I2c_Buf_Read3x[256];

int Y0,Y1,Y2,Y3;
double Y,Y_out1,Y_out2;

unsigned int X0,X1;
float X,X_out;

unsigned long I2c_Buf_Read4x[256];
void Team_Huy_Read(void)
{
//		I2C_EE_BufferRead(I2c_Buf_Read1x,0x80, 0XE3, 2);
//		I2C_EE_BufferRead(I2c_Buf_Read3x,0x80, 0XE5, 2);
//		I2c_Buf_Read4x[0]=(((I2c_Buf_Read1x[0]<<8) + I2c_Buf_Read1x[1])*175.72)/65536-46.85;
//		printf("Temp= %.2f", Team_Return());
//		printf(" C"); 		
////		I2c_Buf_Read4x[1]=((((I2c_Buf_Read3x[0]/100)*25600)*125/65536 + (I2c_Buf_Read3x[0]%100*256+I2c_Buf_Read3x[1])*125)/65536)-6;
//		printf("  Huy= %.2f", Huy_Return()); 
//		printf(" %%\r\n");

		printf("result=%.1f,", Team_Return()-5);		
		printf("%.f", Huy_Return());   

	
}
int cal_culat(void)
{
    return 0;
}
float Team_Return(void)
{
	 I2C_EE_BufferRead(I2c_Buf_Read1x,0x80, 0XE3, 2);
	 X0=I2c_Buf_Read1x[0]<<8;
	 X1=I2c_Buf_Read1x[1];
	 X_out=X0+X1;
	 X=(175.72*X_out)/65536;
	 X=X-46.85;
	 return X;
}
float Huy_Return(void)
{
	 I2C_EE_BufferRead(I2c_Buf_Read3x,0x80, 0XE5, 2);
	 Y0=I2c_Buf_Read3x[0];Y2=Y0/100; Y0=Y0%100;
	 Y1=I2c_Buf_Read3x[1];
	 Y_out1=Y2*25600;
	 Y_out2=Y0*256+Y1;
	 Y_out1=(125*Y_out1)/65536;
	 Y_out2=(125*Y_out2)/65536;
	 Y=Y_out1+Y_out2;
	 Y=Y-6;
	 return Y;
}



