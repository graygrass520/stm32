
#ifndef __BASETIMING_H__
#define __BASETIMING_H__

#include "port.h"

typedef void (*Taskcbf)(void);

typedef struct
{
	Taskcbf pTaskcbf;
	int  cnt;
}TimingSetObj;

void InitBaseTiming(void);

BOOL PutTask(TimingSetObj task);

void TaskcollectionCycle(void);
#endif

