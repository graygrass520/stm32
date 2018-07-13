
/***************************************************************
* �ļ���		 ��
* ����	         : 
* ����           : 
* ����           : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/

#include "BaseTiming.h"
#include "TimeEvt.h"
#include "KeyScan.h"
#include "string.h"
#include "crc.h"
#include "KeyIo.h"
#include "stm32f10x.h"
#include "portserial.h"
#include "Systick.h"

/****************************  �궨�� **************************/

#define KEY_PDU_HEAD_OFF    		( 0 )
#define KEY_PDU_KEY_NO_OFF  		( 2 )
#define KEY_PDU_KEY_VAL_OFF 		( 3 )
#define KEY_PDU_CRC_VAL_OFF 		( 4 )
#define KEY_PDU_MAX_NUMS    		( 6 )

/*
-----+-----+-----+-----+-----+-----+
    (1)    | (2) | (3) |    (4)    |
-----+-----+-----+-----+-----+-----+

(1) :����֡ͷ
(2) :�������
(3) :����ֵ
(4) :crc У��
*/

#define GEN_KEY_USEING_NUM			( MAX_KEY_IO_NUMS )


/**************************** ���͹���**************************/

typedef struct
{
	UCHAR 			KeyNo;
	KeyPutState KeySta;
}KeyValObj;


typedef struct
{
	UCHAR 			QueueIndex;
	UCHAR 			QueueEnd;
	BOOL 				QueueEmpty;
	UCHAR 			QueueCounter;
  KeyValObj stKeyValQueue[MAX_COMMOND_KEY];
}KeyValQueueObj;

/**************************** �ڲ�����**************************/


/**************************** ˽�б���**************************/

static KeyValQueueObj stKeyValQueue;
static UCHAR 					KeyPdu[KEY_PDU_MAX_NUMS];


/**************************** ���ܺ���**************************/



/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
static void KeyValQueueInit(void)
{
	memset(stKeyValQueue.stKeyValQueue,0,sizeof(KeyValObj)*MAX_COMMOND_KEY);
	stKeyValQueue.QueueIndex=stKeyValQueue.QueueEnd=0;
	stKeyValQueue.QueueCounter=0;
	stKeyValQueue.QueueEmpty=FALSE;
}

/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
BOOL  PlaceKeyValInQueue(KeyPutState sta,USHORT no)
{
	BOOL queueSta=FALSE;
	/* ��������� */
	if(stKeyValQueue.QueueCounter==MAX_COMMOND_KEY&&stKeyValQueue.QueueEnd==stKeyValQueue.QueueIndex)
	{
		stKeyValQueue.QueueEmpty=TRUE;
		return	queueSta;
	}
	else
	{
		stKeyValQueue.stKeyValQueue[stKeyValQueue.QueueEnd].KeySta=sta;
		stKeyValQueue.stKeyValQueue[stKeyValQueue.QueueEnd].KeyNo=no;
		stKeyValQueue.QueueEnd=(stKeyValQueue.QueueEnd+1)%MAX_COMMOND_KEY;
		stKeyValQueue.QueueCounter++;
		queueSta=TRUE;
	}
	return queueSta;
}

/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
static BOOL GetKeyValInQueue(KeyValObj *sendevt)
{
	BOOL queueSta=FALSE;
	/* ������п� */
	if(stKeyValQueue.QueueCounter==0&&stKeyValQueue.QueueEnd==stKeyValQueue.QueueIndex)
	{
		stKeyValQueue.QueueEmpty=TRUE;
		return	queueSta;
	}
	else
	{
		*sendevt=stKeyValQueue.stKeyValQueue[stKeyValQueue.QueueIndex];
		stKeyValQueue.QueueIndex=(stKeyValQueue.QueueIndex+1)%MAX_COMMOND_KEY;
		stKeyValQueue.QueueCounter--;
		queueSta=TRUE;
	}
	return queueSta;
}

/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
static void SendKeyVal(KeyValObj obj)
{
	USHORT crcCode;
	
	if(obj.KeySta==KeyIsFree||obj.KeyNo>= GEN_KEY_USEING_NUM)
	{
		return;
	}
	else
	{
		//
		KeyPdu[KEY_PDU_HEAD_OFF]=0x6a;
		KeyPdu[KEY_PDU_HEAD_OFF+1]=0xa6;
		
		KeyPdu[KEY_PDU_KEY_NO_OFF]=obj.KeyNo;
		KeyPdu[KEY_PDU_KEY_VAL_OFF]=(UCHAR)obj.KeySta;
		
		crcCode=usMBCRC16(KeyPdu,KEY_PDU_CRC_VAL_OFF);
		
		KeyPdu[KEY_PDU_CRC_VAL_OFF+1]=(UCHAR)((crcCode>>8)&0x00ff);
		KeyPdu[KEY_PDU_CRC_VAL_OFF]=(UCHAR)(crcCode&0x00ff);
		
		PortSerialPutBytes(KeyPdu,KEY_PDU_MAX_NUMS);
	}
}

/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
static void ScanKeyQueue(void)
{
	KeyValObj keyObj;
	
	if(GetKeyValInQueue(&keyObj)==TRUE)
	{
		SendKeyVal(keyObj);
	}
}


/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
static void EquPoll(void)
{
	if(TRUE == TimeEventGet())
	{

		/* ��ʱ�ֻص����� */
//		(void)TaskcollectionCycle(  );
		/* ����ɨ�輯�� */
		(void)KeyScanPoll(  );
		
//		(void)ScanKeyQueue(  );
	}
}	

static void NivcInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


static void RCC_Configuration(void)
{	
	RCC_DeInit();
	/* �����ⲿʱ�� */	
  RCC_HSEConfig(RCC_HSE_ON);
	/* �ȴ��ⲿʱ��ʹ����� */
	if(RCC_WaitForHSEStartUp()==SUCCESS)
	{   
	    /* ʹ��FLASH������Ԫ */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        /* �ȴ�����ʱ������ */
    	FLASH_SetLatency(FLASH_Latency_2);
		/* ����USBʱ�� */
		RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
		/* ����AHBʱ�� */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1
							 |RCC_AHBPeriph_DMA2
							 |RCC_AHBPeriph_SRAM
							 |RCC_AHBPeriph_FLITF
							 |RCC_AHBPeriph_CRC
                             |RCC_AHBPeriph_FSMC
                             |RCC_AHBPeriph_SDIO,ENABLE);
		/* ����HCLKʱ�� */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/* ����PCLK1ʱ�� */
		RCC_PCLK1Config(RCC_HCLK_Div1);
		/* ����PCLK2ʱ�� */
		RCC_PCLK2Config(RCC_HCLK_Div1);
		/* ѡ���ⲿʱ����ΪPLLʱ��Դ */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
		/* ����ʹ��PLLʱ�� */
		RCC_PLLCmd(ENABLE);
		/* �ȴ�PLL�����ȶ� */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);
		/* ѡ��PLL��Ϊϵͳ��ʱ��Դ */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* ȷ��PLL�Ѿ���Ϊ�µ�ϵͳʱ���� */
		while(RCC_GetSYSCLKSource()!=0x08);
	}
 }

/***************************************************************
* ������		 ��
* ����	         : 
* ����           : 
* ���           : 
* ����           : 
***************************************************************/
int main(void)
{
	
	NivcInit();
	RCC_Configuration();
	InitBaseIO();
	KeyScanInit();
	KeyValQueueInit();
	PortSerialInit(115200);
	SysTick_Init();
	
	SYS_ENABLE;
	while(1)
	{
		(void)EquPoll(  );
	}
}

