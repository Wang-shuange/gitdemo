
#include "esp8266.h"
unsigned int sendCommandx(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{

  for (unsigned char n = 0; n < Retry; n++)
  {
    Serial.print(">>> Send:");
    Serial.println(Command);
    Serial3.write(Command);
    Time_Cont = 0;
    while (Time_Cont < Timeout)
    {
      while(Serial3.available())
	  {
		  rx_buffer[ii++]=Serial3.read();
		  if(ii==rx_buffer_length)
           // clear_buffer();
	   ;
	  }
		if (strstr(rx_buffer, Response) != NULL)
		{
			// Serial.print(">>> Send:");
			// Serial.println(Command);
			Serial.print("<<< Received:");
			Serial.println(Response);       
			return 1;
		}
    }
    Time_Cont = 0;
  }
  return 0;		
}

unsigned int Esp8266::sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
   clear_buffer();
  for (unsigned char n = 0; n < Retry; n++)
  {
    Serial.print(">>> Send:");Serial.println(Command);Serial3.write(Command);
	
    Time_Cont = 0;
    while (Time_Cont < Timeout)
    {
      while(Serial3.available())
	  {
		  //delay(20);
		  rx_buffer[ii++]=Serial3.read();
		  // Serial.print("<<< Received:");
			// Serial.println(rx_buffer); 
		  if(ii==rx_buffer_length)
		  {
			  // Serial.print("<<< Received:");
			// Serial.println(rx_buffer); 
			// delay(200);
			 clear_buffer();
		  }
           //clear_buffer();
	  }
		if (strstr(rx_buffer, Response) != NULL)
		{
		
			Serial.print("<<< Received:");
			Serial.println(Response);       
			return 1;
		}
    }
    Time_Cont = 0;
  }
  return 0;		
}
unsigned int Esp8266::sendCommand1(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
    Serial.print(">>> Send:");
    Serial.println(Command);
    Serial3.write(Command);
	String buff="";
	 long int start=millis();
	
	while(millis()-start>Timeout)
	{
		Serial.println("run ok");
	  while(Serial3.available())
	  {
		  char c = Serial3.read(); 
		  buff+=c;
		  Serial.print("rec:");
		  Serial.println(buff);
	  }
	  Serial3.flush();
	  if(buff.indexOf(Response)!=-1)
	  {
		Serial.print("<<< Received:");
		Serial.println(Response);       
		return 1; 
	  }
	}

  return 0;		
}
unsigned int Esp8266::sendCommand_BT(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
   clear_buffer();
  for (unsigned char n = 0; n < Retry; n++)
  {
    Serial.print(">>> Send:");
    Serial.println(Command);
    Serial1.write(Command);
    Time_Cont = 0;
    while (Time_Cont < Timeout)
    {
      while(Serial1.available())
	  {
		  rx_buffer[ii++]=Serial1.read();
		  if(ii==rx_buffer_length)
           clear_buffer();
	  }
		if (strstr(rx_buffer, Response) != NULL)
		{
			// Serial.print(">>> Send:");
			// Serial.println(Command);
			Serial.print("<<< Received:");
			Serial.println(Response);       
			return 1;
		}
    }
    Time_Cont = 0;
  }
  return 0;		
}

void Esp8266::clear_buffer()
{
	memset(rx_buffer,0,rx_buffer_length);
	ii=0;
}

void Esp8266::read_buffer()
{
	while(Serial3.available())
	  {
		  rx_buffer[ii++]=Serial3.read();
		  if(ii==rx_buffer_length)
           clear_buffer();
	  }
}

