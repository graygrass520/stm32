#ifndef __SENDQUEUE_H__
#define __SENDQUEUE_H__

#include "port.h"




typedef struct 
{
	volatile UCHAR *SendFrameIndex;
	volatile USHORT SendFrameCount;
	//BOOL   NeedSend;
}SendQueueSC;



void SendQueueInit(void);
BOOL  PlaceSedEvtToSendQueue(UCHAR *frame,USHORT frameCount);
BOOL GetSedEvtBySendQueue(SendQueueSC *sendevt);
void DmaChannal4Handler(void);

#endif
