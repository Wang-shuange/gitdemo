 
#include "hermes.h"
void Hermes::status_show(int color)
{
	//total 3 leds
    pinMode(3,OUTPUT);pinMode(2,OUTPUT);pinMode(5,OUTPUT);//led1:PE 5 4 3 /3 2 5
	pinMode(9,OUTPUT);pinMode(8,OUTPUT);pinMode(7,OUTPUT);	//led2:PH 6 5 4 /9 8 7
	pinMode(12,OUTPUT);pinMode(11,OUTPUT);pinMode(10,OUTPUT);//led3:PB 6 5 4 /12 11 10 
	switch(color)
	{
		case 1://RED
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
		break;
		case 2://GREEN
			analogWrite(46,255);analogWrite(2,200);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
			analogWrite(9,255);analogWrite(8,200);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,200);analogWrite(10,255);
		break;
		case 3://BLUE
			analogWrite(46,200);analogWrite(2,255);analogWrite(5,255);//B
			analogWrite(9,200);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,200);analogWrite(11,255);analogWrite(10,255);
		break;
		case 4://RED FLASH
			delay(200);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//R
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);

			delay(200);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//R
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);

			delay(200);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//R
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
			delay(200);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//R
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
			delay(200);
		break;
		case 5://led1 white
			 analogWrite(46,0);analogWrite(2,0);analogWrite(5,0);//LED1 WHITE
			digitalWrite(46,0);digitalWrite(2,0);digitalWrite(5,0);//LED1 WHITE
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		break;
		case 6://led2 white
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);
			analogWrite(9,200);analogWrite(8,200);analogWrite(7,200);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		break;
		case 7://GREEN FLASH
		   for(int i=0;i<1;i++)
		   {
			delay(400);
			analogWrite(46,255);analogWrite(2,200);analogWrite(5,255);//GREEN
			analogWrite(9,255);analogWrite(8,200);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,200);analogWrite(10,255);
			delay(400);
			analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//GREEN
			analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
			analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		   }

		break;
		default:
		break;
		
	}

}
void Timer1_handler1(void)
{
 hermes->status_show(7);
}
void Hermes::init()
{
	////////FAN control//////////////////
	// pinMode(41,OUTPUT);
	// digitalWrite(41,LOW);
	// delay(20);
	///////////////////////
	Serial.begin(9600);//open debug port
	esp->wireless_reset();
	status_show(5);//first power up  led1 is white
	esp->verify_ssid_password();
	DDRJ=1<<PJ7;
	esp->wifi_init();
	this->pms_init();
	Serial2.begin(9600);//PMS    
	this->temp_init();
	this->light_init();
	co->init();//ccs811
	this->pms_reset();
	this->status_show(7);//red flash;
	#ifdef __DEBUG__
	Serial.println(">>> hermes initalize successful!");  
	#endif
 }
 void Hermes::init_with_smartconfig()
{
////////////

/////////////	
	
	status_show(5);
	delay(1000);
	 DDRE |=(1<<PE6);
	 PORTE |=(1<<PE6);
   esp->select_function();
   esp->test_smart();
    // EEPROM.write(0,0xFF);
   // EEPROM.write(3,0);
     // EEPROM.write(450,0);
	 // EEPROM.write(480,0);
	// EEPROM.write(500,0);  // Wifi Number
	// EEPROM.write(501,0);  
	// EEPROM.write(502,0);  
	// EEPROM.write(503,0);
	// EEPROM.write(540,0);
   //EEPROM.write(120,0);
   // EEPROM.write(150,0x00);
    ////////FAN control//////////////////
	pinMode(41,OUTPUT);
	//digitalWrite(41,HIGH);
	digitalWrite(41,LOW);
	// delay(2000);
		delay(20);
	/////////////////////
	Serial.begin(9600);//open debug port
	  esp->wireless_reset();
	Serial3.begin(115200);
	esp->verify_ssid_password_smart_config();
	co_init();
     DDRJ=1<<PJ7;
	esp->wifi_init();
   this->pms_init();
  Serial2.begin(9600);//PMS   
	 this->temp_init();
	this->light_init();
	co->init();//ccs811	
	this->pms_reset();
	this->status_show(7);//red flash;
    Q=myQueue->SQTypeInit();	
	// co_test();
	#ifdef __DEBUG__
    Serial.println(">>> hermes initalize successful!");  
	#endif
	IP=esp->parse_self_ip("AT+CIFSR\r\n", "OK", 20000, 1);
	deviceIP=const_cast<char*>(IP.c_str());
    //strcpy(deviceIP,IP.c_str());  //在此處不能使用，否則LOG出現怪象--每條LOG前加IP
	esp->clear_buffer();
	MAC=esp->parse_self_MAC("AT+CIPSTAMAC?\r\n","OK",3000,1);
	BTaddr=const_cast<char*>(MAC.c_str());
	esp->clear_buffer();
	// if(!postDataToPDC(deviceNum,functionType,deviceIP,BTaddr,"null","null","null","null","null","null","null","null","null","null",fwversion,"null","null","null","null","null") )        		      
	// {
	 // esp->EEPROM_write_log('F');
	 // resetFunc(); 
	// } 
 } 