bool Esp8266::confirm_wifi(char* ssid,char* password)
{
   #ifdef __TIME_COUNT__
	 unsigned long start=millis();
	DDRJ|=(1<<PJ2);
	PORTJ|=(0<<PJ2);
	delay(20);
	if(!sendCommand("AT+RST\r\n", "ready", 3000, 3))
	{
		EEPROM_write_log('B');
		ESP8266_ERROR(5);
	}
	clear_buffer();
	delay(2000);
	// if (!sendCommand("AT+RST\r\n", "CONNECT", 3000, 3))
	 // ESP8266_ERROR(5);
	// clear_buffer();
    if (!sendCommand("AT+CWMODE=1\r\n", "OK", 3000, 3))
    {
		EEPROM_write_log('H');
		ESP8266_ERROR(7);
	}
    clear_buffer();
	delay(3000);
	char cmd[400];
	strcpy(cmd, "AT+CWJAP=\"");
	strcat(cmd, ssid);
	strcat(cmd, "\",\"");
	strcat(cmd, password);
	strcat(cmd, "\"\r\n");
	if(!sendCommand(cmd, "WIFI GOT IP", 8000, 3))
	{
		clear_buffer();
		EEPROM_write_log('C');
		return false;
	}
	clear_buffer();
	 Serial.print("######wifi confirm time:");
	 unsigned long gap=(millis()-start)/1000;
	 Serial.print(gap);
	 Serial.println("sec.");
	 #endif
	return true; 
}
void Esp8266::ESP8266_ERROR(int num)
{
      Serial.print(">>> ERROR");
      Serial.println(num);	
	for(int i=0;i<5;++i)
	{
		delay(200);
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//R
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);

		delay(200);
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//R
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
	}
      double a=millis();
      while ((millis()-a)<5000)
      {
          delay(200);
          digitalWrite(37,1);digitalWrite(36,1);digitalWrite(35,1);
          digitalWrite(34,1);digitalWrite(33,1);digitalWrite(32,1); 
          digitalWrite(38,1);digitalWrite(41,1);digitalWrite(40,1);
          delay(200);
          digitalWrite(37,0);digitalWrite(36,1);digitalWrite(35,1);
          digitalWrite(34,0);digitalWrite(33,1);digitalWrite(32,1); 
          digitalWrite(38,0);digitalWrite(41,1);digitalWrite(40,1);
          delay(200);
      }  
       //EEPROM.write(0,0xff);
       resetFunc();  
}
void Esp8266::error_handle(int num)
{
	switch(num)
	{
		case 1:
		Serial.println(">>> wifi reset fail!");
			analogWrite(46,255);analogWrite(2,200);analogWrite(5,255);//R
			analogWrite(9,255);analogWrite(8,200);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,200);analogWrite(10,255);
		   if (!sendCommand("AT+RST\r\n", "OK", 1000, 3))
				resetFunc();
		break;
		case 2:
		break;
		case 3:
		    Serial.println(">>> wifi confirm fail!");
			delay(200);
			analogWrite(46,255);analogWrite(2,200);analogWrite(5,255);//R
			analogWrite(9,255);analogWrite(8,200);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,200);analogWrite(10,255);

			delay(200);
			analogWrite(3,255);analogWrite(2,255);analogWrite(5,255);//R
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);

			delay(200);
			analogWrite(3,255);analogWrite(2,200);analogWrite(5,255);//R
			analogWrite(9,255);analogWrite(8,200);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,200);analogWrite(10,255);
			delay(200);
			analogWrite(3,255);analogWrite(2,255);analogWrite(5,255);//R
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
			delay(200);
			if (!sendCommand("AT+RST\r\n", "OK", 1000, 3))
				// ESP8266_ERROR(3);
			resetFunc();
			
		break;
		case 4:
		break;
		case 5:
		break;
		case 6:
		break;
		default:
		break;
		
	}
      Serial.print(">>> ERROR");
      Serial.println(num); 
      EEPROM.write(100,0xff);
       resetFunc();  
}
void Timer1_handler(void)
{
  Time_Cont++;
}
/*************smart config****2017-07-01**********************/
boolean Esp8266::smart_config()
{
	// for(int i=0;i<5;i++)
	// {
		// delay(100);
			// analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
			// analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
			// analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3
		// delay(100);
	        // analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);
			// analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			// analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
	// }
	//RED LED ON,until smartconfig ok
	// analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
	// analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
	// analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3

	 delay(800);
	 Serial3.begin(115200);//打開wifi通道UART3
	 Timer1.initialize(1000);
	 Timer1.attachInterrupt(Timer1_handler);
	// /**********************************/
	// /*SMART CONFIG 步驟：1.復位wifi模組;2.設置工作模式為station;3.打開smart;
	// /**********************************/
    
  if(!sendCommand("AT+RST\r\n", "ready", 3000, 3))
  {
	  EEPROM_write_log('B');
	  ESP8266_ERROR(5);
  }
  clear_buffer();
  delay(2000);
  if (!sendCommand("AT+CWMODE=1\r\n", "OK", 3000, 3))
  {
	  EEPROM_write_log('H');
	  ESP8266_ERROR(7);
  }
  clear_buffer();
   delay(1000);
   for(int i=0;i<5;i++)
	{
		delay(100);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3
		delay(100);
	        analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
	}
	//RED LED ON,until smartconfig ok
	analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
	analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
	analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3
	delay(500);
	 sendCommand_ex("AT+CWSTARTSMART\r\n", "smartconfig connected wifi", 60000, 10);
	return true;

}

boolean Esp8266::test_smart()
{
	//delay(1000);
	int_flag=(char)EEPROM.read(120);
	//delay(1000);
	Serial.print("Flag:");
	Serial.println(int_flag);
   if(int_flag==1)
   {
	  if(!esp->smart_config())
		  return false;
	  else
	  {
		  int_flag=0;
		  EEPROM.write(120,int_flag);
		  return true;   
	  }
		  
   }
  return false;
}

void handle(void)
{
   count1++;
   delay(800);//增加延時消除按鍵抖動
   if (digitalRead(3))
   {  
	 if (count1 > 500)
     {
		 count1=0;
		 int_flag = 0;
		 EEPROM.write(120,int_flag);
		 EEPROM.write(0,0xFF);
         EEPROM.write(3,0);
		 EEPROM.write(500,0);
		 resetFunc();
	 }
     if (count1 > 90)
     {
		 count1=0;
		 int_flag = 1;
		 EEPROM.write(120,int_flag);  
	 }
	 if (count1 > 5)
     {
	   count1=0; 
       int_flag = 2;
	   resetFunc();
     }
  }
}

