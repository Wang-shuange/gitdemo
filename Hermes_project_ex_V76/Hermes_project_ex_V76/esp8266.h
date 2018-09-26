#ifndef _ESP_8266_H
#define _ESP_8266_H
#include "Arduino.h"
#include "EEPROM.h"
#include <TimerOne.h>//ESP
   // #define  __BT_RENAME_
   // #define __PRINT__
   #define __TIME_COUNT__
 static unsigned long  Time_Cont = 0;  
 static const unsigned int rx_buffer_length = 600;
 static char rx_buffer[rx_buffer_length]={0};
 static unsigned int ii = 0;
 static void(* resetFunc) (void) = 0;
 static int count1 = 0;
 static int int_flag=0;
 static int led_flag=0;
 //使用過的Wifi存儲功能
 static int wifi_flag=0;
 static int ssid_length=0;
 static int password_length=0;
#define EEPROM_START_ADDRESS    100   // Start Address in EEPROM EEPROM_storage
#define EEPROM_SIZE             20 // EEPROM size

typedef struct _eeprom_storage
{    
	unsigned char flag=0x00;
	unsigned int Fcount=0x00;
	char* ssid ="";           
	char* password="";      
}   EEPROM_storage;
static EEPROM_storage      e2_memory;
class Esp8266{
    private:
	char* PDCServer="mlbserver.foxconn.com"; 
	public:
    unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
	 unsigned int sendCommand1(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
	unsigned int sendCommand_BT(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
	unsigned int sendCommand_ex(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
	void ESP8266_ERROR(int num);
	void EEPROM_write_block(String value, unsigned int start_address);
	bool confirm_wifi(char* ssid,char* password);
	void wireless_reset();
	void select_function();
	boolean smart_config();
	boolean test_smart();
	void wifi_init();
	void wifi_init_1();
	void wifi_reconnect();
	void wifi_control(char* deviceNum);
	unsigned int received_data(unsigned long Timeout);
	void clear_buffer();
	void read_buffer();
	void bt_control();
	void bt_init();
	void bt_rename(char* name);
	void verify_ssid_password();
		void verify_ssid_password_smart_config();
	void error_handle(int num);
	void parse_smart_config_info(String input, String & ssid, String & password);
	bool parse_string(String input,char token,String& before,String& after);
	void EEPROM_read_block(unsigned char *memory_block, unsigned int start_address, unsigned int block_size);
	void EEPROM_clear_all(unsigned int eeprom_size);
	void EEPROM_write_short(unsigned int Address, unsigned int Data);
	String EEPROM_read_block(unsigned int start_address,int length);
	unsigned int EEPROM_read_short(unsigned int Address);
	//異常LOG存儲新增2017-11-20
	void EEPROM_write_log(char value);
	String EEPROM_read_log(unsigned int startaddr,int loglength);
	int EEPROM_log_count();
	void EEPROM_clear_log(unsigned int startadd,int eeprom_size);
	//2017-11-24讀取IP&Mac地址
	String parse_self_ip(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
	String parse_self_MAC(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);
	//2018-02-27增加遠程清空密碼功能函數
	String received_info();
	//void parse_clearpassword_info(String input,String& DeviceID,String& Mac);
	//2018-03-15增加遠程修改ServerIP功能
	//bool parse_ServerIP_config(String& ServerIP,String& ServerPort);
	// String received_Server_info();
	//void parse_ServerIP_info(String input,String& Mac,String& sServerIP,String& sServerPort);
	void parse_info(String input,String& DeviceID,String& Mac,String& sServerIP,String& sServerPort);
	unsigned int SearchAndParseWifi();
	void led_blank();
};
 static Esp8266 *esp; 
#endif
