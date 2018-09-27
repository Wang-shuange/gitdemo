/***************************************************************
��λ�����ϳ������ͻ�е���޹�˾
����ˣ���ΰ
���䣺104461049@qq.com

FreeModbus ͨ�� 
eMBRegInputCB eMBRegHoldingCB 
eMBRegCoilsCB	eMBRegDiscreteCB �ĸ��ӿں���������ݵĶ�д���� 
**************************************************************/
#include "stm32f10x.h"
#include "mb.h" 
#include "stdio.h"
void ENTER_CRITICAL_SECTION(void)//���볬�ٽ� �����ж�
{
	__set_PRIMASK(1);
}

void EXIT_CRITICAL_SECTION(void)//�˳����ٽ� �����ж�
{
	__set_PRIMASK(0);
}

u16 usRegInputBuf[50]={0};
//void char2int() //transmit char to int_16 atuthor wang shuange
//{
//	int i;
//	for(i=0;i<50;++i)
//	{
//		printf("ucRTU value:%0x\r\n",ucRTU_Buf[i]);
//		usRegInputBuf[i]=(ucRTU_Buf[2*i]<<8)|(ucRTU_Buf[2*i+1]);	
//		printf("usReg value:%0x\r\n",usRegInputBuf[i]);
//	}
//}

u16 *usRegHoldingBuf=usRegInputBuf;	

u8 REG_INPUT_START=0,REG_HOLDING_START=0;
//u8 REG_INPUT_NREGS=8,REG_HOLDING_NREGS=8;
u8 usRegInputStart=0,usRegHoldingStart=0;

#define  REG_INPUT_NREGS 255

#define  REG_HOLDING_NREGS 255

//�����ּĴ��� ������0x04

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;

}
// �Ĵ����Ķ�д���� ֧�ֵ�����Ϊ�� 0x03 ��д0x06

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
	  u16 *PRT=(u16*)pucRegBuffer;
    if( ( usAddress >= REG_HOLDING_START ) && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
        case MB_REG_READ:
//				   char2int();
            while( usNRegs > 0 )
            {
                *PRT++ = __REV16(usRegHoldingBuf[iRegIndex++]); //������ת REV16.W

// 				*pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
//              *pucRegBuffer++ = ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
//				iRegIndex++;
                usNRegs--;
            }
            break;

        case MB_REG_WRITE:
//						printf("MB_REG_WRITE\r\n");
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex++] = __REV16(*PRT++); //������ת REV16.W

//				usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
//              usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
//              iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
//			printf("MB_ENOREG\r\n");
    }
    return eStatus;
}

//��/д���ؼĴ���  0x01  x05

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
    return MB_ENOREG;
}

//�����ؼĴ��� 0x02
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}
