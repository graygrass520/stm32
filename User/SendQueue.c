
#include "SendQueue.h"
//#include ""

#define SEND_FRAME_QUEUE_LEN_MAX 6

typedef struct
{
	SendQueueSC SendQueue[SEND_FRAME_QUEUE_LEN_MAX];
	UCHAR 			QueueIndex;
	UCHAR 			QueueEnd;
	BOOL 				QueueEmpty;
	UCHAR 			QueueCounter;
}Us_SendQueue;



static Us_SendQueue UsSendQueue;

/*******************************************************************************
* 函数名				 ：PortSerialInit
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void SendQueueInit(void)
{
//	UCHAR i=0;
	UsSendQueue.QueueIndex=UsSendQueue.QueueEnd=0;
	UsSendQueue.QueueCounter=0;
	UsSendQueue.QueueEmpty=FALSE;
}

/*******************************************************************************
* 函数名				 ：PortSerialInit
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
BOOL  PlaceSedEvtToSendQueue(UCHAR *frame,USHORT frameCount)
{
	BOOL queueSta=FALSE;
	/* 如果队列满 */
	if(UsSendQueue.QueueCounter==SEND_FRAME_QUEUE_LEN_MAX&&UsSendQueue.QueueEnd==UsSendQueue.QueueIndex)
	{
		UsSendQueue.QueueEmpty=TRUE;
		return	queueSta;
	}
	else
	{
		UsSendQueue.SendQueue[UsSendQueue.QueueEnd].SendFrameIndex=frame;
		UsSendQueue.SendQueue[UsSendQueue.QueueEnd].SendFrameCount=frameCount;
		UsSendQueue.QueueEnd=(UsSendQueue.QueueEnd+1)%SEND_FRAME_QUEUE_LEN_MAX;
		UsSendQueue.QueueCounter++;
		queueSta=TRUE;
	}
	return queueSta;
}

/*******************************************************************************
* 函数名				 ：PortSerialInit
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
BOOL GetSedEvtBySendQueue(SendQueueSC *sendevt)
{
	BOOL queueSta=FALSE;
	/* 如果队列空 */
	if(UsSendQueue.QueueCounter==0&&UsSendQueue.QueueEnd==UsSendQueue.QueueIndex)
	{
		UsSendQueue.QueueEmpty=TRUE;
		return	queueSta;
	}
	else
	{
		*sendevt=UsSendQueue.SendQueue[UsSendQueue.QueueIndex];
		UsSendQueue.QueueIndex=(UsSendQueue.QueueIndex+1)%SEND_FRAME_QUEUE_LEN_MAX;
		UsSendQueue.QueueCounter--;
		queueSta=TRUE;
	}
	return queueSta;
}









