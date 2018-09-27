#ifndef  __esp8266_H
#define	 __esp8266_H

#define uchar  unsigned char 
extern char user_input[2048];        
extern char cmd[400];

unsigned int wifi_send_commandx(char *Command, char *Response, unsigned long Timeout,uchar  Retry);
unsigned int wifi_send_command(char *Command, char *Response, unsigned long Timeout,uchar  Retry);
unsigned int wifi_send_commandxx(char *Command, char *Response, unsigned long Timeout,uchar  Retry);
void wifi_send_exit(char *Command);
void wifi_send_data(char* data);
void wifi_disconnect(void);
void wifi_connect(void);
void wifi_init(char* ssid,char* pwd);
void ssid_password_jiequ(char*buffer,char*code,int len);
void smartconfig(void);
void WIFI_Reset(void);
unsigned int wifi_send_command_ReadMAC(char *Command, char *Response, unsigned long Timeout,uchar  Retry);
unsigned int wifi_send_command_ReadIP(char *Command, char *Response, unsigned long Timeout,uchar  Retry);





#endif