void Esp8266::select_function()
{
	Timer1.initialize(1000); 
    Timer1.attachInterrupt(Timer1_handler);
	pinMode(3, OUTPUT);
	digitalWrite(3, HIGH);
	attachInterrupt(1, handle, LOW); //当int.0电平改变时,触发中断函数blink

}
  void Esp8266::parse_smart_config_info(String input, String & ssid, String & password)
  {
    String s = input;
    if (s.indexOf("ssid") != -1 && s.indexOf("password") != -1)
    {
	   ssid = s.substring(s.indexOf("ssid:") + 5, s.indexOf("\r\npassword"));
       password = s.substring(s.indexOf("password:") + 9, s.indexOf("\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\nsmartconfig connected wifi"));
	 }
  }
   
  void Esp8266::parse_info(String input,String& DeviceID,String& Mac,String& sServerIP,String& sServerPort)
  {
	  String s=input;
	  if(s.indexOf("HermesCommand#")!=-1 && s.lastIndexOf("*")!=-1)
	  {
		  s=s.substring(s.indexOf("HermesCommand#"),s.indexOf("*"));
		  DeviceID=s.substring(s.indexOf("HermesCommand#")+14,s.indexOf(","));
		  Mac=s.substring(s.indexOf(",")+1,s.indexOf("*"));
		  Serial.print("DeviceID=");
		  Serial.println(DeviceID);
		  Serial.print("Mac=");
		  Serial.println(Mac);  
	  }
	  if(s.indexOf("ChangeServer#")!=-1 && s.lastIndexOf("*")!=-1)
	  {
		  s=s.substring(s.indexOf("ChangeServer#"),s.indexOf("*"));
		  Mac=s.substring(s.indexOf("ChangeServer#")+13,s.indexOf(","));
		  s=s.substring(s.indexOf(",")+1,s.indexOf("*"));
		  sServerIP=s.substring(0,s.indexOf(","));
		  sServerPort=s.substring(s.indexOf(",")+1,s.indexOf("*"));
		  Serial.print("Mac=");
		  Serial.println(Mac);  
		  Serial.print("ServerIP=");
		  Serial.println(sServerIP);
		  Serial.print("ServerPort=");
		  Serial.println(sServerPort);
	  }
  }
  
String Esp8266::received_info()
{
	clear_buffer();
	Time_Cont = 0;
	String s;
	String s1;
	String s2;
	String s3;
    // while(Time_Cont < Timeout)
	// {
      read_buffer();
	  Serial.print("<<< Received:");
	   Serial.println(rx_buffer);
	  String input(rx_buffer);
	  s=input;
	  if(s.indexOf("HermesCommand#")!=-1 && s.lastIndexOf("*")!=-1)
	  {
		  s1=s;
		  return s1;
	  }
	  if(s.indexOf("ChangeServer#")!=-1 && s.lastIndexOf("*")!=-1)
	  {
		  s2=s;
		  return s2;
	  }
	  
	  //add by lujunxian20180604  (add location information)
	  if(s.indexOf("Location#")!=-1 && s.lastIndexOf("*")!=-1)
	  {
		  s3=s;
		  return s3;
	  }
	//}
	Time_Cont = 0;
	return s;
}


unsigned int Esp8266::received_data(unsigned long Timeout)
{
    Time_Cont = 0;
    while (Time_Cont < Timeout)
    {
      while(Serial3.available())
	  {
		  rx_buffer[ii++]=Serial3.read();
		  if(ii==rx_buffer_length)
           clear_buffer();
	  }
	  Serial.print("<<< Received:");
	   Serial.println(rx_buffer);
	  String input(rx_buffer);
	  String ssid,password;
	  parse_smart_config_info(input, ssid,password);	
    }
    Time_Cont = 0;

  return 0;		
}
  
