
#include "co2.h"

void Co2::init()
{
  this->set_address(0x5A);
   Wire.begin();                                   
   delay(100);  
   Wire.beginTransmission(0x5A);
   Wire.write(0xF3); 
   Wire.endTransmission();
   delay(1000);
   Wire.beginTransmission(0x5A);
   Wire.write(0xF4);  
   Wire.endTransmission();
   delay(1000);
    Wire.beginTransmission(0x5A);                    
   Wire.write(0x01);  
   Wire.write(0x10);
   Wire.endTransmission();
   Serial.println(">>> CCS811 is available.\n");   
}
 // void Co2::read(long& co2,long& tvoc){
   // while(1)
   // {
	 //  Serial.println(">>> co2 sensor reading...");
       // Wire.beginTransmission(0x5A);
       // Wire.write(0x00);  
       // Wire.endTransmission();                         
       // Wire.requestFrom(0x5A,1); 
       // int X0 = Wire.read();	   
		// if(X0 == 0x98)
		// {
		//	Serial.println(">>> received ox98...");
			// pinMode(13,OUTPUT);    
			// Wire.beginTransmission(0x5A);                    
			// Wire.write(0x02);                               
			// Wire.endTransmission();
			// digitalWrite(13,HIGH);                         
			// Wire.requestFrom(0x5A,4);                    
			// int X0 = Wire.read();
			// int X1 = Wire.read();
			// co2=(X0<<8)+X1;
			//float Z=X*1.295;
			// Serial.print("CO2:  ");Serial.print(co2);Serial.println(" PPM");
			// int X2 = Wire.read();
			// int X3 = Wire.read();
			// tvoc=(X2<<8)+X3;
			// float Z1=*1.295/1000;
			// Serial.print("TVOC:  ");Serial.print(tvoc);Serial.println(" PPB");
			// delay(500);
		// return;
		// }
   // }
// }
 void Co2::read(long& co2,long& tvoc)
 {
   
   unsigned long start=millis();
   while(millis()-start<1000)
   {
       Wire.beginTransmission(0x5A);
       Wire.write(0x00);  
       Wire.endTransmission();                         
       Wire.requestFrom(0x5A,1); 
       int X0 = Wire.read();	   
		if(X0 == 0x98)
		{
			pinMode(13,OUTPUT);    
			Wire.beginTransmission(0x5A);                    
			Wire.write(0x02);                               
			Wire.endTransmission();
			digitalWrite(13,HIGH);                         
			Wire.requestFrom(0x5A,4);                    
			int X0 = Wire.read();
			int X1 = Wire.read();
			co2=(X0<<8)+X1;
			int X2 = Wire.read();
			int X3 = Wire.read();
			tvoc=(X2<<8)+X3;
			delay(500);
		return;
		}
   }
   co2=0;
   tvoc=0;
   return ;
}
 void Co2::Failure_Check(double& resistance)
 {
   
   unsigned long start=millis();
   while(millis()-start<3000)
   {
       Wire.beginTransmission(0x5A);
       Wire.write(0x00);  
       Wire.endTransmission();                         
       Wire.requestFrom(0x5A,1); 
       int X0 = Wire.read();	   
		if(X0 == 0x98)
		{
			pinMode(13,OUTPUT);    
			Wire.beginTransmission(0x5A);                    
			Wire.write(0x03);                               
			Wire.endTransmission();
			digitalWrite(13,HIGH);                         
			Wire.requestFrom(0x5A,2);                    
			int X0 = Wire.read();
			int X1 = Wire.read();
			double current=X0>>2;
			Serial.print("--->current:");
			Serial.print(current);
			Serial.println(" uA");
			double  raw_adc=X0<<8+X1;
		Serial.print("--->raw_adc:");
		Serial.println(raw_adc);
			 resistance=(double)(1.65*(double)raw_adc*1000000)/((double)current*1023);
			 Serial.print("--->Ressistance:");
			 Serial.print(resistance);
			 Serial.println(" Ohm");
			delay(500);
		return;
		}
   }
 resistance=0.00;
   // tvoc=0;
   return ;
}

void Co2::set_address(int addr)
{
	this->address=addr;
}

int Co2::get_address()
{
	return this->address;
}

