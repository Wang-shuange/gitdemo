/******************************************************************************
* @file    Team_Huy.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   温湿度测量
*******************************************************************************/  
#include "Team&Huy.h"
#include "stm32f10x.h"
#include "bsp_i2c_ee.h"
#include "bsp_usart.h"
/*********************************/
uint8_t I2c_Buf_Read1x[256];
uint8_t I2c_Buf_Read2x[256];
uint8_t I2c_Buf_Read3x[256];
int Y0,Y1,Y2,Y3;
double Y,Y_out1,Y_out2,Y_;
unsigned int X0,X1;
float X,X_out,X_;
unsigned long I2c_Buf_Read4x[256];
/*********************************/
float Team_Return(void)
{
	 I2C_EE_BufferRead(I2c_Buf_Read1x,0x80, 0XE3, 2);
	 X0=I2c_Buf_Read1x[0]<<8;
	 X1=I2c_Buf_Read1x[1];
	 X_out=X0+X1;
	 X=(175.72*X_out)/65536;
	 X=X-46.85;
	 X_=X;//用于转移原始值
	 X=X*(1.0275)-(6.6512)-4;
	 if(X>100)X=100;  //最大值限制
	 if(X<-40)X=-40;  //最小值限制	
	 return X;
}
/*********************************/
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
	 Y_=Y;//用于转移原始值
   Y=Y*(1.46)-5.493+13;
	 if(Y>100)Y=100;//最大值限制
	 if(Y<0)Y=0;    //最小值限制
	 return Y;
}



