/*********************************
>file name:queue.cpp
>Author:lujunxian
>Created Time:20170923
******************************/
#include <Queue.h>


SQType *Queue::SQTypeInit()
{
	SQType *q;

	if(q=(SQType*)malloc(sizeof(SQType)))
	{
		q->front=0;
		q->rear=0;
		q->count=0;
		return q;
	}
	else
	{
		return NULL;
	}
	
}

bool Queue::SQTypeIsEmpty(SQType *q)    //判斷是否為空
{
    bool temp;
    temp = (q->front == q->rear);//0--false  1--true
    return (temp);
}


bool Queue::InSQType(SQType *q, QElemType data)  //進隊列
{
	if(((q->rear+1)%MAXSIZE)==(q->front))  //dui man
	{
		q->count=28;
		q->front=((q->front+1)%MAXSIZE);
		q->Data[q->rear]=data;
		q->rear=((q->rear+1)%MAXSIZE);
	}
	else
	{
		q->count=(q->count+1);
		q->Data[q->rear]=data;
		q->rear=((q->rear+1)%MAXSIZE);
	}
	
    return true;
}

bool Queue::OutSQType(SQType *q,QElemType *data)   //出隊列
{
    if(SQTypeIsEmpty(q))  
    {  
        return false;  
    }  
    else  
    {  
        *data = q->Data[q->front];       //先進先出
        q->front = ((q->front+1)%MAXSIZE);      //移到下一位置
		//q->front = (q->front+1);
        return true;  
    }  
}

int Queue::SQLTypeLen(SQType *q)    //計算element number
{
    int temp;
	int iLen;
	temp=(((q->rear)-(q->front)+MAXSIZE)%MAXSIZE);
	if(q->count==28)
	{
		iLen=28;
		return iLen;
	}
	else
	{
		return temp;
	}
	
	
}