unsigned int Esp8266::sendCommand_ex(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	unsigned char ssidlen=EEPROM.read(3);
	unsigned char WifiNum=EEPROM.read(500);
	unsigned char sameflag=0;
	Serial.println(WifiNum);
  while(1)
  {
    Serial.print(">>> Send:");
    Serial.println(Command);
    Serial3.write(Command);
    Time_Cont = 0;
    while (Time_Cont < Timeout)
    {
      while(Serial3.available())
	  {
		  rx_buffer[ii++]=Serial3.read();
		  if(ii==rx_buffer_length)
           clear_buffer();
	  }
	     String buf(rx_buffer);
		if (buf.indexOf("Smart get wifi info")!=-1&&buf.indexOf("smartconfig connected wifi")!=-1)
		{
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//LED OFF
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
			String ssid,password;
			parse_smart_config_info(buf, ssid,password);
			Serial.print("[ssid]");
			Serial.println(ssid);
			Serial.print("[password]");
			Serial.println(password);
	        delay(100);
          if(esp->confirm_wifi((char*)ssid.c_str(),(char*)password.c_str()))  
           {        
			analogWrite(46,255);analogWrite(2,200);analogWrite(5,255);//wifi connection ok,green led open;
			analogWrite(9,255);analogWrite(8,200);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,200);analogWrite(10,255);
			delay(100);
			EEPROM.write(0,0xf0);//save flag
			int ssid_len=ssid.length();
			delay(100);
			EEPROM.write(3,ssid_len);//save ssid length
			int password_len=password.length();
			delay(100);
			EEPROM.write(4,password_len);//save password length
			delay(100);
			EEPROM_write_block(ssid, 10);//ssid flag;
			delay(100);
			EEPROM_write_block(password, 100);
			delay(100);
			int_flag=0;
			EEPROM.write(120,int_flag);
			delay(100);
			unsigned int ssidlenloc=501+WifiNum*110;
			unsigned int psdlenloc=502+WifiNum*110;	
			unsigned int ssidloc=503+WifiNum*110;     //SSID起始位置	
			unsigned int psdloc=540+WifiNum*110;   //password起始位置	
			// String sid;
			// String sd;
			delay(200);
			
			for(int j=0;j<WifiNum;j++)
			{
				String sid="";
				String sd="";
				unsigned int sidlenloc=501+110*j;
				unsigned int sidloc=503+110*j;
				unsigned int sdlenloc=502+110*j;
				unsigned int sdloc=540+110*j;
				delay(500);
				sid=EEPROM_read_block(sidloc,sidlenloc);
				delay(200);
				sd=EEPROM_read_block(sdloc,sdlenloc);
				delay(500);
				Serial.print(sid);
				delay(200);
				Serial.print(sd);
				delay(200);
				if((ssid.compareTo(sid)==0) && (password.compareTo(sd)==0))
				//if((sid==ssid) && (sd==password))  //用戶名及密碼一樣，無需再存儲
				{
					sameflag=1;
					Serial.print("------->>");
					Serial.println(sameflag);
					delay(100);
					break;
				}
			}
			if(sameflag!=1)
			{
				if(WifiNum==10)
				{
					EEPROM.write(501,ssid_len);
					delay(100);
					EEPROM.write(502,password_len);
					delay(100);
					EEPROM_write_block(ssid, 503);
					delay(100);
					EEPROM_write_block(password,540);
				}
				else
				{
					EEPROM.write(ssidlenloc,ssid_len);
					delay(100);
					EEPROM.write(psdlenloc,password_len);
					delay(100);
					EEPROM_write_block(ssid, ssidloc);
					delay(100);
					EEPROM_write_block(password,psdloc);
					delay(100);
					WifiNum=WifiNum+1;
					EEPROM.write(500,WifiNum);
				}
			}
			delay(100);
			Serial.println(">>> wifi confirm OK");
			delay(100); 
			resetFunc(); //重啟單片機，保存在e2prom中的數據方可生效，實驗證明
           } else
		   {
			   EEPROM_write_log('E');
			   delay(100);
			if(ssidlen>0)
			{
				int_flag=0; 
				EEPROM.write(120,int_flag);
			}
			else
			{
				int_flag=0; 
				EEPROM.write(120,int_flag);
				EEPROM.write(0,0xFF);
			}
			delay(100);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//wifi connection fail ,red led 
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
			Serial.println(">>> wifi confirm fail,input again");
			 delay(100);
			 // resetFunc(); 
		   }
		}
		else
		{
			if(ssidlen>0)
			{
				int_flag=0; 
				EEPROM.write(120,int_flag);
			}
			else
			{
				int_flag=0; 
				EEPROM.write(120,int_flag);
				EEPROM.write(0,0xFF);
			}
		}
    }
    Time_Cont = 0;
  }
  return 0;		
}
/***************************************/
unsigned int Esp8266::SearchAndParseWifi()
{
	unsigned int EWifiNum=EEPROM.read(500);
	for(int i=0;i<EWifiNum;i++)
	{
		unsigned int Essidlenloc=501+110*i;
		unsigned int Essidloc=503+110*i;
		unsigned int Epsdlenloc=502+110*i;
		unsigned int Epsdloc=540+110*i;
		delay(200);
		unsigned int Essidlen=EEPROM.read(Essidlenloc);
		delay(100);
		String Essid=EEPROM_read_block(Essidloc,Essidlen);
		delay(100);
		unsigned int Epsdlen=EEPROM.read(Epsdlenloc);
		delay(100);
		String Epsd=EEPROM_read_block(Epsdloc,Epsdlen);
		delay(100);
		String buf(rx_buffer);
		if(buf.indexOf(Essid)!=-1)
		{
			String temp=buf.substring(buf.indexOf(Essid)+Essid.length(),buf.indexOf(Essid)+Essid.length()+1);
			if(temp=="\"")
			{
				delay(100);
				EEPROM.write(3,Essidlen);//save ssid length
				delay(100);
				EEPROM.write(4,Epsdlen);//save password length
				delay(100);
				EEPROM_write_block(Essid,10);
				delay(100);
				EEPROM_write_block(Epsd,100);
				return 1;
			}
		}
	}
	return 0;
}
/***************************************/
void Esp8266::wifi_init()
{
	 #ifdef __TIME_COUNT__
	 unsigned long start=millis();
	 #endif
	DDRJ|=(1<<PJ2);
	PORTJ|=(0<<PJ2);
   delay(20);
  Serial3.begin(115200);
  Timer1.initialize(1000); 
  Timer1.attachInterrupt(Timer1_handler);
  if(!sendCommand("AT+RST\r\n", "ready", 3000, 3))
  {
	  EEPROM_write_log('B');
	  ESP8266_ERROR(5);
  }
  clear_buffer();
  delay(2000);
  if (!sendCommand("AT+CWMODE=1\r\n", "OK", 3000, 3))
  {
	  EEPROM_write_log('H');
	  ESP8266_ERROR(7);
  }
  clear_buffer();
  delay(3000);
  char cmd[400];
   strcpy(cmd, "AT+CWJAP=\"");
   delay(100);
	unsigned int s_l=EEPROM.read(3);
	delay(100);
	unsigned int p_l=EEPROM.read(4);
	  delay(100);
	String ssid=EEPROM_read_block(10,s_l);
	  delay(100);
	String password=EEPROM_read_block(100,p_l);
	  delay(100);
	if(ssid.length()==0||password.length()==0)
	{
		Serial.println("--->wifi data not valid");
		EEPROM_write_log('C');
		 ESP8266_ERROR(8);
	}
  strcat(cmd, ssid.c_str());
  strcat(cmd, "\",\"");
  strcat(cmd, password.c_str());
  strcat(cmd, "\"\r\n");
  if (!sendCommand(cmd, "WIFI GOT IP", 8000, 3))
  {
	  delay(1000);
	  //EEPROM_write_log('C');
	  //查看500位已存儲的Wifi個數，搜索周圍可用Wifi，然後進行檢索
	  if(!sendCommand("AT+CWLAP\r\n","OK",8000,3))
	  {
		  EEPROM_write_log('K');
		  ESP8266_ERROR(10);
	  }
	  else
	  {
		  if(!SearchAndParseWifi())
		  {
			  EEPROM_write_log('L');
			  ESP8266_ERROR(11);
		  }
		  else
		  {
			  resetFunc();
		  }
	  }
	  //ESP8266_ERROR(8);
  }
 clear_buffer();
 // delay(2000);
  if (!sendCommand("AT+CIPMUX=0\r\n", "OK", 3000, 3))
  ESP8266_ERROR(9);
 clear_buffer();
  if (!sendCommand("AT+CIFSR\r\n", "OK", 20000, 1))
  ESP8266_ERROR(10);
   clear_buffer();
   //add by lujunxian 20180316
   memset(cmd, 0, 400);  
   delay(100);
   unsigned int ServerIP_len=EEPROM.read(450);
   delay(100);
   unsigned int ServerPort_len=EEPROM.read(480);
   delay(100);
   String sSerIP=EEPROM_read_block(455,ServerIP_len);
   delay(100);
   String sPort=EEPROM_read_block(490,ServerPort_len);
   delay(100);
   if((sSerIP.length()==0) || (sPort.length()==0))
   {
	   // strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
	   // strcat(cmd, "mlbserver.foxconn.com");
	   // strcat(cmd, "\",1080\r\n");
	    strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
  strcat(cmd, "10.199.61.250");
   //strcat(cmd, "10.251.142.175");
  strcat(cmd, "\",8443\r\n");
   }
   else
   {
	   strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
	   strcat(cmd, sSerIP.c_str());
	   strcat(cmd, "\",");
	   strcat(cmd, sPort.c_str());
	   strcat(cmd, "\r\n");
   }
  if(!sendCommand(cmd, "CONNECT", 3000, 3))
  {
	  EEPROM_write_log('D');
	  ESP8266_ERROR(1);
  }
  
  clear_buffer();
  #ifdef __TIME_COUNT__
  	 Serial.print("######wifi config time:");
	 unsigned long gap=(millis()-start)/1000;
	 Serial.print(gap);
	 Serial.println("sec.");
	 #endif
}
void Esp8266::wireless_reset()
{
	DDRJ|=(1<<PJ3);
	DDRJ|=(1<<PJ5);
	PORTJ|=(0<<PJ3);
	delay(10);
	PORTJ|=(1<<PJ3);
	PORTJ|=(0<<PJ5);
	delay(10);
    PORTJ|=(1<<PJ5);
	// DDRL|=(1<<PL7);
	// PORTL|=(1<<PL7);
	delay(1000);
}
void Esp8266::bt_rename(char* name)
{
	char cmd[64];
	memset(cmd,0,64);	
	strcpy(cmd,"AT+name");
	strcat(cmd,name);
	strcat(cmd,"\r\n");
	sendCommand_BT(cmd, "OK", 3000, 3);
    clear_buffer();
}
void Esp8266::bt_init(){
	// Serial.begin(9600);
	// Serial1.begin(9600);//打開藍牙通道UART1
	// DDRL =0x80;  //設置PL7輸出方向為OUT
	// PORTL =0x80;  //設置PORTL7輸出高電平
	// delay(1000);
	
	/////////////////////////////////
	
		Serial.begin(9600);
	Serial1.begin(9600);
	DDRL =0x80;  //BT
	PORTL =0x80;  //1 1
	delay(1000);
	
	if(sendCommand_BT("AT\r\n", "OK", 1000, 3))
     Serial.println("--->bt reset OK");
    else
	 Serial.println("--->bt reset fail");	
	
	
	//////////////////////////
	
	// #ifdef __BT_RENAME_
   // bt_rename("DVT5-28");//rename BT device id for connect use
   // #endif
	char buffer[100]; 
     Serial.println(">>> waiting for iphone to scan BT device ID..."); 	
	analogWrite(3,200);analogWrite(2,255);analogWrite(5,255);// BT received ssid and password,Blue led open
	analogWrite(9,200);analogWrite(8,255);analogWrite(7,255);
	analogWrite(12,200);analogWrite(11,255);analogWrite(10,255);
	while(1)
	{
		  Serial1.println("BT initial");  
		    Serial.println("BT initial");
		while (Serial1.available() > 0)  
		{
	       Serial.println("bt is channel  is ok!");
		  int index = 0;
		  delay(100); //延时等待串口收完数据，否则刚收到1个字节时就会执行后续程序
		  int numChar = Serial1.available(); 
		   while(numChar--)
		  {
			buffer[index++] = Serial1.read();  //将串口数据一字一字的存入缓冲
		  }
		   String input(buffer);
		   String ssid,password;
		   if(parse_string(input,'#',ssid,password))
		   {
			    Serial1.println("OK");
				Serial1.println("OK");
				Serial1.println("OK");
				Serial1.println("OK");	
                Serial.println(">>> send OK to iphone");				
		   }
		   ssid.trim();
		   password.trim();
		   // #ifdef __DEBUG__
		   Serial.print(">>> ssid:");
		   Serial.println(ssid);
		   Serial.print(">>> password:");
		   Serial.println(password);
		   Serial.println(">>> parse wifi ssid and password successful!");
			analogWrite(3,255);analogWrite(2,255);analogWrite(5,255);// bt ok,black led 
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);  
		     PORTL =0x00;  //設置PORTL7輸出低電平
			Serial3.begin(115200);
			Timer1.initialize(1000);
			Timer1.attachInterrupt(Timer1_handler);  
		   if(esp->confirm_wifi((char*)ssid.c_str(),(char*)password.c_str()))  
           {        
				analogWrite(3,255);analogWrite(2,200);analogWrite(5,255);//wifi connection ok,green led open;
				analogWrite(9,255);analogWrite(8,200);analogWrite(7,255);
				analogWrite(12,255);analogWrite(11,200);analogWrite(10,255);
				delay(100);
				 EEPROM.write(0,0xf0);//save flag
				 int ssid_len=ssid.length();
				 EEPROM.write(3,ssid_len);//save ssid length
				 int password_len=password.length();
				 EEPROM.write(4,password_len);//save password length
				 EEPROM_write_block(ssid, 10);//ssid flag;
				 EEPROM_write_block(password, 100);
				Serial.println(">>> wifi confirm OK");
				delay(100);
				resetFunc(); //重啟單片機，保存在e2prom中的數據方可生效，實現證明
           } else
		   {
			analogWrite(3,255);analogWrite(2,255);analogWrite(5,200);//wifi connection fail ,red led 
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
			Serial.println(">>> wifi confirm fail,will restart");
			 delay(100);
			 resetFunc(); 
		   }
           return;	   
	}
  }
}
void Esp8266::bt_control(){
      String buf="";
      while (Serial2.available())  
        {
            buf +=Serial2.read();
            delay(2);
        }
        if (buf.length() > 0)
        {
            if(buf.indexOf("A1")!=-1)
			{
				Serial.println(buf);
		        digitalWrite(44,0);
			    delay(200);
			    digitalWrite(44,1);
			    digitalWrite(43,1);  
			}
            if(buf.indexOf("A2")!=-1)
			{
			    Serial.println(buf);
		        digitalWrite(43,0); 
		     }
            buf = "";
        }
}

