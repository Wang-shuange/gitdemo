/******************************************************************************
* @file    TSL2561.c
* @author  ART
* @version V1.0
* @date    2018-04-02
* @brief   TSL2561光照测量
*******************************************************************************/ 
#include "TSL2562.h"
#include "stm32f10x.h"
#include "bsp_i2c_ee.h"
#include "bsp_usart.h"
#include "utility.h"
/************************************************/
#define CMD_2     0x80
#define CLEAR_2   0x40
#define WORD_PROTOCAL_2   0x20
#define BLOCK_PROTOCAL_2   0x10

unsigned long chScale_2;
unsigned long channel1_2;
unsigned long channel0_2;
unsigned long  ratio1_2;
unsigned int b_2;
unsigned int m_2;
long temp_2;
long lux_2,ratio_2;
/************************************************/
#define ADDRESS_CONTROL_2             0x00
#define ADDRESS_TIMING_2              0x01
#define ADDRESS_THRESHLOWLOW_2        0x02
#define ADDRESS_THRESHLOWHIGH_2       0x03
#define ADDRESS_THRESHHIGHLOW_2       0x04
#define ADDRESS_THRESHHIGHHIGH_2      0x05
/************************************************/
#define ADDRESS_INTERRUPT_2           0x06
#define ADDRESS_RESV1_2               0x07
#define ADDRESS_CRC_2                 0x08
#define ADDRESS_RESV2_2               0x09
#define ADDRESS_ID_2                  0x0A
#define ADDRESS_RESV3_2               0x0B
/************************************************/
uint8_t I2c_Buf_Write_2X[]={0x03};
uint8_t I2c_Buf_Write_1_2[]={0x03};
uint8_t I2c_Buf_Write_2_2[]={0x00};
uint8_t I2c_Buf_Write_3_2[]={0x00};
uint8_t I2c_Buf_Read1_2[256];
uint8_t I2c_Buf_Read2_2[256];
uint8_t I2c_Buf_Read3_2[256];
uint8_t I2c_Buf_Read4_2[256];
/************************************************/
#define ADDRESS_DATA0LOW_2            0x0C
#define ADDRESS_DATA0HIGH_2           0x0D
#define ADDRESS_DATA1LOW_2            0x0E
#define ADDRESS_DATA1HIGH_2           0x0F
uint8_t TXbuffer_4_2[] = {(CMD_2 | CLEAR_2 | WORD_PROTOCAL_2 | ADDRESS_DATA0LOW_2),
(CMD_2 | CLEAR_2 | WORD_PROTOCAL_2 | ADDRESS_DATA0HIGH_2),(CMD_2 | CLEAR_2 | WORD_PROTOCAL_2 | ADDRESS_DATA1LOW_2),
(CMD_2 | CLEAR_2 | WORD_PROTOCAL_2 | ADDRESS_DATA1HIGH_2)};
uint8_t TXbuffer_2_2[] = {(CMD_2 | ADDRESS_CONTROL_2), 0x03};

int L1_2,L2_2;
float CH0_2,CH1_2;

#define    LIGHT_GPIO_CLK_2           RCC_APB2Periph_GPIOC
#define    LIGHT_GPIO_PORT_2          GPIOC		   
#define    LIGHT_ADDR_SEL_GPIO_PIN_2	GPIO_Pin_2  //Address select pin ,low level 	   
#define    LIGHT_INT_GPIO_PIN_2		    GPIO_Pin_1   //int pin 