void Hermes::temp_init()
{
	TH03.begin(); 
	delay(100);//  Reset HP20x_dev
	Serial.println(">>> Si7020 is available.");
}
void Hermes::light_init()
{ 
	pinMode(6,OUTPUT);  
	pinMode(7,OUTPUT);
	digitalWrite(6,0);// 0x29
	digitalWrite(7,1);// 0x49
	TSL2561.init();  
	TSL2561.init(1);
	Serial.println(">>> TSL2561 is available."); 
}
void Hermes::pms_init()
{
	// pinMode(42,OUTPUT);  digitalWrite(42,0); 	//PMS init ,can't work;
	pinMode(43,OUTPUT);  digitalWrite(43,0); 
	pinMode(44,OUTPUT);  digitalWrite(44,0); 
	Serial2.begin(9600);  
	// pinMode(42,OUTPUT);  digitalWrite(42,1); 
	pinMode(43,OUTPUT);  digitalWrite(43,1); 
	pinMode(44,OUTPUT);  digitalWrite(44,1); 
	Serial.println(">>> PMS is available."); 
}
void Hermes::pms_reset()
{
  unsigned char hexdata[7] = {0x42, 0x4D, 0xE3, 0x00, 0x00, 0x01, 0x72};
  Serial2.write(hexdata, 8);                                                    
   delay(100);
}

bool Hermes::ChangeServerIPFun()
{
	String input;
	String sDeviceID;
	String sMac;
	String sSerIP;
	String sSerPort;
	String DeMac;
	input=esp->received_info();
	delay(50);
	if(input.length()!=0)
	{
		esp->parse_info(input,sDeviceID,sMac,sSerIP,sSerPort);
		DeMac=(String)BTaddr;
		Serial.print("device Mac:");
		Serial.println(DeMac);
		if((DeMac==sMac) && (sDeviceID.length()!=0))
		{
			if(postResultToPDC(const_cast<char*>(sDeviceID.c_str()),const_cast<char*>(sMac.c_str())))
			{
				Serial.println("Send OK");
				int_flag=0;
				EEPROM.write(120,int_flag);
				EEPROM.write(0,0xFF);
				EEPROM.write(3,0);
				resetFunc();
			}
		}
		if((DeMac==sMac) && (sSerIP.length()!=0))
		{
			int sip_len=sSerIP.length();
			delay(100);
			EEPROM.write(450,sip_len);
			int sport_len=sSerPort.length();
			delay(100);
			EEPROM.write(480,sport_len);
			delay(100);
			esp->EEPROM_write_block(sSerIP,455);
			delay(100);
			esp->EEPROM_write_block(sSerPort,490);
			delay(100);
			if(postChangeServerResultToPDC(const_cast<char*>(sMac.c_str())))
			{
				delay(100);
				resetFunc();
			}
			else
			{
				delay(100);
				EEPROM.write(450,0);
				EEPROM.write(480,0);
				resetFunc();
			}
		}
	}
	return true;
}

// bool Hermes::ClearPasswordFun()
// {
	// String input;
	// String sDeviceID;
	// String sMac;
	// String DevMac;
	// input=esp->received_info();
	// if(input.length()!=0)
	// {
	// esp->parse_clearpassword_info(input,sDeviceID,sMac);
	// DevMac=(String)BTaddr;
	// Serial.print("BTaddr:");
	// Serial.println(DevMac);
	// if(DevMac==sMac)
	// {
		 // if(postResultToPDC(const_cast<char*>(sDeviceID.c_str()),const_cast<char*>(sMac.c_str())))
		 // {
			 // Serial.println("Send OK");
			 // int_flag=0;
			 // EEPROM.write(120,int_flag);
			 // EEPROM.write(0,0xFF);
			 // EEPROM.write(3,0);
			 // resetFunc();
		 // }
	// }
	// }
	// return true;