bool Esp8266::parse_string(String input,char token,String& before,String& after)
{
   if (input.lastIndexOf(token) != -1 && input.indexOf(token) != -1)
  {
    input.remove(input.indexOf(token),1);
    before=input.substring(0,input.indexOf(token));
    input.remove(0, input.indexOf(token)+1);
    after=input.substring(0,input.indexOf(token));
    return true;
  }
  return false;
}

void Esp8266::EEPROM_write_block(String value, unsigned int start_address)
{
	Serial.print("value length:");
	Serial.println(value.length());
	for(int i=0;i<value.length();++i)
    EEPROM.write(i+start_address,value[i]);
}


String Esp8266::EEPROM_read_block(unsigned int start_address,int length)
{
     String ret="";
	for(int i=0;i<length;++i)
    ret+=(char)(EEPROM.read(i+start_address));
    return ret;
}



// void Esp8266::EEPROM_read_block(unsigned char *memory_block, unsigned int start_address, unsigned int block_size)
// {
   // unsigned int Count = 0;
   // Serial.print("block_size:");
   // Serial.println(block_size);
   // Serial.print("start_address:");
   // Serial.println(start_address);
   // for(int i=0;i<20;++i)
	   
   // EEPROM.write(i,'a');
   // for (Count=0; Count<(block_size+1); Count++)
   // {
	 // unsigned char a= EEPROM.read(start_address + Count);
	  // Serial.print("--->");
	   // delay(10);
	  // Serial.println(a,HEX);
   // }
      // for (Count=0; Count<2048; Count++)
   // {
	 // unsigned char a= EEPROM.read(Count);
	  // Serial.print("--->");
	   // delay(10);
	  // Serial.println(a);
   // }
