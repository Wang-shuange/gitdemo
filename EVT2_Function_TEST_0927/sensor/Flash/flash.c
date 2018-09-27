/******************************************************************************
* @file    bsp_spi_flash.c
* @author  ART
* @version V1.0
* @date    2018-04-18
* @brief   Flash初始化
*******************************************************************************/ 
#include "flash.h"
#include "./usart/bsp_usart.h"
#include "utility.h"
#include "string.h"
#include "esp8266.h"
#include "bsp_iwdg.h" 

uint8_t Tx_Buffer[] = "公板：Hermes Flash Test OK\r\n";
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
/* 发送缓冲区初始化 */
uint8_t smartconfig_flag[]="SMART_FLAG";
uint8_t smartconfig_flag_Buffer[10];

uint8_t Key_flag[]="KEY_SET";
uint8_t Key_flag_Buffer[10];

uint8_t APP_FLAG[]="000000000";
uint8_t APP_FLAG_Buffer[BufferSize];
uint8_t APP_FLAGx[]="PROGRAMUPDATEOK";
uint8_t APP_FLAG_Bufferx[BufferSize];

uint8_t MAC_ID[30];
uint8_t MAC_ID_Buffer[BufferSize]="0008DC22FA6F";

extern uint8_t Server_IP[30];
extern uint8_t Server_Port[10];
uint8_t Server_IP_Buffer[BufferSize];
uint8_t Server_Port_Buffer[BufferSize];

/* 发送缓冲区初始化 */
uint8_t IP[]="172.27.35.12";
uint8_t Port[]="5000\r\n";
uint8_t SSID[30];
uint8_t Password[30]; 
extern char Read_IP[30];
uint8_t zhon_zhuan[20];

extern char code_ssid[30];
extern char code_password[30];

extern uint8_t Rx_Buffer[BufferSize];
extern uint8_t SSID_Buffer[BufferSize];
extern uint8_t Password_Buffer[BufferSize];
extern uint8_t Port_Buffer[BufferSize];
extern uint8_t IP_Buffer[BufferSize];
extern uint8_t Read_IP_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

uint8_t Ethernet[]="11111111";
uint8_t Ethernet_Buffer[10];

