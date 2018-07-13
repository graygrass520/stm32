
#ifndef  __TIMEEVT_H__
#define  __TIMEEVT_H__

#include "port.h"


//typedef enum
//{
//	SysSoftTimeEvt,
//	SysCtrlTimeEvt,

//}	TimeEvtType;


BOOL TimeEventInit( void );

BOOL TimeEventPost( void );

BOOL TimeEventGet( void );



#endif