// }
void Esp8266::EEPROM_clear_all(unsigned int eeprom_size)
{
   unsigned int Count = 0;
   unsigned char data = 0xff;
   for (Count=0; Count<eeprom_size; Count++)
   {  
       EEPROM.write(Count, data);
   }
}

// Write an uint value to EEPROM
void Esp8266::EEPROM_write_short(unsigned int Address, unsigned int Data)
{
   unsigned int DataL = Data&0x00FF;
   unsigned int DataH = Data>>8;
   EEPROM.write(Address,   DataL);
   EEPROM.write(Address+1, DataH);
}                      

// Read an uint value from EEPROM
unsigned int Esp8266::EEPROM_read_short(unsigned int Address)
{
   unsigned int DataL = EEPROM.read(Address);
   unsigned int DataH = EEPROM.read(Address+1);
   return((DataH<<8) + DataL);
}

void Esp8266::verify_ssid_password()
{
   unsigned char a=EEPROM.read(0);//flag
     if(a==0xFF)
	{
		EEPROM.write(1,0x01);//  FF FF FF F0  --->fcount
		EEPROM.write(2,0); //init 
		EEPROM.write(3,0);//ssid length
		EEPROM.write(4,0);//password length
		Serial.println(">>> First start,will start up BT4.0!");
		delay(100);//wait for atmega2560,must be 100 ms or more;
		bt_init();
    } 
	else
    Serial.println(">>> The ssid and password is already exist");  
}
void Esp8266::verify_ssid_password_smart_config()
{
     unsigned char a=EEPROM.read(0);//flag
     if(a==0xFF)
	{
		EEPROM.write(1,0x01);//  FF FF FF F0  --->fcount
		EEPROM.write(2,0); //init 
		EEPROM.write(3,0);//ssid length
		EEPROM.write(4,0);//password length
		EEPROM.write(150,0x00);
	    Serial.println(">>> First run,config ssid and password in APP,then hold reset button over 3 seconds to start smart_config!");
		analogWrite(46,200);analogWrite(2,255);analogWrite(5,255);// LED1 BLUE OPEN
		analogWrite(9,200);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,200);analogWrite(11,255);analogWrite(10,255);
		delay(100);//wait for atmega2560,must be 100 ms or more;
		while(1)
		{
			if (!digitalRead(3))
			test_smart();
		}
    } else
    Serial.println(">>> The ssid and password is already exist");  
}
void Esp8266::wifi_control(char* deviceNum){
  String comdata1;
      // Serial.print(">>> wifi_recieve cmd:");
         while (Serial3.available() > 0)  
        {
            comdata1 += char(Serial3.read());
            delay(2);
        }
		Serial.println(comdata1);
        if (comdata1.length() > 0)
        {
            Serial.println(comdata1);
            if(comdata1.indexOf("A1")!=-1)
			{
				Serial.println(">>> A1");
				digitalWrite(44,0);
				delay(200);
				digitalWrite(44,1);
				digitalWrite(43,1);  
			}
            if(comdata1.indexOf("A2")!=-1)
			{
				Serial.println(">>> A2");
				digitalWrite(43,0); 
			} 
            if(comdata1.indexOf(deviceNum)!=-1)
			{
				if(comdata1.indexOf("A601")!=-1)
				{
					Serial.println(">>> A601");
					EEPROM.write(1,1);
				}
				if(comdata1.indexOf("A06")!=-1)
				{
					Serial.println(">>> A06");
					EEPROM.write(1,5);
				}
				if(comdata1.indexOf("A07")!=-1)
				{
				  Serial.println(">>> A07");
				  EEPROM.write(1,11);
				 }
				if(comdata1.indexOf("A08")!=-1)
				{
					Serial.println(">>> A08");
					  EEPROM.write(1,16);
				}
				if(comdata1.indexOf("A09")!=-1)
				{
					Serial.println(">>> A09");
					EEPROM.write(1,33);
				}
				if(comdata1.indexOf("A10")!=-1)
				{
					Serial.println(">>> A10");
		          EEPROM.write(1,65);
				}
				if(comdata1.indexOf("A11")!=-1)
				{
					Serial.println(">>> A11");
					EEPROM.write(1,200);
				}
           }
            comdata1 = "";
        }
}