// }

void Hermes::exec()
{
		  char cdust[30],cdust1[30],cnoise[30],csun[30],csun1[30],ctemp[30],chum[30],cCO2[30],ctvoc[30],cmotion[2],cmotion1[2],csmoke[2],cco[30];
		   float temp,hum,noise;
		   int sun,sun1;
		    double  co1;
		    long co2,tvoc;
			int motion=0;
			int motion1=0;
			int smoke=0;       
			int times=0;
			unsigned int errorlength;

	while(1)
	{
		esp->test_smart();
       while (Serial2.available()>0)
         {
			 // esp->test_smart();
			 errorlength=(unsigned char)EEPROM.read(150);
			 if((errorlength>0) && (errorlength<=255))
			 {
				postToServer();
			 }
              inputchar[i] = Serial2.read();
              i++;
              if ((inputchar[0] != 0x42) && (inputchar[1] != 0x4D))
              {
                i = 0;
              }
              if (i == 32)
              {
				Serial.println("");
                i = 0;
                int sum=0;
                for(int m=0;m<30;m++)
				{
					sum+=inputchar[m];
				}
				#ifdef __DEBUG__
                Serial.print(">>> sum=");
				Serial.println(sum);
				#endif
                int checksum=(inputchar[30] << 8) + inputchar[31];   
                if (checksum ==sum)
                { 
                  count++;
				  delay(500);
				   pm25 = ((long(inputchar[12]) << 8) + inputchar[13]);				
				   dust = (long(inputchar[18]) << 8) + inputchar[19] ;//read 0.5um 
					#ifdef __DEBUG__
					Serial.print(">>> dust_0.5um per 0.1 L : "); 
					Serial.println(dust);
					#endif

					myQueue->InSQType(Q,dust);
					 
					 int iQLen=(myQueue->SQLTypeLen(Q));
					 
					 
					 if(iQLen==28)
					 {
						for(int j=0;j<28;j++)
						{
						 fenchen_sum+=(Q->Data[j]);
						 }
					 }
					
					fenchen_sum=fenchen_sum*10;		
					/////////////////////
					 #ifdef __DEBUG__
				Serial.print(">>> dust  per 2.83L: "); 
				Serial.println(fenchen_sum);
				#endif
                pm25 = ((long(inputchar[12]) << 8) + inputchar[13]);				
                #ifdef __DEBUG__
				Serial.print(">>> PM25 : "); 
				Serial.println(pm25);
				#endif
				//修正后溫度=0.000163033*[Hermes溫度]^2 +1.01402*[Hermes溫度] - 2.55026  --NO fun
				//修正后濕度=1.1438*[21秒後的Hermes濕度]-1.5616   ---NO fun
				temp=0.000163033*(TH03.ReadTemperature())*(TH03.ReadTemperature())+1.01402*(TH03.ReadTemperature())-z3-3.06003-1.85;
			    hum=0.121680074383*(1.1438*TH03.ReadHumidity()+18.62653)+25.1369117401;
				//hum=1.1438*TH03.ReadHumidity()-h2+12.95+7.23813;		
				//temp=TH03.ReadTemperature()-0.7;
				//補償后濕度=Hermes讀取濕度*(1+溫度Offset*5%)
				 //hum=TH03.ReadHumidity()*(1+0.7*0.05);
				 // temp=(y3*TH03.ReadTemperature()*1.00929-2.39105)-4.45;
				 // hum=y4*TH03.ReadHumidity()+10;
				 #ifdef __DEBUG__	
                Serial.print(">> temp : "); Serial.println(temp);
				Serial.print(">> hum : "); Serial.println(hum);
				#endif
                ///////////////////noise/////////////////////////
                noise=y1*hermes->noise_test();
			    #ifdef __DEBUG__	
                Serial.print(">>> noise : "); 
				Serial.println(noise);
				#endif
                ///////////////////light/////////////////////////
                sun=y2*TSL2561.readVisibleLux(0);
				#ifdef __DEBUG__	
                Serial.print(">>> sun : "); 
				Serial.println(sun);
				 #endif
                sun1=y2*TSL2561.readVisibleLux(1);  
                 #ifdef __DEBUG__				
                Serial.print(">>> sun1 : ");
				Serial.println(sun1);
				  #endif
                  ///////////////////CO2///////////////////////////  
				  
				    co->read(co2,tvoc);
				   #ifdef __DEBUG__
                  Serial.print(">>> co2: "); Serial.println(co2);
                  Serial.print(">>> tvoc: "); Serial.println(tvoc);
				  #endif
				  double res;
				  co->Failure_Check(res);
                  ///////////////////Motion///////////////////////////
          
				  get_motion_value(motion,motion1);
				  #ifdef __DEBUG__
                  Serial.print(">>> motion1: "); Serial.println(motion);
                  Serial.print(">>> motion2: "); Serial.println(motion1);
				  #endif
				   ///////////////////smoke///////////////////////////
                 #ifdef __VP__
				           
				  get_smoke_value(smoke);	
				  #ifdef __DEBUG__
                  Serial.print(">>> smoke: "); Serial.println(smoke);                  
				  #endif
				  #endif
				  #ifdef __VP__
				   ///////////////////CO///////////////////////////
                   co1=y7*hermes->co_test();               
				  #ifdef __DEBUG__
                  Serial.print(">>> co: "); 
				  Serial.println(co1);                  
				  #endif
				  #endif
                  ///////////////////Format///////////////////////////   
                 Serial.print("last_dust: ");
                  Serial.println(fenchen_sum);		  
				  dtostrf(fenchen_sum,1,0,cdust);
				  dtostrf(pm25,1,0,cdust1);dtostrf(noise,1,0,cnoise);dtostrf(sun,1,0,csun);dtostrf(sun1,1,0,csun1);
                  dtostrf(temp,1,2,ctemp);dtostrf(hum,1,2,chum);dtostrf(co2,1,0,cCO2);dtostrf(tvoc,1,0,ctvoc);
                  dtostrf(motion,1,0,cmotion);dtostrf(motion1,1,0,cmotion1);
				  #ifdef __VP__
				  dtostrf(smoke,1,0,csmoke);dtostrf(co1,1,2,cco);	
				  #endif
				  
				  if(count_green++==2)
					{
						  hermes->status_show(7);	
						  count_green=0;
					}	
				//////2017-08-22///////
				int val=analogRead(3);
				Serial.print("bat detech analog:");
				Serial.println(val);
				double vol=val/1023.0*5;
				Serial.println(vol);
				 
				if(vol<=3.2)
				{
					for(int i=0;i<60;i++)
					{
						delay(20);
							analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
							analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
							analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
						delay(20);
							analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//GREEN
							analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
							analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
					}
					 unsigned long st=millis();
				     while(millis()-st<10000);
					  DDRE |=(1<<PE6);
	                 PORTE |=(0<<PE6);
					 Energy energy;
					 energy.PowerDown();
				}
				esp->test_smart();
				 ChangeServerIPFun();
				  if(fenchen_sum!=0)
				  {
					  #ifdef __VP__
					  {
						  if(!postDataToPDC(deviceNum,functionType,deviceIP,BTaddr,cdust,cnoise,csun,csun1,ctemp,chum,cCO2,ctvoc,cmotion1,cmotion,fwversion,cdust1,cco,csmoke,"null","null") )        		      
						  {
							 esp->EEPROM_write_log('F');
							 resetFunc(); 
						  } 
					  }
					  #else
					  {
						  if(!postDataToPDC(deviceNum,functionType,deviceIP,BTaddr,cdust,cnoise,csun,csun1,ctemp,chum,cCO2,ctvoc,cmotion1,cmotion,fwversion,cdust1,"null","null","null","null"))              		      
						  {
							  esp->EEPROM_write_log('F');
							  resetFunc(); 
						  }   
					  }
					  #endif
					  fenchen_sum=0;
				  }
				  else
				  {
					  #ifdef __VP__
					  {
						  if(!postDataToPDC(deviceNum,functionType,deviceIP,BTaddr,"null",cnoise,csun,csun1,ctemp,chum,cCO2,ctvoc,cmotion1,cmotion,fwversion,"null",cco,csmoke,"null","null") )        		      
						  {
							 esp->EEPROM_write_log('F');
							 resetFunc(); 
						  } 
					  }
					  #else
					  {
						  if(!postDataToPDC(deviceNum,functionType,deviceIP,BTaddr,"null",cnoise,csun,csun1,ctemp,chum,cCO2,ctvoc,cmotion1,cmotion,fwversion,"null","null","null","null","null"))              		      
						  {
							  esp->EEPROM_write_log('F');
							  resetFunc(); 
						  }   
					  }
					  #endif
				  }
		
                }
              }
						
          }  
		  this->pms_reset();	
     }
}

