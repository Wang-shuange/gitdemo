/******************************************************************************
* @file    bsp_spi_flash.c
* @author  ART
* @version V1.0
* @date    2018-04-18
* @brief   Flash��ʼ��
*******************************************************************************/ 
#include "flash.h"
#include "./usart/bsp_usart.h"
#include "utility.h"
#include "string.h"
#include "esp8266.h"
#include "bsp_iwdg.h" 

uint8_t Tx_Buffer[] = "���壺Hermes Flash Test OK\r\n";
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
/* ���ͻ�������ʼ�� */
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

/* ���ͻ�������ʼ�� */
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
extern uint8 mac[6];//��̫�WMAC
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
  /* ��ȡ Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	delay_ms(1000);
	
	/* ��ȡ SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("FlashID is 0x%X,Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID)                  /*********���ڴ洢WiFi�ȵ㡢���롢IP��ַ*********/
	{	
//		printf("\r\n ��⵽����flash W25Q64 !\r\n");		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		// �������4K����һ����������������С��λ������
		SPI_FLASH_SectorErase(FLASH_SectorToErase);
//		SPI_FLASH_SectorErase(0x00030);	 	 		
		/* �����ͻ�����������д��flash�� */
		// ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�
		SPI_FLASH_BufferWrite((uint8_t*)code_ssid, 0x00000, IP_Port_BufferSize);
    printf("д�������Ϊ:%s\r\n",code_ssid);		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		SPI_FLASH_BufferRead(SSID_Buffer, 0x00000, IP_Port_BufferSize);
		printf("����������Ϊ:%s\r\n", SSID_Buffer);
		if(strstr((char*)code_ssid,(char*)SSID_Buffer))
    printf("--->>>FLASH TEST OK\r\n");	
		else printf("--->>>FLASH TEST FAIL\r\n");	
	}
}
void Flash_Write(void)//�洢APP���±�־���ж��Ƿ������Դ����APP����
{
	SPI_FLASH_SectorErase(0x01000);	 
	SPI_FLASH_BufferWrite(APP_FLAG, 0x01000, APP_FLAG_BufferSize);	
	SPI_FLASH_BufferRead(APP_FLAG_Buffer, 0x01000, APP_FLAG_BufferSize);
	printf("\r\n����������Ϊ��%s\r\n", APP_FLAG_Buffer);
}
void Flash_Writex(void)
{
	SPI_FLASH_SectorErase(0x02000);	 
}
int Flash_Readx(void)
{
		int str=0,i;
		SPI_FLASH_BufferRead(APP_FLAG_Bufferx, 0x02000, APP_FLAG_BufferSizex);
		printf("�����FLAG���ݣ�%s\r\n", APP_FLAG_Bufferx);
		printf("ʮ���ƽ�������: ");
		for(i=0;i<9;i++)printf("%d", APP_FLAG_Bufferx[i]);
		printf("\r\n");
		if(APP_FLAG_Bufferx[1]!=255&&APP_FLAG_Bufferx[2]!=255)str=1;
    //48�������0��255�������NULL�����������Ҳ����ڿ�ʱ����
	  printf("str=%d\r\n",str);
		return 	str;
}
void Flash_Read_MAC(void)
{
	SPI_FLASH_BufferRead(MAC_ID_Buffer, 0x05000, MAC_ID_BufferSize);
	printf("����������Ϊ  MAC_ADDR��%s\r\n", MAC_ID_Buffer);
}
void Flash_Read(void)
{
		SPI_FLASH_BufferRead(SSID_Buffer, 0x00000, IP_Port_BufferSize);
		printf("����������Ϊ    SSID��%s\r\n", SSID_Buffer);
		printf("  SSID_Lenth:%d\r\n",strlen((char*)SSID_Buffer));
		SPI_FLASH_BufferRead(Password_Buffer, 0x00020, Password_BufferSize);
		printf("����������Ϊ  Passrd��%s\r\n", Password_Buffer);
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
		printf("����������Ϊ    Server_IP��%s\r\n", Server_IP_Buffer);
		SPI_FLASH_BufferRead(Server_Port_Buffer, 0x07000, Password_BufferSize);
		printf("����������Ϊ  Server_Port��%s\r\n", Server_Port_Buffer);
}
void Flash_Read_Serve_IP_Port(void)
{
	  SPI_FLASH_BufferRead(Server_IP_Buffer, 0x06000, Password_BufferSize);
		printf("����������Ϊ    Server_IP��%s\r\n", Server_IP_Buffer);
		SPI_FLASH_BufferRead(Server_Port_Buffer, 0x07000, Password_BufferSize);
		printf("����������Ϊ  Server_Port��%s\r\n", Server_Port_Buffer);
}
void Write_smartconfig_flag()
{
   SPI_FLASH_SectorErase(0x03000); 
	 SPI_FLASH_BufferWrite(smartconfig_flag, 0x03000, countof(smartconfig_flag)-1); 
}
void Read_smartconfig_flag()
{
	 SPI_FLASH_BufferRead(smartconfig_flag_Buffer, 0x03000, countof(smartconfig_flag)-1); 
	 printf("����������Ϊsmart_flag��%s    \r\n", smartconfig_flag_Buffer);

}
void Write_Key_reset_flag()
{
   SPI_FLASH_SectorErase(0x04000); 
	 SPI_FLASH_BufferWrite(Key_flag, 0x04000, countof(Key_flag)-1); 
}
void Read_Key_reset_flag()
{
	 SPI_FLASH_BufferRead(Key_flag_Buffer, 0x04000, countof(Key_flag)-1); 
	 printf("����������ΪKey_flag��%s    \r\n", Key_flag_Buffer);
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
  * @brief  SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI2_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	/* ʹ��SPIʱ�� */
	FLASH_SPI_APBxClock_FUN ( FLASH_SPI_CLK, ENABLE );
	/* ʹ��SPI������ص�ʱ�� */
 	FLASH_SPI_CS_APBxClock_FUN ( FLASH_SPI_CS_CLK|FLASH_SPI_SCK_CLK|
																	FLASH_SPI_MISO_PIN|FLASH_SPI_MOSI_PIN, ENABLE );	
  /* ����SPI�� CS���ţ���ͨIO���� */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);
	
  /* ����SPI�� SCK����*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

  /* ����SPI�� MISO����*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

  /* ����SPI�� MOSI����*/
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

  /* ֹͣ�ź� FLASH: CS���Ÿߵ�ƽ*/
  SPI_FLASH_CS_HIGH();

  /* SPI ģʽ���� */
  // FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
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

  /* ʹ�� SPI  */
  SPI_Cmd(FLASH_SPIx , ENABLE);
}
 /**
  * @brief  ����FLASH����
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* ������������ָ��*/
  SPI2_FLASH_SendByte(W25X_SectorErase);
  /*���Ͳ���������ַ�ĸ�λ*/
  SPI2_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ����λ */
  SPI2_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�λ */
  SPI2_FLASH_SendByte(SectorAddr & 0xFF);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}
 /**
  * @brief  ����FLASH��������Ƭ����
  * @param  ��
  * @retval ��
  */