void Esp8266::wifi_reconnect()
{
	int count=0;
	while(1)
	{
		count++;
		if(count==15)
		{
			count=0;
			delay(100);
			resetFunc();
		}
		for(int i=0;i<5;i++)
		{
		delay(100);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3
		delay(100);
	        analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		}
		 esp->test_smart();
		Serial3.begin(115200);
		Timer1.initialize(1000);
		Timer1.attachInterrupt(Timer1_handler);
		DDRJ|=(1<<PJ3);
		PORTJ|=(0<<PJ3);
		delay(10);
		PORTJ|=(1<<PJ3);
		delay(10000);//等待wifi 模组复位
			clear_buffer();
			if(sendCommand("AT+CWMODE=1\r\n", "OK", 1000, 1))
			{
				clear_buffer();
				delay(100);
				unsigned int s_l=EEPROM.read(3);
				delay(100);
				unsigned int p_l=EEPROM.read(4);
				delay(100);
				String ssid=EEPROM_read_block(10,s_l);
				delay(100);
				String password=EEPROM_read_block(100,p_l);
				delay(100);
				 char cmd[400];
				strcpy(cmd, "AT+CWJAP=\"");
				strcat(cmd,ssid.c_str());
				strcat(cmd, "\",\"");
				strcat(cmd, password.c_str());
				strcat(cmd, "\"\r\n");
				if(sendCommand(cmd, "WIFI GOT IP", 6000, 1))
				{
					clear_buffer();
					if(sendCommand("AT+CIPMUX=0\r\n", "OK", 5000, 1))
					{	
		              clear_buffer();
					  memset(cmd, 0, 400);  
					  delay(100);
					  unsigned int ServerIP_len=EEPROM.read(450);
					  delay(100);
					  unsigned int ServerPort_len=EEPROM.read(480);
					  delay(100);
					  String sSerIP=EEPROM_read_block(455,ServerIP_len);
					  delay(100);
					  String sPort=EEPROM_read_block(490,ServerPort_len);
					  delay(100);
					  if((sSerIP.length()==0) || (sPort.length()==0))
					  {
						  // strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
						  // strcat(cmd, "mlbserver.foxconn.com");
						  // strcat(cmd, "\",1080\r\n");
						  strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
  strcat(cmd, "10.199.61.250");
   //strcat(cmd, "10.251.142.175");
  strcat(cmd, "\",8443\r\n");
					  }
					  else
					  {
						  strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
						  strcat(cmd, sSerIP.c_str());
						  strcat(cmd, "\",");
						  strcat(cmd, sPort.c_str());
						  strcat(cmd, "\r\n");
					  }
						 // memset(cmd, 0, 400);  
						// strcpy(cmd, "AT+CIPSTART=\"TCP\",\"");
						// strcat(cmd, "10.199.28.206");
						// strcat(cmd, "\",8443\r\n");
						//strcat(cmd, "\",1080\r\n");
						if(sendCommand(cmd, "CONNECT", 3000, 1))
						{
							clear_buffer();
							break;
						}
						else
						{
							for(int i=0;i<5;i++)
							{
							delay(100);
								analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
								analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
								analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3
							delay(100);
								analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);
								analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
								analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
							}
							EEPROM_write_log('D');
							
						}
					  
				   }
			  }
			  else
			  {
				  for(int i=0;i<5;i++)
					{
					delay(100);
						analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
						analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
						analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3
					delay(100);
						analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);
						analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
						analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
					}
				  EEPROM_write_log('C');
			  }
			}
			else
			{
				for(int i=0;i<5;i++)
				{
				delay(100);
					analogWrite(46,255);analogWrite(2,255);analogWrite(5,200); //LED1
					analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);//LED2
					analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);//LED3
				delay(100);
					analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);
					analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
					analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
				}
				EEPROM_write_log('H');
			}				
	} 
}