void Hermes::get_smoke_value(int& smoke)
 {
	analogRead(A12)>512?smoke=1:smoke=0;
 }

 void Hermes::get_motion_value(int& motion1,int& motion2)
 {
	analogRead(A1)>500?motion1=1:motion1=0;
	analogRead(A2)>500?motion2=1:motion2=0;
 }
 
 bool Hermes::postToServer()
 {
	 char* logcmd="";
	 unsigned int errorloglength;
	 bool bPost;
	 String errorlogvalue="";
	 errorloglength=(unsigned char)EEPROM.read(150);
	 if(errorloglength<=255)
	 {
		 Serial.print("length:");
		 Serial.println(errorloglength,HEX);
		 delay(100);
		 errorlogvalue=esp->EEPROM_read_log(151,errorloglength);
		 delay(100);
		 Serial.print("error log:");
		 Serial.println(errorlogvalue);
		 logcmd=const_cast<char*>(errorlogvalue.c_str()); 
		 if(postLogToPDC(deviceNum,logcmd))
		 {
			 delay(100);
			 esp->EEPROM_clear_log(151,errorloglength);	
		 }
			 	 
	 }
	 EEPROM.write(150,0x00);
	 return true;
 }
 
 bool Hermes::postChangeServerResultToPDC(char* Mac)
 {
	 char cmd[400];
	 memset(cmd,0,400);
	 char http_cmd[600];
	 memset(http_cmd,0,600);
	 strcpy(http_cmd,"ChangeServer#");
	 strcat(http_cmd, Mac);strcat(http_cmd, ",");
	 strcat(http_cmd, "OK");strcat(http_cmd, "*");
	 memset(cmd,0,400);
	 sprintf(cmd, "AT+CIPSEND=%d\r\n",strlen(http_cmd));
	 if(!esp->sendCommand(cmd, ">", 1000, 1))	
     {
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
		esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
    }		
	if(!esp->sendCommand(http_cmd, "SEND OK", 3000, 1))	
	{
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
	    esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
	}	 

	return true;
 }
 
 bool Hermes::postResultToPDC(char* deviceNum,char* Mac)
 {
	 char cmd[400];
	 memset(cmd,0,400);
	 char http_cmd[600];
	 memset(http_cmd,0,600);
	 strcpy(http_cmd,"CLEARPW#");
	 strcat(http_cmd, deviceNum);strcat(http_cmd, ",");
	 strcat(http_cmd, Mac);strcat(http_cmd, ",");
	 strcat(http_cmd, "OK");strcat(http_cmd, "*");
	 memset(cmd,0,400);
	 sprintf(cmd, "AT+CIPSEND=%d\r\n",strlen(http_cmd));
	 if(!esp->sendCommand(cmd, ">", 1000, 1))	
     {
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
		esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
    }		
	if(!esp->sendCommand(http_cmd, "SEND OK", 3000, 1))	
	{
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
	    esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
	}	 

	return true;
 }
 
 bool Hermes::postLogToPDC(char* deviceNum,char* logValue)
 {
	 Serial.print("logvalue:");
	 Serial.println(logValue);
	 char cmd[400];
	 memset(cmd,0,400);
	 char http_cmd[600];
	 memset(http_cmd,0,600);
	 strcpy(http_cmd, "ERRORCODE$");
	 strcat(http_cmd, deviceNum);strcat(http_cmd, ","); 
	 strcat(http_cmd,logValue);strcat(http_cmd,"@");
	 memset(cmd, 0, 400);  
     sprintf(cmd, "AT+CIPSEND=%d\r\n",strlen(http_cmd));
     if(!esp->sendCommand(cmd, ">", 1000, 1))	
     {
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
		// analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//wifi connection fail ,red led 
		// analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
		// analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
		esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
    }		
 if(!esp->sendCommand(http_cmd, "SEND OK", 3000, 1))	
  {
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
		// analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//wifi connection fail ,red led 
		// analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
		// analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
	    esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
  }	 

  return true;
 }

