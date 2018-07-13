
/***************************************************************
* �ļ���		 ��
* ����	         : 
* ����           : 
* ����           : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/

#include "KeyScan.h"
#include "string.h"
#include "KeyIo.h"

/****************************  �궨�� **************************/

#define GENERAL_KEY_ADDR  	( 100 )
#define SWI_MODE_KEY_ADDR 	( 101 )


#define GEN_KEY_USEING_START		( 0  )
#define GEN_KEY_USEING_NUM			( MAX_KEY_IO_NUMS )


#define KEY_PLUS  0x01


/**************************** ���͹���**************************/







typedef struct
{
	USHORT 					PutDownNum;
	USHORT 					KeyTrg;
}KeyStateObj;


/**************************** �ڲ�����**************************/

/**************************** ˽�б���**************************/

static  KeyStateObj  GenStateTab[GEN_KEY_USEING_NUM];
const static UCHAR KeyMaxComNum = MAX_COMMOND_KEY;

/**************************** ���ܺ���**************************/



/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ����           : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
void KeyScanInit(void)
{
	USHORT i=0;
	for(i=0;i<GEN_KEY_USEING_NUM;i++)
	{
		GenStateTab[i].KeyTrg=0;
		GenStateTab[i].PutDownNum=0;
	}
}


KeyPutState AnlyKeyState(UCHAR coor)
{
	KeyPutState putSta;
	UCHAR 	 		keyIoSta=0;
	
	keyIoSta=(UCHAR)ReadKeyIoState(coor);
	
	GenStateTab[coor].KeyTrg = keyIoSta & (keyIoSta ^ GenStateTab[coor].PutDownNum);      // 2
  GenStateTab[coor].PutDownNum = keyIoSta;                         											// 3
	
	putSta=KeyIsFree;
	/* ������� */
	if(GenStateTab[coor].KeyTrg&KEY_PLUS)
	{
		putSta=KeyPutDown;
	}
	else if(GenStateTab[coor].PutDownNum&KEY_PLUS)
	{
		putSta=KeyIsFree;//KeyPutDownL;
	}
	
	return putSta;
}

/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ����           : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
void KeyScanPoll(void)
{
	USHORT i=0;
	USHORT onePollPutNum=0;
	KeyPutState sta;
	
	for(i=0;i<GEN_KEY_USEING_NUM;i++)
	{
		sta=AnlyKeyState(i);
		if(sta!=KeyIsFree)
		{
			onePollPutNum++;
			if(onePollPutNum<KeyMaxComNum)
			{
			 	PlaceKeyValInQueue(sta,i);
			}
			else
			{
				;
			}
		}
	}
}
	