#define DEVICE_MAC_READ_BUF_LEN 13
#define DEVICE_MAC 0x09000
u8 Device_Mac_Read_buf[DEVICE_MAC_READ_BUF_LEN],Device_Mac_Write_buf[]="0008DC22FA6F";
extern uint8 mac[6];//以太網MAC
char ethernet_ok_flag=0;
/*************************************************/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
/*************************************************/
void Flash_Write_SSID_Password(void)
{
  /* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	delay_ms(1000);
	
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("FlashID is 0x%X,Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID)                  /*********用于存储WiFi热点、密码、IP地址*********/
	{	
//		printf("\r\n 检测到串行flash W25Q64 !\r\n");		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
		SPI_FLASH_SectorErase(FLASH_SectorToErase);
//		SPI_FLASH_SectorErase(0x00030);	 	 		
		/* 将发送缓冲区的数据写到flash中 */
		// 这里写一页，一页的大小为256个字节
		SPI_FLASH_BufferWrite((uint8_t*)code_ssid, 0x00000, IP_Port_BufferSize);
    printf("写入的数据为:%s\r\n",code_ssid);		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(SSID_Buffer, 0x00000, IP_Port_BufferSize);
		printf("读出的数据为:%s\r\n", SSID_Buffer);
		if(strstr((char*)code_ssid,(char*)SSID_Buffer))
    printf("--->>>FLASH TEST OK\r\n");	
		else printf("--->>>FLASH TEST FAIL\r\n");	
	}
}
void Flash_Write(void)//存储APP更新标志，判断是否运行以存入的APP程序
{
	SPI_FLASH_SectorErase(0x01000);	 
	SPI_FLASH_BufferWrite(APP_FLAG, 0x01000, APP_FLAG_BufferSize);	
	SPI_FLASH_BufferRead(APP_FLAG_Buffer, 0x01000, APP_FLAG_BufferSize);
	printf("\r\n读出的数据为%s\r\n", APP_FLAG_Buffer);
}
void Flash_Writex(void)
{
	SPI_FLASH_SectorErase(0x02000);	 
}
int Flash_Readx(void)
{
		int str=0,i;
		SPI_FLASH_BufferRead(APP_FLAG_Bufferx, 0x02000, APP_FLAG_BufferSizex);
		printf("保存的FLAG数据：%s\r\n", APP_FLAG_Bufferx);
		printf("十进制接收数据: ");
		for(i=0;i<9;i++)printf("%d", APP_FLAG_Bufferx[i]);
		printf("\r\n");
		if(APP_FLAG_Bufferx[1]!=255&&APP_FLAG_Bufferx[2]!=255)str=1;
    //48代表的是0，255代表的是NULL即不等于零且不等于空时成立
	  printf("str=%d\r\n",str);
		return 	str;
}
void Flash_Read_MAC(void)
{
	SPI_FLASH_BufferRead(MAC_ID_Buffer, 0x05000, MAC_ID_BufferSize);
	printf("读出的数据为  MAC_ADDR：%s\r\n", MAC_ID_Buffer);
}
void Flash_Read(void)
{
		SPI_FLASH_BufferRead(SSID_Buffer, 0x00000, IP_Port_BufferSize);
		printf("读出的数据为    SSID：%s\r\n", SSID_Buffer);
		printf("  SSID_Lenth:%d\r\n",strlen((char*)SSID_Buffer));
		SPI_FLASH_BufferRead(Password_Buffer, 0x00020, Password_BufferSize);
		printf("读出的数据为  Passrd：%s\r\n", Password_Buffer);
    printf("Passrd_Lenth:%d\r\n",strlen((char*)Password_Buffer));	
}
void Flash_Write_Serve_IP_Port(void)
{
	  SPI_FLASH_SectorErase(0x06000);
    SPI_FLASH_SectorErase(0x07000);	
	  SPI_FLASH_BufferWrite(Server_IP, 0x06000, Server_IP_BufferSize);
		SPI_FLASH_BufferWrite(Server_Port, 0x07000, Server_Port_BufferSize);
/***************************/		
	  SPI_FLASH_BufferRead(Server_IP_Buffer, 0x06000, Password_BufferSize);
		printf("读出的数据为    Server_IP：%s\r\n", Server_IP_Buffer);
		SPI_FLASH_BufferRead(Server_Port_Buffer, 0x07000, Password_BufferSize);
		printf("读出的数据为  Server_Port：%s\r\n", Server_Port_Buffer);
}
void Flash_Read_Serve_IP_Port(void)
{
	  SPI_FLASH_BufferRead(Server_IP_Buffer, 0x06000, Password_BufferSize);
		printf("读出的数据为    Server_IP：%s\r\n", Server_IP_Buffer);
		SPI_FLASH_BufferRead(Server_Port_Buffer, 0x07000, Password_BufferSize);
		printf("读出的数据为  Server_Port：%s\r\n", Server_Port_Buffer);
}
void Write_smartconfig_flag()
{
   SPI_FLASH_SectorErase(0x03000); 
	 SPI_FLASH_BufferWrite(smartconfig_flag, 0x03000, countof(smartconfig_flag)-1); 
}
void Read_smartconfig_flag()
{
	 SPI_FLASH_BufferRead(smartconfig_flag_Buffer, 0x03000, countof(smartconfig_flag)-1); 
	 printf("读出的数据为smart_flag：%s    \r\n", smartconfig_flag_Buffer);

}
void Write_Key_reset_flag()
{
   SPI_FLASH_SectorErase(0x04000); 
	 SPI_FLASH_BufferWrite(Key_flag, 0x04000, countof(Key_flag)-1); 
}
void Read_Key_reset_flag()
{
	 SPI_FLASH_BufferRead(Key_flag_Buffer, 0x04000, countof(Key_flag)-1); 
	 printf("读出的数据为Key_flag：%s    \r\n", Key_flag_Buffer);
}
//void Ethernet_Write_flag(void)
//{
//   SPI_FLASH_SectorErase(0x09000); 
//	 SPI_FLASH_BufferWrite(Ethernet, 0x09000, countof(Ethernet)-1); 
//}
//void Ethernet_Read_flag(void)
//{
//	 SPI_FLASH_BufferRead(Ethernet_Buffer, 0x09000, countof(Ethernet)-1); 
//	 printf("Ethernet_Buffer:%s\r\n",Ethernet_Buffer);
//	 printf("EthNet_Buff_Lth:%d\r\n",countof(Ethernet)-1);
//}
void Ethernet_Write_MAC(void)
{
   SPI_FLASH_SectorErase(DEVICE_MAC); 
	 SPI_FLASH_BufferWrite(Device_Mac_Write_buf, DEVICE_MAC, 12); 
}
void Ethernet_Read_MAC(void)
{
	 SPI_FLASH_BufferRead(Device_Mac_Read_buf, DEVICE_MAC, 12); 
	 printf("--->>>ETHERNET_RESP_MAC#%s@#\r\n",Device_Mac_Read_buf);
}
/*************************************************/
void Config_Ehernet_MAC(void)
{
  if(strstr(user_input,"AT+WRITE_ETHMAC#"))
	{
	  strncpy((char*)Device_Mac_Write_buf,user_input+16,12);
		printf("--->>>ETHERNET_REQ_MAC#:%s#@\r\n",Device_Mac_Write_buf);
		Ethernet_Write_MAC();
		delay_ms(100);
		Ethernet_Read_MAC();
		printf("ReadMAC--->>>:%s\r\n",Device_Mac_Read_buf);
		string_to_hex_ethernet();//for eth mac transmit use
		if(strstr((char*)Device_Mac_Write_buf,(char*)Device_Mac_Read_buf))
		{
		  ethernet_ok_flag=1;
		}
		memset(user_input,0,1024);
	}	
}
/**********************************************************************************/
void string_to_hex_ethernet(void)//for eth mac transmit use
{
	  char code[12];//0008DC22FA6F
    int i;
		for(i=0;i<12;i++)
	{
		if(Device_Mac_Read_buf[i]<='9')
			code[i]=Device_Mac_Read_buf[i]-0x30; //0x30 
		else 
			code[i]=Device_Mac_Read_buf[i]-0x37;
	}
	  printf("--->>MAC_Hex:");
		for(i=0;i<6;i++)
		{
			mac[i]=code[2*i]<<4|code[2*i+1];
			printf(" %0x",mac[i]);
		}
		printf("\r\n");
}
/**
  * @brief  SPI_FLASH初始化
  * @param  无
  * @retval 无
  */