/************************************************/
void TSL2561_Config_2(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = LIGHT_ADDR_SEL_GPIO_PIN_2|LIGHT_INT_GPIO_PIN_2;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//////////////////xxxxxxxxxxxxx   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LIGHT_GPIO_PORT_2, &GPIO_InitStructure);			
//		GPIO_SetBits(LIGHT_GPIO_PORT_2,LIGHT_ADDR_SEL_GPIO_PIN_2|LIGHT_INT_GPIO_PIN_2);
}
void TSL2561_INIT_2()
{
	 TSL2561_Config_2();
	 delay_ms(300);
	 I2C_EE_ByteWrite(I2c_Buf_Write_1_2,0x72, 0x80);
	 I2C_EE_ByteWrite(I2c_Buf_Write_2_2,0x72, 0x81);
	 I2C_EE_ByteWrite(I2c_Buf_Write_3_2,0x72, 0x86);
}
/************************************************/
void TSL2561_Read_2(void)
{    
	I2C_EE_BufferRead(I2c_Buf_Read1_2,0x72, 0X8C, 2);
	I2C_EE_BufferRead(I2c_Buf_Read2_2,0x72, 0X8D, 1);		
	I2C_EE_BufferRead(I2c_Buf_Read3_2,0x72, 0X8E, 1);
	I2C_EE_BufferRead(I2c_Buf_Read4_2,0x72, 0X8F, 1);
}
int TSL2561_ReturnL2(void)
{
  TSL2561_Read_2();          //光照測量	 
	GZ_Calculate_2();
	lux_2=lux_2*1.145+46.07;
	if(lux_2<=46.07)lux_2=0;   //强制归零
	return	lux_2;
}
//int TSL2561_ReturnL2_2X(void)
//{
//	return	L2_2;
//}
void GZ_Calculate_2()
{
	CH0_2=I2c_Buf_Read1_2[0]+(I2c_Buf_Read2_2[0]<<8);
	CH1_2=I2c_Buf_Read3_2[0]+(I2c_Buf_Read4_2[0]<<8);
	TSL2561_CalculateLux_2(0,0,0);
}
unsigned long TSL2561_CalculateLux_2(unsigned int iGain, unsigned int tInt,int iType)
{
 switch (tInt)
 {
  case 0:  // 13.7 msec
  chScale_2 = CHSCALE_TINT0_2;
  break;
  case 1: // 101 msec
  chScale_2 = CHSCALE_TINT1_2;
  break;
  default: // assume no scaling
  chScale_2 = (1 << CH_SCALE_2);
  break;
}
if (!iGain)  chScale_2 = chScale_2 << 4; // scale 1X to 16X
// scale the channel values
channel0_2 = ((uint16_t)CH0_2 * chScale_2) >> CH_SCALE_2;
channel1_2 = ((uint16_t)CH1_2 * chScale_2) >> CH_SCALE_2;

  ratio1_2 = 0;
 if (channel0_2!= 0) ratio1_2 = (channel1_2 << (RATIO_SCALE_2+1))/channel0_2;
// round the ratio value
 ratio_2 = (ratio1_2 + 1) >> 1;

 switch (iType)
 {
 case 0: // T package
   if ((ratio_2 >= 0) && (ratio_2 <= K1T_2))
    {b_2=B1T_2; m_2=M1T_2;}
   else if (ratio_2 <= K2T_2)
    {b_2=B2T_2; m_2=M2T_2;}
   else if (ratio_2 <= K3T_2)
    {b_2=B3T_2; m_2=M3T_2;}
   else if (ratio_2 <= K4T_2)
    {b_2=B4T_2; m_2=M4T_2;}
   else if (ratio_2 <= K5T_2)
    {b_2=B5T_2; m_2=M5T_2;}
   else if (ratio_2 <= K6T_2)
    {b_2=B6T_2; m_2=M6T_2;}
   else if (ratio_2 <= K7T_2)
    {b_2=B7T_2; m_2=M7T_2;}
   else if (ratio_2 > K8T_2)
    {b_2=B8T_2; m_2=M8T_2;}
 break;
  case 1:// CS package
   if ((ratio_2 >= 0) && (ratio_2 <= K1C_2))
    {b_2=B1C_2; m_2=M1C_2;}
   else if (ratio_2 <= K2C_2)
    {b_2=B2C_2; m_2=M2C_2;}
  else if (ratio_2 <= K3C_2)
   {b_2=B3C_2; m_2=M3C_2;}
  else if (ratio_2 <= K4C_2)
   {b_2=B4C_2; m_2=M4C_2;}
  else if (ratio_2 <= K5C_2)
   {b_2=B5C_2; m_2=M5C_2;}
  else if (ratio_2 <= K6C_2)
   {b_2=B6C_2; m_2=M6C_2;}
  else if (ratio_2 <= K7C_2)
    {b_2=B7C_2; m_2=M7C_2;}
 }
  temp_2=((channel0_2*b_2)-(channel1_2*m_2));
  if(temp_2<0) temp_2=0;
  temp_2+=(1<<(LUX_SCALE_2-1));
  // strip off fractional portion
  lux_2=temp_2>>LUX_SCALE_2;
  return (lux_2);
 }

