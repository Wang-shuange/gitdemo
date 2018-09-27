#ifndef __param_config_H
#define	__param_config_H

void read_device_addr(void);
void write_device_addr(void);
void read_device_addr_direct(void);
void str_sub(void);
void device_addr_str_sub(void);//设备地址解析
/*************************/
void Device_ID(void);
void Write_Device_ID(void);
void Read_Device_ID(void);
void Read_Device_ID_Direct(void);
void Device_Parameter_Config(void);
void Read_Device_Parameter(void);
void Read_Device_Parameter_Direct(void);
void Ehernet_MAC(void);
void string_to_hex_ethernet(void);//for eth mac transmit use
void write_time(void);//写延时时间
void Delay_s(void);
/*************************///数据上传函数
void data_send(void);
void sprintf_scop_send(void);
void Ethernet_test_scop(void);
#endif

