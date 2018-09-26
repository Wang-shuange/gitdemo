#ifndef _CO2_H
#define _CO2_H
#include <Wire.h> 
#include "Arduino.h"
class Co2{
  private:
   int address;
	void set_address(int addr);
	int get_address();
	public:
	void init();
	void read(long& co2,long& tvoc);
	 void Failure_Check(double& resistance);
};
 static Co2 *co; 
#endif
