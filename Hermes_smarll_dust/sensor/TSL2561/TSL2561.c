/******************************************************************************
* @file    TSL2561.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   TSL2561光照测量
*******************************************************************************/ 
#include "TSL2561.h"
#include "stm32f10x.h"
#include "bsp_i2c_ee.h"
#include "bsp_usart.h"
#include "utility.h"
#include "string.h"
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
uint8_t I2c_Buf_Read1[5];
uint8_t I2c_Buf_Read2[5];
uint8_t I2c_Buf_Read3[5];
uint8_t I2c_Buf_Read4[5];
/************************************************/
#define ADDRESS_DATA0LOW            0x0C
#define ADDRESS_DATA0HIGH           0x0D
#define ADDRESS_DATA1LOW            0x0E
#define ADDRESS_DATA1HIGH           0x0F
uint8_t TXbuffer_4[] = {(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA0LOW),
(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA0HIGH),(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA1LOW),
(CMD | CLEAR | WORD_PROTOCAL | ADDRESS_DATA1HIGH)};
uint8_t TXbuffer_2[] = {(CMD | ADDRESS_CONTROL), 0x03};

int L1,L2;
float CH0,CH1;
//光照一地址引腳設置低電平addr:0x52
#define    LIGHT_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    LIGHT_GPIO_PORT    GPIOE		   
#define    LIGHT_ADDR_SEL_GPIO_PIN		 GPIO_Pin_1  //Address select pin ,low level 	   
#define    LIGHT_INT_GPIO_PIN		       GPIO_Pin_11   //int pin 
//光照二地址引腳設置高電平addr:0x92
#define    LIGHT_GPIO_CLK_2           RCC_APB2Periph_GPIOC
#define    LIGHT_GPIO_PORT_2          GPIOC		   
#define    LIGHT_ADDR_SEL_GPIO_PIN_2	GPIO_Pin_2  //Address select pin ,low level 	   
#define    LIGHT_INT_GPIO_PIN_2		    GPIO_Pin_1   //int pin 
/************************************************/
void TSL2561_Config(void)//light_1
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
		GPIO_InitStructure.GPIO_Pin = LIGHT_ADDR_SEL_GPIO_PIN|LIGHT_INT_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LIGHT_GPIO_PORT, &GPIO_InitStructure);	
		GPIO_ResetBits(LIGHT_GPIO_PORT,LIGHT_ADDR_SEL_GPIO_PIN|LIGHT_INT_GPIO_PIN);
}
void TSL2561_Config_2(void)//light_2
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = LIGHT_ADDR_SEL_GPIO_PIN_2|LIGHT_INT_GPIO_PIN_2;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LIGHT_GPIO_PORT_2, &GPIO_InitStructure);			
		GPIO_SetBits(LIGHT_GPIO_PORT_2,LIGHT_ADDR_SEL_GPIO_PIN_2|LIGHT_INT_GPIO_PIN_2);
}
void TSL2561_INIT_2()//light_2
{
	 TSL2561_Config_2();
	 delay_ms(300);
	 I2C_EE_ByteWrite(I2c_Buf_Write_1,0x92, 0x80);
	 I2C_EE_ByteWrite(I2c_Buf_Write_2,0x92, 0x81);
	 I2C_EE_ByteWrite(I2c_Buf_Write_3,0x92, 0x86);
}
/************************************************/
void TSL2561_Read_2(void)//light_2
{ 
	memset(I2c_Buf_Read1,0,5);
	memset(I2c_Buf_Read2,0,5);
	memset(I2c_Buf_Read3,0,5);
	memset(I2c_Buf_Read4,0,5);
	I2C_EE_BufferRead(I2c_Buf_Read1,0x92, 0X8C, 2);
	I2C_EE_BufferRead(I2c_Buf_Read2,0x92, 0X8D, 1);		
	I2C_EE_BufferRead(I2c_Buf_Read3,0x92, 0X8E, 1);
	I2C_EE_BufferRead(I2c_Buf_Read4,0x92, 0X8F, 1);
}
int TSL2561_ReturnL2(void)//light_2
{
  TSL2561_Read_2();      //光照測量	 
	GZ_Calculate();
	lux=lux*1.145+46.07;
	if(lux<=46.07)lux=0;   //强制归零
	return	lux;
}
void TSL2561_INIT()//light_1
{
	TSL2561_Config();
	delay_ms(300);
	I2C_EE_ByteWrite(I2c_Buf_Write_1,0x52, 0x80);
	I2C_EE_ByteWrite(I2c_Buf_Write_2,0x52, 0x81);
	I2C_EE_ByteWrite(I2c_Buf_Write_3,0x52, 0x86);
}
/************************************************/
void TSL2561_Read(void)//light_1
{ 
	memset(I2c_Buf_Read1,0,5);
	memset(I2c_Buf_Read2,0,5);
	memset(I2c_Buf_Read3,0,5);
	memset(I2c_Buf_Read4,0,5);
	I2C_EE_BufferRead(I2c_Buf_Read1,0x52, 0X8C, 2);
	I2C_EE_BufferRead(I2c_Buf_Read2,0x52, 0X8D, 1);		
	I2C_EE_BufferRead(I2c_Buf_Read3,0x52, 0X8E, 1);
	I2C_EE_BufferRead(I2c_Buf_Read4,0x52, 0X8F, 1);
}
int TSL2561_ReturnL1(void)//light_1
{
  TSL2561_Read();          //光照測量	 
	GZ_Calculate();
	lux=lux*1.145+46.07;
	if(lux<=46.07)lux=0;     //强制归零
	return	lux;
}
void GZ_Calculate()//light_1/light_2 common
{
	CH0=I2c_Buf_Read1[0]+(I2c_Buf_Read2[0]<<8);
	CH1=I2c_Buf_Read3[0]+(I2c_Buf_Read4[0]<<8);
	TSL2561_CalculateLux(0,0,0);
}
//light_1/light_2 common
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

