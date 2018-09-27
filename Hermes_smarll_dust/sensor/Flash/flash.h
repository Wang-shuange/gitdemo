/******************************************************************************
* @file    bsp_spi_flash.h
* @author  ART
* @version V1.0
* @date    2018-04-18
* @brief   Flash初始化
*******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"
#include <stdio.h>

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
	
#define  BufferSize (countof(Tx_Buffer)-1)
#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
/**********网络参数存储地址*****************/
#define  IP_Port_BufferSize (countof(code_ssid)-1)
#define  Password_BufferSize (countof(code_password)-1)
#define  Read_IP_BufferSize   (countof(Read_IP)-1)
#define  Port_BufferSize (countof(Port)-1)

#define  MAC_ID_BufferSize (countof(MAC_ID)-1)
#define  APP_FLAG_BufferSize (countof(APP_FLAG)-1)
#define  APP_FLAG_BufferSizex (countof(APP_FLAGx)-1)

#define  Server_IP_BufferSize (countof(Server_IP)-1)
#define  Server_Port_BufferSize (countof(Server_Port)-1)


#define  code_ssid_BufferSize (countof(code_ssid)-1)
#define  code_Password_BufferSize (countof(code_Password)-1)

#define  IP_Port_WriteAddress     0x00000
#define  IP_Port_ReadAddress      IP_Port_WriteAddress
#define  IP_Port_SectorToErase    IP_Port_WriteAddress
void Flash_Write_SSID_Password(void);
//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
//#define  sFLASH_ID              0XEF4018   //W25Q128
#define  sFLASH_ID              0XEF4017    //W25Q64

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/

/***********************************************************************************************************/
/*SPI接口定义-开头****************************/   //公板Flash选用
#define      FLASH_SPIx                        SPI2
#define      FLASH_SPI_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define      FLASH_SPI_CLK                     RCC_APB1Periph_SPI2

//CS(NSS)引脚 片选选普通GPIO即可
#define      FLASH_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOB  
#define      FLASH_SPI_CS_PORT                 GPIOB
#define      FLASH_SPI_CS_PIN                  GPIO_Pin_12

//SCK引脚
#define      FLASH_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOB  
#define      FLASH_SPI_SCK_PORT                GPIOB   
#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_13
//MISO引脚
#define      FLASH_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOB    
#define      FLASH_SPI_MISO_PORT               GPIOB 
#define      FLASH_SPI_MISO_PIN                GPIO_Pin_14
//MOSI引脚
#define      FLASH_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOB    
#define      FLASH_SPI_MOSI_PORT               GPIOB
#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_15

#define  		SPI_FLASH_CS_LOW()     						GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define  		SPI_FLASH_CS_HIGH()    						GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
/***********************************************************************************************************/
//#define      FLASH_SPIx                        SPI1
//#define      FLASH_SPI_APBxClock_FUN           RCC_APB2PeriphClockCmd
//#define      FLASH_SPI_CLK                     RCC_APB2Periph_SPI1

////CS(NSS)引脚 片选选普通GPIO即可
//#define      FLASH_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
//#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOA  
//#define      FLASH_SPI_CS_PORT                 GPIOA
//#define      FLASH_SPI_CS_PIN                  GPIO_Pin_4

////SCK引脚
//#define      FLASH_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
//#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA  
//#define      FLASH_SPI_SCK_PORT                GPIOA   
//#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_5
////MISO引脚
//#define      FLASH_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
//#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
//#define      FLASH_SPI_MISO_PORT               GPIOA 
//#define      FLASH_SPI_MISO_PIN                GPIO_Pin_6
////MOSI引脚
//#define      FLASH_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
//#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
//#define      FLASH_SPI_MOSI_PORT               GPIOA
//#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_7

//#define  		SPI_FLASH_CS_LOW()     						GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
//#define  		SPI_FLASH_CS_HIGH()    						GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

/*SPI接口定义-结尾****************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
extern char zhongzhuanx[30];
void SPI2_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);																					
																					
u8 SPI_FLASH_ReadByte(void);
u8 SPI2_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SSID_Password_Read(void);
void Flash_Read(void);	
void Flash_Write(void);
void Write_smartconfig_flag(void);
void Read_smartconfig_flag(void);
void Write_Key_reset_flag(void);
void Read_Key_reset_flag(void);
void Flash_Read_MAC(void);
int Flash_Readx(void);
void Flash_Writex(void);
void Flash_Write_Serve_IP_Port(void);
void Flash_Read_Serve_IP_Port(void);
void Ethernet_Write_flag(void);
void Ethernet_Read_flag(void);

#endif /* __SPI_FLASH_H */

