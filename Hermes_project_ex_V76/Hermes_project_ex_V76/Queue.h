/*********************************
>file name:queue.h
>Author:lujunxian
>Created Time:20170923
******************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXSIZE 28

using namespace std;
typedef int QElemType;
typedef struct{
	QElemType Data[MAXSIZE];
	int front;
	int rear;
	int count;
}SQType;
class Queue
{
public:
    //static Queue *Created(void);
	SQType *SQTypeInit();  //隊列初始化
	//void SQTypeInit(SQType *q);  //隊列初始化
	bool SQTypeIsEmpty(SQType *q);   //判斷空隊列
    void SQTypeFree(SQType *q);     //釋放隊列
    bool InSQType(SQType *q,QElemType data);//入隊
    bool OutSQType(SQType *q,QElemType *data);      //出隊
    int SQLTypeLen(SQType *q);       //計算隊列長度
};
#endif