bool Hermes::postDataToPDC(char* deviceNum,char* functionType,char* deviceIP,char* BTaddr,char* dust,char* noise,char* sun1,char* sun2,char* temp,
char* hum,char* CO2,char* TVOC,char* O2,char* motion1,char* motion2,char* resv1,char* resv2,char* resv3,char* resv4,char* resv5) {
  //////////////////////make connection////////////////////////////////
  char cmd[400];
  memset(cmd, 0, 400);  
  char http_cmd[600];
  memset(http_cmd, 0, 600);  
  strcpy(http_cmd, "PM25#");
  strcat(http_cmd, deviceNum);strcat(http_cmd, ",");                    //1
  strcat(http_cmd, functionType);strcat(http_cmd, ",");// new           //2
  strcat(http_cmd, deviceIP);strcat(http_cmd, ",");// new               //3
  strcat(http_cmd, BTaddr);strcat(http_cmd, ",");// new                 //4
  strcat(http_cmd, dust);strcat(http_cmd, ",");                         //5
  strcat(http_cmd, noise);strcat(http_cmd, ",");                        //6
  strcat(http_cmd, sun1);strcat(http_cmd, ",");                         //7
  strcat(http_cmd, sun2);strcat(http_cmd, ",");//sun2                   //8
  strcat(http_cmd, temp);strcat(http_cmd, ",");                         //9
  strcat(http_cmd, hum);strcat(http_cmd, ",");                          //10
  strcat(http_cmd, CO2);strcat(http_cmd, ",");                          //11
  strcat(http_cmd, TVOC);strcat(http_cmd, ",");                         //12
  strcat(http_cmd, O2);strcat(http_cmd, ","); //o2                      //13
  strcat(http_cmd, motion1);strcat(http_cmd, ",");                      //14
  strcat(http_cmd, motion2);strcat(http_cmd, ","); //motion2            //15
  strcat(http_cmd, resv1);strcat(http_cmd, ","); //resv            //16
  strcat(http_cmd, resv2);strcat(http_cmd, ","); //resv            //17
  strcat(http_cmd, resv3);strcat(http_cmd, ","); //resv            //18
  strcat(http_cmd, resv4);strcat(http_cmd, ","); //resv            //19
  strcat(http_cmd, resv5);strcat(http_cmd, "*"); //resv            //20
  ///////////////////////////////////////CMD////////////////////
  memset(cmd, 0, 400);  
  sprintf(cmd, "AT+CIPSEND=%d\r\n",strlen(http_cmd));
    if(!esp->sendCommand(cmd, ">", 1000, 1))	
    {
		esp->EEPROM_write_log('F');
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
		// analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//wifi connection fail ,red led 
		// analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
		// analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
		delay(100);
		 esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
    }		
 if(!esp->sendCommand(http_cmd, "SEND OK", 3000, 1))	
  {
	  esp->EEPROM_write_log('F');
		#ifdef __TIME_COUNT__
		unsigned long start=millis();
		#endif
		// analogWrite(46,255);analogWrite(2,255);analogWrite(5,200);//wifi connection fail ,red led 
		// analogWrite(9,255);analogWrite(8,255);analogWrite(7,200);
		// analogWrite(12,255);analogWrite(11,255);analogWrite(10,200);
	     esp->wifi_reconnect();
		analogWrite(46,255);analogWrite(2,255);analogWrite(5,255);//3,9,12 is blue,5,7,10 is red,2,8,11 is green;
		analogWrite(9,255);analogWrite(8,255);analogWrite(7,255);
		analogWrite(12,255);analogWrite(11,255);analogWrite(10,255);
		delay(10);
		#ifdef __TIME_COUNT__
		Serial.print("######recover time:");
		unsigned long stamp=(millis()-start)/1000;
		Serial.println(stamp);
		#endif
  }	 

  return true;
}
double Hermes::noise_test()
{                    

  int a[1000];
  double b = 0;
  double maxNoise1 = 0;

  //double noise = 0;

  for (int m = 0; m < 5; m++) {
    for (int i = 0; i <= 1000; i++) {
      a[i] = analogRead(A0);
      maxNoise1 = max(maxNoise1, a[i]);
    }
    b += maxNoise1;
  }

  maxNoise1 = b / 5;

  maxNoise1 = maxNoise1 * 100 / 1024;
  //Serial.print("ana=");Serial.println(maxNoise1);
  if (maxNoise1 <= 3.5)
  {
    return maxNoise1 + 37;
  }
  else if ((maxNoise1 <= 3.75) && (maxNoise1 > 3.5))
  {
    return maxNoise1 + 46;
  }
  else if ((maxNoise1 <= 4.25) && (maxNoise1 > 3.75))
  {
    return maxNoise1 + 50;
  }
  else if ((maxNoise1 <= 5) && (maxNoise1 > 4.25))
  {
    return maxNoise1 + 54;
  }
  else if ((maxNoise1 <= 5.5) && (maxNoise1 > 5))
  {
    return maxNoise1 + 59;
  }
  else if ((maxNoise1 <= 6) && (maxNoise1 > 5.5))
  {
    return maxNoise1 + 50;
  }
  else if ((maxNoise1 <= 6.6) && (maxNoise1 > 6))
  {
    return maxNoise1 + 48;
  }
  else if ((maxNoise1 <= 7.9) && (maxNoise1 > 6.6))
  {
    return maxNoise1 + 44.5;
  }
  else if ((maxNoise1 <= 9) && (maxNoise1 > 7.9))
  {
    return maxNoise1 + 53;
  }
  else if ((maxNoise1 <= 12) && (maxNoise1 > 9))
  {
    return maxNoise1 + 60;
  }
  else if ((maxNoise1 <= 18) && (maxNoise1 > 12))
  {
    return maxNoise1 + 60;
  }
  else if ((maxNoise1 <= 25) && (maxNoise1 > 18))
  {
    return maxNoise1 + 50;
  }
  else if ((maxNoise1 <= 30) && (maxNoise1 > 25))
  {
    return maxNoise1 + 55;
  }
  else if ((maxNoise1 <= 35) && (maxNoise1 > 30))
  {
    return maxNoise1 + 53;
  }
  else if ((maxNoise1 <= 40) && (maxNoise1 > 35))
  {
    return maxNoise1 + 59;
  }
  else if ((maxNoise1 <= 45) && (maxNoise1 > 40))
  {
    return maxNoise1 + 57;
  }
  else if ((maxNoise1 <= 50) && (maxNoise1 > 45))
  {
    return maxNoise1 + 49;
  }
  else if ((maxNoise1 <= 55) && (maxNoise1 > 50))
  {
    return maxNoise1 + 50;
  }
  else if ((maxNoise1 <= 65) && (maxNoise1 > 55))
  {
    return maxNoise1 + 40;
  }
  else if ((maxNoise1 <= 80) && (maxNoise1 > 65))
  {
    return maxNoise1 + 30;
  }
  else
  {
    return 110;
  }
}
void Hermes::co_init()
{
		Serial.begin(9600);
	Serial1.begin(9600);
		DDRL|=(1<<PL7);
		PORTL|=(0<<PL7);
}
double Hermes::co_test()
{
	Serial.begin(9600);
	Serial1.begin(9600);
	DDRL|=(1<<PL7);
	PORTL|=(0<<PL7);
	unsigned char buffer[9]={0};	
	int i=0;
	while(Serial1.available())
	{
		buffer[i]=Serial1.read();
		i++;
		delay(100);
		if((buffer[0]!=0xFF)&&(buffer[1]!=0x04)&&(buffer[2]!=0x03))
		{
			i=0;
		}
		if(i>8)
		{
			// for(int m=0;m<9;m++)
			// {
					// Serial.print("co-->");
		            // Serial.println(buffer[m],HEX);
			// }
				i=0;
				// Serial.print("buffer[8]:");
				// Serial.println(buffer[8],HEX);
				// Serial.print("checksum:");
				// unsigned char sum=FucCheckSum(buffer,9);
				// Serial.println(sum,HEX);
				if(FucCheckSum(buffer,9)==buffer[8])
				break;
		}		
	}
		return (buffer[4]*256+buffer[5])*0.1;
}
unsigned char Hermes::FucCheckSum(unsigned char *i,unsigned char ln)
{
	unsigned char j,tempq=0;
	i+=1;
	for(j=0;j<(ln-2);j++)
	{
		tempq+=*i;
		i++;
	}
	tempq=(~tempq)+1;
	return(tempq);
}