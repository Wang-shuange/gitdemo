#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й����ʺ�STM32F10xϵ�У�
//����delay_us,delay_ms
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.8
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!
//V1.3�޸�˵��
//�����˶�UCOSII��ʱ��֧��.
//���ʹ��ucosII,delay_init���Զ�����SYSTICK��ֵ,ʹ֮��ucos��TICKS_PER_SEC��Ӧ.
//delay_ms��delay_usҲ���������ucos�ĸ���.
//delay_us������ucos��ʹ��,����׼ȷ�Ⱥܸ�,����Ҫ����û��ռ�ö���Ķ�ʱ��.
//delay_ms��ucos��,���Ե���OSTimeDly����,��δ����ucosʱ,������delay_usʵ��,�Ӷ�׼ȷ��ʱ
//����������ʼ������,��������ucos֮��delay_ms������ʱ�ĳ���,ѡ��OSTimeDlyʵ�ֻ���delay_usʵ��.
//V1.4�޸�˵�� 20110929
//�޸���ʹ��ucos,����ucosδ������ʱ��,delay_ms���ж��޷���Ӧ��bug.
//V1.5�޸�˵�� 20120902
//��delay_us����ucos��������ֹ����ucos���delay_us��ִ�У����ܵ��µ���ʱ��׼��
//V1.6�޸�˵�� 20150109
//��delay_ms����OSLockNesting�жϡ�
//V1.7�޸�˵�� 20150319
//�޸�OS֧�ַ�ʽ,��֧������OS(������UCOSII��UCOSIII,����������OS������֧��)
//���:delay_osrunning/delay_ostickspersec/delay_osintnesting�����궨��
//���:delay_osschedlock/delay_osschedunlock/delay_ostimedly��������
//V1.8�޸�˵�� 20150519
//����UCOSIII֧��ʱ��2��bug��
//delay_tickspersec��Ϊ��delay_ostickspersec
//delay_intnesting��Ϊ��delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
typedef char int8;

typedef volatile char vint8;

typedef unsigned char uint8;

typedef volatile unsigned char vuint8;

typedef int int16;

typedef unsigned short uint16;

typedef long int32;

typedef unsigned long uint32;

typedef uint8			u_char;		/**< 8-bit value */
typedef uint8 			SOCKET;
typedef uint16			u_short;	/**< 16-bit value */
typedef uint16			u_int;		/**< 16-bit value */
typedef uint32			u_long;		/**< 32-bit value */

uint16 atoi16(char* str,uint16 base); 			/* Convert a string to integer number */
uint32 atoi32(char* str,uint16 base); 			/* Convert a string to integer number */
void itoa(uint16 n,uint8* str, uint8 len);
int validatoi(char* str, int base, int* ret); 		/* Verify character string and Convert it to (hexa-)decimal. */
char c2d(u_char c); 					/* Convert a character to HEX */

uint16 swaps(uint16 i);
uint32 swapl(uint32 l);

void replacetochar(char * str, char oldchar, char newchar);

void mid(int8* src, int8* s1, int8* s2, int8* sub);
void inet_addr_(unsigned char* addr,unsigned char *ip);


char* inet_ntoa(unsigned long addr);			/* Convert 32bit Address into Dotted Decimal Format */
char* inet_ntoa_pad(unsigned long addr);

uint32 inet_addr(unsigned char* addr);		/* Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address */

char verify_ip_address(char* src, uint8 * ip);/* Verify dotted notation IP address string */

uint16 htons( unsigned short hostshort);	/* htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).*/

uint32 htonl(unsigned long hostlong);		/* htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian). */

uint32 ntohs(unsigned short netshort);		/* ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors). */

uint32 ntohl(unsigned long netlong);		/* ntohl function converts a u_long from TCP/IP network order to host byte order (which is little-endian on Intel processors). */

uint16 checksum(unsigned char * src, unsigned int len);		/* Calculate checksum of a stream */

uint8 check_dest_in_local(u_long destip);			/* Check Destination in local or remote */

void systick_init (u8 sysclk);
void delay_init(void);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);
#endif