void SPI_FLASH_BulkErase(void)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ���� Erase */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* �����������ָ��*/
  SPI2_FLASH_SendByte(W25X_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}
 /**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
  * @retval ��
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* дҳдָ��*/
  SPI2_FLASH_SendByte(W25X_PageProgram);
  /*����д��ַ�ĸ�λ*/
  SPI2_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ����λ*/
  SPI2_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�λ*/
  SPI2_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!"); 
  }
  /* д������*/
  while (NumByteToWrite--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    SPI2_FLASH_SendByte(*pBuffer);
    /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd();
}
 /**
  * @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*��д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			
			/* ���������ظ���ַ�������� */
      WriteAddr +=  count;
      pBuffer += count;
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
 /**
  * @brief  ��ȡFLASH����
  * @param 	pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval ��
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* ���� �� ָ�� */
  SPI2_FLASH_SendByte(W25X_ReadData);
  /* ���� �� ��ַ��λ */
  SPI2_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* ���� �� ��ַ��λ */
  SPI2_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* ���� �� ��ַ��λ */
  SPI2_FLASH_SendByte(ReadAddr & 0xFF);	
	/* ��ȡ���� */
  while (NumByteToRead--) /* while there is data to be read */
  {
    /* ��ȡһ���ֽ�*/
    *pBuffer = SPI2_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  ��ȡFLASH ID
  * @param 	��
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ��ʼͨѶ��CS�͵�ƽ */
  SPI_FLASH_CS_LOW();

  /* ����JEDECָ���ȡID */
  SPI2_FLASH_SendByte(W25X_JedecDeviceID);

  /* ��ȡһ���ֽ����� */
  Temp0 = SPI2_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp1 = SPI2_FLASH_SendByte(Dummy_Byte);

  /* ��ȡһ���ֽ����� */
  Temp2 = SPI2_FLASH_SendByte(Dummy_Byte);

 /* ֹͣͨѶ��CS�ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /*�����������������Ϊ�����ķ���ֵ*/
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
 /**
  * @brief  ��ȡFLASH Device ID
  * @param 	��
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
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI2_FLASH_SendByte(Dummy_Byte));
}

 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 SPI2_FLASH_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(FLASH_SPIx , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

 /**
  * @brief  ʹ��SPI���������ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
	  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }
	
  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(FLASH_SPIx , HalfWord);

	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(FLASH_SPIx );
}

 /**
  * @brief  ��FLASH���� дʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_CS_LOW();

  /* ����дʹ������*/
  SPI2_FLASH_SendByte(W25X_WriteEnable);

  /*ͨѶ������CS�� */
  SPI_FLASH_CS_HIGH();
}

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  0x01

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� ��״̬�Ĵ��� ���� */
  SPI2_FLASH_SendByte(W25X_ReadStatusReg);

  /* ��FLASHæµ����ȴ� */
  do
  {
		/* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = SPI2_FLASH_SendByte(Dummy_Byte);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}


//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_CS_LOW();

  /* ���� ���� ���� */
  SPI2_FLASH_SendByte(W25X_PowerDown);

  /*ͨѶ������CS�� */
  SPI_FLASH_CS_HIGH();
}   

//����
void SPI_Flash_WAKEUP(void)   
{
  /*ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� �ϵ� ���� */
  SPI2_FLASH_SendByte(W25X_ReleasePowerDown);

   /* ֹͣ�ź� FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}   
   

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
   
/*********************************************END OF FILE**********************/
