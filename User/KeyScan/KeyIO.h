
#ifndef  __KEYIO_H__
#define  __KEYIO_H__

#include "port.h"

typedef enum
{
	High=1,
	Low=0,
}IoVal;

typedef enum
{
	Up=0x00,
	Down=0x01,
}KeyState;

void InitBaseIO(void);

KeyState ReadKeyIoState(UCHAR coord);

#endif

