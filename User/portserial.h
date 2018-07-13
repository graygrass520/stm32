


#ifndef __PORTSERIAL_H__
#define __PORTSERIAL_H__

#include "port.h"

void DmaChannal4Handler(void);

void PortSerialEnable( BOOL xRxEnable );

BOOL PortSerialInit( ULONG ulBaudRate);

BOOL PortSerialPutBytes(UCHAR *bytes,USHORT counter);


BOOL PortSerialGetByte( CHAR * pucByte );


#endif 




