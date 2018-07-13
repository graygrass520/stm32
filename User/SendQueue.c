
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
* ������				 ��PortSerialInit
* ����	         : ֹͣ�����ж�
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void SendQueueInit(void)
{
//	UCHAR i=0;
	UsSendQueue.QueueIndex=UsSendQueue.QueueEnd=0;
	UsSendQueue.QueueCounter=0;
	UsSendQueue.QueueEmpty=FALSE;
}

/*******************************************************************************
* ������				 ��PortSerialInit
* ����	         : ֹͣ�����ж�
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
BOOL  PlaceSedEvtToSendQueue(UCHAR *frame,USHORT frameCount)
{
	BOOL queueSta=FALSE;
	/* ��������� */
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
* ������				 ��PortSerialInit
* ����	         : ֹͣ�����ж�
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
BOOL GetSedEvtBySendQueue(SendQueueSC *sendevt)
{
	BOOL queueSta=FALSE;
	/* ������п� */
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









