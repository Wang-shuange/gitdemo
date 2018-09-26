#ifndef _HERMES_H
#define _HERMES_H
#include <Wire.h> 
#include "Arduino.h"
#include "co2.h"
#include "esp8266.h"
#include "Si7020.h"
#include "Enerlib.h"
#include "Queue.h"
#include "Digital_Light_TSL2561.h"  ///Light Sensor IIC
#define EEPROM_START_ADDRESS    100     // Start Address in EEPROM EEPROM_storage
#define __TIME_COUNT__
   #define __DEBUG__
   #define __VP__
	 static char* functionType= "A"; 
	 static String IP;
	 static String MAC;
	 static char* deviceIP= ""; 
	 static char* BTaddr = ""; 
	 static unsigned char inputchar[32] = {0}; // Dust Serial buffer
	 static long dust,cc;
	 static long pm25;
	 static char* deviceNum= "DVT6-151"    ;    //Device ID PL-1F-A-01
	 static char* fwversion= "V76";  
	 static Queue *myQueue;
	 static SQType *Q;
     /*----------------------------------------------
	 2018-05-29 新增功能V76：新增功能1：設備location本地存儲
	 新增功能2：本地存儲已連接過的Wifi信息，實現環境變更時自動連接
	 問題點：功能1與功能2衝突（定位是在SmartConfig過程中獲取，實現自動連接無需再SmartConfig）
	 500位存儲是否存儲過Wifi的標誌位（flag=0未存儲過Wifi，flag=1有存儲Wifi）
	 占用字符長度（共50位）：
	 SSID長度：2位
	 SSID：16位
	 PSD長度：2位
	 PSD：16位
	 Location長度：2位
	 Location：12位
	 /*----------------------------------------------
	 2018-03-21修改內容：取消1分鐘后開始上傳數據邏輯，改為僅粉塵1分鐘后開始上傳數據，
	 其他Sensor值連接上Server后即上傳
     增加Flag判斷位置
	 2018-03-15新增功能V73：遠程修改Server IP及端口號功能
	 WEB中控發送Server IP及端口號，Hermes收到信息后保存至EEPROM
	 接收：ChangeServer#MAC,ServerIP,COM*
	 回傳：ChangeServer#MAC,OK*
	 收到信息后存入EEPROM，450位存儲ServerIP長度，455開始存IP
	 480位存儲端口號長度，490后存端口號
	 /*------------------------------------------------
	 2018-02-27新增功能V72：遠程清空密碼功能
	 手機APP端發送清空密碼指令，Hermes設備接到指令清空密碼
	 接收：HermesCommand#ID,MAC*
	 回傳：CLEARPW#ID,MAC,OK*
	 測試服務器：10.199.28.206：8443
	 2018-02-02修改內容：
	 1.粉塵顆粒數X10；
	 2.新增恢復出廠設備按鍵，長按Reset 7S以上，即長按7S閃藍燈；
	 3.修復溫度40度以上出現負值的情況（變量定義問題，由int變為unsigned int）
	 4.除SmartConfig長亮紅燈外，其他異常均為每16S紅燈閃爍一次
	 /*--------------------------------------------
	 2017-12-22修改內容V71：小功率無風扇帶煙感添加功能：恢復出廠設置功能
	 /*---------------------------------------------
	 2017-12-02修改內容V66：小功率無風扇帶煙感增加異常LOG存儲功能
	 異常LOG存儲從EEPROM中150位開始，最長存255，至405結束
	 SmartConfig邏輯修改為長按Reset未接收到新Wifi信息前不清空原Wifi信息
	 /*-------------------------------------------
	 2017-11-24變更內容V63：小功率無風扇帶煙感增加讀取IP&Mac地址功能
	 /*-------------------------------------------
	 2017-11-11變更內容V58：Wifi重連機制變更
	 ---------------------------------------------*/
	 //2017-10-14 V56變更內容：溫濕度算法變更，濕度延時21S輸出
	 /*-------------------------------------------
	 小功率無風扇帶煙感：
	 fix溫度 = 0.000163033*[Hermes溫度]^2 +1.01402*[Hermes溫度] - 3.06003-1.85				
     fix濕度 = 1.1438*[21秒後的Hermes濕度]+12.95+7.23813					 
	 ---------------------------------------------*/
	//2017-10-13  变更内容：增加CO2阻抗检测算法，变更文件CO2.H,CO2.CPP;
	//2017-09-26 變更內容：
	//V52：修改粉塵數據為第一次有效數據為一分鐘，之後每次數據每3S傳一次，實現實時動態更新（28個有效值為一個隊列，當隊列滿時每更新一個有效值都需要刪除隊頭一個值）
	 //2017-09-21 变更内容：
	 //修改pm25浓度为1分钟显示一次；
	 //a.为实现sensor 动态值，粉尘一分钟传一次有效值，在此期间，3秒传一次值，上传所有数据，与之前通信协议兼容，粉尘值使用上一次值；
	 //b.粉尘取消X10;
	 //2017-09-20 小风扇使用累加算法
	 //2017-09-14 添加一个程序开关用来切换VP的不同需求版本，将co 及烟感发送到服务器 
	 //2017-09-12 增加CO &烟感，调试OK;
	 
	 //修改PM2.5 粉尘颗粒（0.5um）数系数 ，按照王清要求>10000 不变，小于10000值除2
	 //2017-08-23 change content：
	 //1.依照PL副理指示，修改smart config 配置状态指示（变更前红灯闪烁3次后，变黑，变更后红灯闪烁3次后配置过程红灯常亮，配置成功led关掉。
	 //2.fix bug：smartconfig配置失败超时后无反应，变更后，配置失败两次超时后自动重启单片机；
	 //3.修改LED1 蓝灯port，由digital3 变为digital46，消除LED1蓝灯与smarconfig 配置中断pin之冲突；
	 //2017-08-25 change content：
	 //1.修改smartconfig，将超时设置取消，一直等待用户输入，直到设置成功；
	 
    static double s1,s2,t1,t2;
	 static int   i = 0; //32  Dust counter
	 static int   count = 0; //10 Dust counter
	 static int   num = 0;   //28 Dust counter
	 static float y = 1;   ///0.58; //Dust 1.76
	 static float y1 = 1;  //nosie  
	 static float y2 = 1;  //sun �
	 static float y3=1;     //Temp � 25.8/19.34
	 static float y4=1; //Humi�   63/53.25
	 static int   y5=1; //CO2� 
	 static int   y6=1; //TVOC
	 static int   y7=1; //CO
	 static double D0=0.00;
	 static double D1=0.00;
	 static double DS=0.00;
	 static float z1=0.000163033;
	 static float z2=1.01402;
 	 static float z3=2.55026;
	 static float h1=1.1438;
	 static float h2=1.5616;
	 static char* PDCServer= "mlbserver.foxconn.com"; 
	 static int count_green=0;
	  static  long  fenchen_sum=0;
	  static long last_fenchen_sum=0;
	  static long last_pm25=0;
	  static int queueflag=0;
class Hermes{
private:

public:
 void init();
  void init_with_smartconfig();
 void exec();
 void status_show(int color);
 void pms_reset();
 void pms_init();
 void light_init();
 void temp_init();
 bool postDataToPDC(char* deviceNum,char* functionType,char* deviceIP,char* BTaddr,char* dust,char* noise,char* sun1,char* sun2,char* temp,char* hum,char* CO2,char* TVOC,char* O2,char* motion1,char* motion2,char* resv1,char* resv2,char* resv3,char* resv4,char* resv5);
double noise_test();
void co_init();
double co_test();
unsigned char FucCheckSum(unsigned char *i,unsigned char ln);
 void get_motion_value(int& motion1,int& motion2);
 void get_smoke_value(int& smoke);
void wifi_control();

//2017-11-20
 bool postLogToPDC(char* deviceNum,char* logValue);
 bool postToServer();
 //2018-02-27清空密碼
 bool postResultToPDC(char* deviceNum,char* Mac);
 // bool ClearPasswordFun();
 //2018-03-16遠程修改Server IP
 bool postChangeServerResultToPDC(char* Mac);
 bool ChangeServerIPFun();
};
 static Hermes *hermes; 
#endif
