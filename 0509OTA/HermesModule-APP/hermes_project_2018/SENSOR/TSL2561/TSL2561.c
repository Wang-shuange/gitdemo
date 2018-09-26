/******************************************************************************
* @file    TSL2561.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   TSL2561π‚’’≤‚¡ø
*******************************************************************************/ 
#include "TSL2561.h"
#include "stm32f10x.h"
#include "./i2c/bsp_i2c_ee.h"
#include "./usart/bsp_usart.h"
/************************************************/
#define CMD     0x80
#define CLEAR   0x40
#define WORD_PROTOCAL   0x20
#define BLOCK_PROTOCAL   0x10

unsigned long chScale;
unsigned long channel1;
unsigned long channel0;
unsigned long  ratio1;
unsigned int b;
unsigned int m;
long temp;
long lux,ratio;
/************************************************/
#define ADDRESS_CONTROL             0x00
#define ADDRESS_TIMING              0x01
#define ADDRESS_THRESHLOWLOW        0x02
#define ADDRESS_THRESHLOWHIGH       0x03
#define ADDRESS_THRESHHIGHLOW       0x04
#define ADDRESS_THRESHHIGHHIGH      0x05
/************************************************/
#define ADDRESS_INTERRUPT           0x06
#define ADDRESS_RESV1               0x07
#define ADDRESS_CRC                 0x08
#define ADDRESS_RESV2               0x09
#define ADDRESS_ID                  0x0A
#define ADDRESS_RESV3               0x0B
/************************************************/
uint8_t I2c_Buf_Write[]={0x03};
uint8_t I2c_Buf_Write_1[]={0x03};
uint8_t I2c_Buf_Write_2[]={0x00};
uint8_t I2c_Buf_Write_3[]={0x00};
uint8_t I2c_Buf_Read1[256];
uint8_t I2c_Buf_Read2[256];
uint8_t I2c_Buf_Read3[256];
uint8_t I2c_Buf_Read4[256];
/************************************************/
#define ADDRESS_DATA0LOW            0x0C
#define ADDRESS_DATA0HIGH           0x0D
#define ADDRESS_DATA1LOW            0x0E
#define ADDRESS_DATA1HIGH           0x0F
uint8_t TXbuffer_4[] = {(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA0LOW),
(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA0HIGH),(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA1LOW),
(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA1HIGH)};
uint8_t TXbuffer_2[] = {(CMD | ADDRESS_CONTROL), 0x03};

int i,L1,L2;
float CH0,CH1,LUX;
/************************************************/
void TSL2561_INIT()
{
//   I2C_EE_ByteWrite(I2c_Buf_Write,0x52, TXbuffer_2[0]);
//	 I2C_EE_ByteWrite(I2c_Buf_Write,0x52, TXbuffer_2[1]);
	
	 I2C_EE_ByteWrite(I2c_Buf_Write_1,0x52, 0x80);
	 I2C_EE_ByteWrite(I2c_Buf_Write_2,0x52, 0x81);
	 I2C_EE_ByteWrite(I2c_Buf_Write_3,0x52, 0x86);
}
/************************************************/
void TSL2561_Read(void)
{    
			I2C_EE_BufferRead(I2c_Buf_Read1,0x52, 0X8C, 2);
			I2C_EE_BufferRead(I2c_Buf_Read2,0x52, 0X8D, 1);		
			I2C_EE_BufferRead(I2c_Buf_Read3,0x52, 0X8E, 1);
			I2C_EE_BufferRead(I2c_Buf_Read4,0x52, 0X8F, 1);
			GZ_Calculate();	
}
int TSL2561_ReturnL1(void)
{
			return	LUX;
}
int TSL2561_ReturnL2(void)
{
			return	L2;
}
void GZ_Calculate()
{
			CH0=I2c_Buf_Read1[0]+(I2c_Buf_Read2[0]<<8);
			CH1=I2c_Buf_Read3[0]+(I2c_Buf_Read4[0]<<8);
//		if(0<CH0/CH1&&CH0/CH1<=0.50) LUX=(0.0304)*CH0-(0.062)*CH0*(CH1/CH0)/4;
//		if(0.50<CH0/CH1&&CH0/CH1<=0.61) LUX=(0.0224)*CH0-0.031*CH1;
//		if(0.61<CH0/CH1&&CH0/CH1<=0.80) LUX=(0.0128)*CH0-0.0153*CH1;
//		if(0.80<CH0/CH1&&CH0/CH1<=1.30) LUX=(0.001)*CH0-0.00112*CH1;
//		if(CH1/CH0>1.30) LUX=0;
//			printf("LIAGHT_DATA: CH0: %.f CH1: %.f\r\n",CH0,CH1);
//			if(CH0<=1)LUX=0;
//			LUX=CH0-CH1*2;
//			printf("Lux %.f\r\n",LUX );
	    TSL2561_CalculateLux(0,0,0);
//	    printf("asasaaLux %ld\r\n",lux );
	      printf("%ld",lux );
}
unsigned long TSL2561_CalculateLux(unsigned int iGain, unsigned int tInt,int iType)
{
 switch (tInt)
 {
  case 0:  // 13.7 msec
  chScale = CHSCALE_TINT0;
  break;
  case 1: // 101 msec
  chScale = CHSCALE_TINT1;
  break;
  default: // assume no scaling
  chScale = (1 << CH_SCALE);
  break;
}
if (!iGain)  chScale = chScale << 4; // scale 1X to 16X
// scale the channel values
channel0 = ((uint16_t)CH0 * chScale) >> CH_SCALE;
channel1 = ((uint16_t)CH1 * chScale) >> CH_SCALE;

  ratio1 = 0;
 if (channel0!= 0) ratio1 = (channel1 << (RATIO_SCALE+1))/channel0;
// round the ratio value
 ratio = (ratio1 + 1) >> 1;

 switch (iType)
 {
 case 0: // T package
   if ((ratio >= 0) && (ratio <= K1T))
    {b=B1T; m=M1T;}
   else if (ratio <= K2T)
    {b=B2T; m=M2T;}
   else if (ratio <= K3T)
    {b=B3T; m=M3T;}
   else if (ratio <= K4T)
    {b=B4T; m=M4T;}
   else if (ratio <= K5T)
    {b=B5T; m=M5T;}
   else if (ratio <= K6T)
    {b=B6T; m=M6T;}
   else if (ratio <= K7T)
    {b=B7T; m=M7T;}
   else if (ratio > K8T)
    {b=B8T; m=M8T;}
 break;
  case 1:// CS package
   if ((ratio >= 0) && (ratio <= K1C))
    {b=B1C; m=M1C;}
   else if (ratio <= K2C)
    {b=B2C; m=M2C;}
  else if (ratio <= K3C)
   {b=B3C; m=M3C;}
  else if (ratio <= K4C)
   {b=B4C; m=M4C;}
  else if (ratio <= K5C)
   {b=B5C; m=M5C;}
  else if (ratio <= K6C)
   {b=B6C; m=M6C;}
  else if (ratio <= K7C)
    {b=B7C; m=M7C;}
 }
  temp=((channel0*b)-(channel1*m));
  if(temp<0) temp=0;
  temp+=(1<<(LUX_SCALE-1));
  // strip off fractional portion
  lux=temp>>LUX_SCALE;
  return (lux);
 }

