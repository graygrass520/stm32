
#ifndef  __KEYSCAN_H__
#define  __KEYSCAN_H__

#include "port.h"

typedef enum
{
	KeyIsFree=0,
	KeyPutDown=1,
	KeyPutDownL=2,
	KeyPutDouble=3,
}KeyPutState;


#define MAX_COMMOND_KEY  5

#define MAX_KEY_IO_NUMS  64

void KeyScanInit(void);

void KeyScanPoll(void);


BOOL  PlaceKeyValInQueue(KeyPutState sta,USHORT no);

#endif

