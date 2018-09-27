#ifndef __TCP_DEMO_H
#define __TCP_DEMO_H
#include "types.h"
#include "stm32f10x.h"

extern uint16 W5500_tcp_server_port;
void do_tcp_server(void);//TCP Server回环演示函数
void do_tcp_client(void);//TCP Clinet回环演示函数
void ethernet_send(char* data);
//void do_tcp_client(u8* buf,u16 len);
#endif 