void SPI2_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	/* 使能SPI时钟 */
	FLASH_SPI_APBxClock_FUN ( FLASH_SPI_CLK, ENABLE );
	/* 使能SPI引脚相关的时钟 */
 	FLASH_SPI_CS_APBxClock_FUN ( FLASH_SPI_CS_CLK|FLASH_SPI_SCK_CLK|
																	FLASH_SPI_MISO_PIN|FLASH_SPI_MOSI_PIN, ENABLE );	
  /* 配置SPI的 CS引脚，普通IO即可 */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);
	
  /* 配置SPI的 SCK引脚*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

  /* 配置SPI的 MISO引脚*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

  /* 配置SPI的 MOSI引脚*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

  /* 停止信号 FLASH: CS引脚高电平*/
  SPI_FLASH_CS_HIGH();

  /* SPI 模式配置 */
  // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPIx , &SPI_InitStructure);

  /* 使能 SPI  */
  SPI_Cmd(FLASH_SPIx , ENABLE);
}
 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* 擦除扇区 */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送扇区擦除指令*/
  SPI2_FLASH_SendByte(W25X_SectorErase);
  /*发送擦除扇区地址的高位*/
  SPI2_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* 发送擦除扇区地址的中位 */
  SPI2_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* 发送擦除扇区地址的低位 */
  SPI2_FLASH_SendByte(SectorAddr & 0xFF);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}
 /**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送整块擦除指令*/
  SPI2_FLASH_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}
 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 写页写指令*/
  SPI2_FLASH_SendByte(W25X_PageProgram);
  /*发送写地址的高位*/
  SPI2_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中位*/
  SPI2_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低位*/
  SPI2_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
  }
  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    SPI2_FLASH_SendByte(*pBuffer);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}
 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*先写满当前页*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*再写剩余的数据*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* 先写完count个数据，为的是让下一次要写的地址对齐 */
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			
			/* 接下来就重复地址对齐的情况 */
      WriteAddr +=  count;
      pBuffer += count;
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
 /**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送 读 指令 */
  SPI2_FLASH_SendByte(W25X_ReadData);
  /* 发送 读 地址高位 */
  SPI2_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* 发送 读 地址中位 */
  SPI2_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* 发送 读 地址低位 */
  SPI2_FLASH_SendByte(ReadAddr & 0xFF);	
	/* 读取数据 */
  while (NumByteToRead--) /* while there is data to be read */
  {
    /* 读取一个字节*/
    *pBuffer = SPI2_FLASH_SendByte(Dummy_Byte);
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送JEDEC指令，读取ID */
  SPI2_FLASH_SendByte(W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  Temp0 = SPI2_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp1 = SPI2_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp2 = SPI2_FLASH_SendByte(Dummy_Byte);

 /* 停止通讯：CS高电平 */
  SPI_FLASH_CS_HIGH();

  /*把数据组合起来，作为函数的返回值*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
 /**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI2_FLASH_SendByte(W25X_DeviceID);
  SPI2_FLASH_SendByte(Dummy_Byte);
  SPI2_FLASH_SendByte(Dummy_Byte);
  SPI2_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI2_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI2_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI2_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI2_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI2_FLASH_SendByte(ReadAddr & 0xFF);
}


 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI2_FLASH_SendByte(Dummy_Byte));
}

 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u8 SPI2_FLASH_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(FLASH_SPIx , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

 /**
  * @brief  使用SPI发送两个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }
	
  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(FLASH_SPIx , HalfWord);

	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* 通讯开始：CS低 */
  SPI_FLASH_CS_LOW();

  /* 发送写使能命令*/
  SPI2_FLASH_SendByte(W25X_WriteEnable);

  /*通讯结束：CS高 */
  SPI_FLASH_CS_HIGH();
}

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* 选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发送 读状态寄存器 命令 */
  SPI2_FLASH_SendByte(W25X_ReadStatusReg);

  /* 若FLASH忙碌，则等待 */
  do
  {
		/* 读取FLASH芯片的状态寄存器 */
    FLASH_Status = SPI2_FLASH_SendByte(Dummy_Byte);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 */

  /* 停止信号  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}


//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  /* 通讯开始：CS低 */
  SPI_FLASH_CS_LOW();

  /* 发送 掉电 命令 */
  SPI2_FLASH_SendByte(W25X_PowerDown);

  /*通讯结束：CS高 */
  SPI_FLASH_CS_HIGH();
}   

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  /*选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发送 上电 命令 */
  SPI2_FLASH_SendByte(W25X_ReleasePowerDown);

   /* 停止信号 FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}   
   

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}
   
/*********************************************END OF FILE**********************/
