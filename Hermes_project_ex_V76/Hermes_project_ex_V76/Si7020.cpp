/*
 * TH02_dev.cpp
 * Driver for DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR
 *  
 * Copyright (c) 2014 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Oliver Wang
 * Create Time: April 2014
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "Si7020.h"
#include <Wire.h>
#include <Arduino.h>
/* Use Serial IIC */
#ifdef SERIAL_IIC
#endif

TH03_dev TH03;
/****************************************************************************/
/***       Local Variable                                                 ***/
/****************************************************************************/


/****************************************************************************/
/***       Class member Functions                                         ***/
/****************************************************************************/

void TH03_dev::begin(void)
{
    /* Start IIC */
    Wire.begin();
	/* TH02 don't need to software reset */
}

float TH03_dev::ReadTemperature(void)
{    
    /* Start a new temperature conversion */
	TH02_IIC_WriteCmd(CMD_MEASURE_TEMP);	 	 
    //delay(100);
	/* Wait until conversion is done */
	//while(!isAvailable());
	float value = TH02_IIC_ReadTempCmd();
	return value;
}
 
float TH03_dev::ReadHumidity(void)
{
 /* Start a new humility conversion */
		TH02_IIC_WriteCmd(CMD_MEASURE_HUMI);	 	 
    //delay(100);
	/* Wait until conversion is done */
	//while(!isAvailable());
	float value = TH02_IIC_ReadHumdiCmd();
	return value;	
}

/****************************************************************************/
/***       Local Functions                                                ***/
/****************************************************************************/
/*uint8_t TH02_dev::isAvailable()
{
    uint8_t status =  TH02_IIC_ReadReg(REG_STATUS);
	if(status & STATUS_RDY_MASK)
	{
	    return 0;    //ready
	}
	else
	{
	    return 1;    //not ready yet
	}
}
*/
void TH03_dev::TH02_IIC_WriteCmd(uint8_t u8Cmd)
{		
	/* Port to arduino */
	Wire.beginTransmission(TH02_I2C_DEV_ID);
	Wire.write(u8Cmd);
	Wire.endTransmission();
}


double TH03_dev::TH02_IIC_ReadTempCmd()
{		
	/* Port to arduino */
	//Wire.beginTransmission(TH02_I2C_DEV_ID);
	//Wire.write(u8Cmd);
	//Wire.endTransmission();
	Wire.requestFrom(TH02_I2C_DEV_ID,2); 
	unsigned int X0,X1;
	double X,X_out;
	if(Wire.available()<=2);
	{
		 X0 = Wire.read();//Serial.print("X0:");Serial.print(X0);
		 X1 = Wire.read();//Serial.print("X1");Serial.print(X1);
		 X0 = X0<<8;
		 X_out = X0+X1;
	}
	 X=(175.72*X_out)/65536;                        
   	 X=X-46.85;//Serial.print("X");Serial.print(X);
	return X;	
}
double TH03_dev::TH02_IIC_ReadHumdiCmd()
{		
	/* Port to arduino */
	//Wire.beginTransmission(TH02_I2C_DEV_ID);
	//Wire.write(u8Cmd);
	//Wire.endTransmission();
	Wire.requestFrom(TH02_I2C_DEV_ID,2); 
	int Y0,Y1,Y2,Y3;
	double Y,Y_out1,Y_out2;
	 if(Wire.available()<=2);
	{
		 Y0 = Wire.read();Y2=Y0/100; Y0=Y0%100;
		 Y1 = Wire.read();
		 Y_out1 = Y2*25600;
		 Y_out2 = Y0*256+Y1;
	}

   /**计算并显示湿度**/
	   Y_out1 = (125*Y_out1)/65536;                     
	   Y_out2 = (125*Y_out2)/65536;
	   Y = Y_out1+Y_out2;
	   Y=Y-6;
	   return Y;
}