void Esp8266::EEPROM_write_log(char value)
{
	unsigned int logaddr;
	unsigned char iCount;
	iCount=(unsigned char)EEPROM.read(150);
	Serial.print("read rom 150bit:");
	Serial.println(iCount);
	if((iCount>=255) || (iCount<0))
	{
		for(int i=151;i<=405;i++)
			EEPROM.write(i,0xff);
		iCount=0;
	}
	iCount++;
	Serial.print("write rom icount:");
	Serial.println(iCount);
	EEPROM.write(150,iCount);
	logaddr=150+iCount;
	Serial.print("write rom logaddr:");
	Serial.println(logaddr);
	Serial.print("write rom logvalue:");
	Serial.println(value);
	EEPROM.write(logaddr,value);
}

int Esp8266::EEPROM_log_count()
{
	unsigned int loglen;
	loglen=(char)EEPROM.read(150);
	Serial.print("write rom log num:");
	Serial.println(loglen);
	return loglen;
}

String Esp8266::EEPROM_read_log(unsigned int startaddr,int readlength)
{
	String strlog="";
	for(int j=0;j<(readlength-1);j++)
		strlog=strlog+(char)(EEPROM.read(startaddr+j))+",";
	strlog=strlog+(char)EEPROM.read(startaddr+readlength-1);
	return strlog;
}

void Esp8266::EEPROM_clear_log(unsigned int startadd,int eeprom_size)
{
	unsigned int icount=0;
	unsigned char eeprom_data=0xff;
	for(icount=startadd;icount<eeprom_size;icount++)
	{
		EEPROM.write(icount,eeprom_data);
	}
}

String Esp8266::parse_self_ip(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	String sub;
	String subip;
  clear_buffer();
  for (unsigned char n = 0; n < Retry; n++)
  {
    Serial.print("\r\nsend AT Command:\r\n----------\r\n");
    Serial.println(Command);
    Serial3.write(Command);
    Time_Cont = 0;
    while (Time_Cont < Timeout)
    {
       read_buffer();
       Serial.print("recv:");
       Serial.println(rx_buffer);
      String strBuf(rx_buffer);
      if ((strBuf.indexOf("+CIFSR:STAIP") != -1) && (strBuf.indexOf("+CIFSR:STAMAC") != -1))
      {
       sub = strBuf.substring(strBuf.indexOf("+CIFSR:STAIP") + strlen("+CIFSR:STAIP") + 2, strBuf.indexOf("+CIFSR:STAMAC")-1);
       subip=sub.substring(0,sub.length()-2);
	   Serial.print("--->IP token:");
       Serial.println(subip);
	   return subip;
      }
    }
    Time_Cont = 0;
  }
  return subip;
}

String Esp8266::parse_self_MAC(char *Command, char *Response, unsigned long Timeout, unsigned char Retry)
{
	String subtemp;
	String subMAC;
  clear_buffer();
  for (unsigned char n = 0; n < Retry; n++)
  {
    Serial.print("\r\nsend AT Command:\r\n----------\r\n");
    Serial.println(Command);
    Serial3.write(Command);
    Time_Cont = 0;
    while (Time_Cont < Timeout)
    {
       read_buffer();
       Serial.print("recv:");
       Serial.println(rx_buffer);
      String strBuf(rx_buffer);
      if ((strBuf.indexOf("+CIPSTAMAC") != -1))
      {
       subtemp = strBuf.substring(strBuf.indexOf("+CIPSTAMAC:\"") + strlen("+CIPSTAMAC:\""));
	   subMAC=subtemp.substring(0,subtemp.indexOf("\""));
	   Serial.print("--->MAC token:");
       Serial.println(subMAC);
	   return subMAC;
      }
    }
    Time_Cont = 0;
  }
  return subMAC;
}

// bool Esp8266::parse_ServerIP_config(String& ServerIP,String& ServerPort)
// {
	// unsigned int ServerIP_len=EEPROM.read(450);
	// unsigned int ServerPort_len=EEPROM.read(480);
	// if((ServerIP_len>0) && (ServerPort_len>0))
	// {
		// delay(100);
		// String sIP=EEPROM_read_block(455,ServerIP_len);
		// ServerIP=sIP;
		// delay(100);
		// String sPort=EEPROM_read_block(490,ServerPort_len);
		// ServerPort=sPort;
		// return true;
	// }
	// return false;
// }
