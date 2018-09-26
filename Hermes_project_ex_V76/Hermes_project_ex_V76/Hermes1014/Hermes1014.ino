/*--------------------------------------------------------*/
/* project:hermes_smart_sensor
 * author:wangshuange
 * version:02
/*--------------------------------------------------------*/
#include <Wire.h>
#include <TimerOne.h>
#include "hermes.h"
#define __SMART_CONFIG__
//#define __BT__
void setup()
{
  Serial.begin(9600);
#ifdef __SMART_CONFIG__
  hermes->init_with_smartconfig();
#endif
#ifdef __BT__
  hermes->init();
#endif
}
void loop()
{
//#ifdef __SMART_CONFIG__
//  esp->test_smart();
//#endif
  hermes->exec();
}


