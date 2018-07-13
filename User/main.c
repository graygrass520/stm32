
/***************************************************************
* 文件名		 ：
* 描述	         : 
* 输入           : 
* 输入           : 
* 输入           : 
* 输出           : 
* 返回           : 
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

/****************************  宏定义 **************************/

#define KEY_PDU_HEAD_OFF    		( 0 )
#define KEY_PDU_KEY_NO_OFF  		( 2 )
#define KEY_PDU_KEY_VAL_OFF 		( 3 )
#define KEY_PDU_CRC_VAL_OFF 		( 4 )
#define KEY_PDU_MAX_NUMS    		( 6 )

/*
-----+-----+-----+-----+-----+-----+
    (1)    | (2) | (3) |    (4)    |
-----+-----+-----+-----+-----+-----+

(1) :按键帧头
(2) :按键序号
(3) :按键值
(4) :crc 校验
*/

#define GEN_KEY_USEING_NUM			( MAX_KEY_IO_NUMS )


/**************************** 类型构造**************************/

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

/**************************** 内部声明**************************/


/**************************** 私有变量**************************/

static KeyValQueueObj stKeyValQueue;
static UCHAR 					KeyPdu[KEY_PDU_MAX_NUMS];


/**************************** 功能函数**************************/



/***************************************************************
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 
***************************************************************/
static void KeyValQueueInit(void)
{
	memset(stKeyValQueue.stKeyValQueue,0,sizeof(KeyValObj)*MAX_COMMOND_KEY);
	stKeyValQueue.QueueIndex=stKeyValQueue.QueueEnd=0;
	stKeyValQueue.QueueCounter=0;
	stKeyValQueue.QueueEmpty=FALSE;
}

/***************************************************************
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 
***************************************************************/
BOOL  PlaceKeyValInQueue(KeyPutState sta,USHORT no)
{
	BOOL queueSta=FALSE;
	/* 如果队列满 */
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
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 
***************************************************************/
static BOOL GetKeyValInQueue(KeyValObj *sendevt)
{
	BOOL queueSta=FALSE;
	/* 如果队列空 */
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
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 
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
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 
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
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 
***************************************************************/
static void EquPoll(void)
{
	if(TRUE == TimeEventGet())
	{

		/* 软时分回调集合 */
//		(void)TaskcollectionCycle(  );
		/* 按键扫描集合 */
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
	/* 配置外部时钟 */	
  RCC_HSEConfig(RCC_HSE_ON);
	/* 等待外部时钟使能完毕 */
	if(RCC_WaitForHSEStartUp()==SUCCESS)
	{   
	    /* 使能FLASH保护单元 */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        /* 等待两个时钟周期 */
    	FLASH_SetLatency(FLASH_Latency_2);
		/* 配置USB时钟 */
		RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
		/* 配置AHB时钟 */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1
							 |RCC_AHBPeriph_DMA2
							 |RCC_AHBPeriph_SRAM
							 |RCC_AHBPeriph_FLITF
							 |RCC_AHBPeriph_CRC
                             |RCC_AHBPeriph_FSMC
                             |RCC_AHBPeriph_SDIO,ENABLE);
		/* 配置HCLK时钟 */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/* 配置PCLK1时钟 */
		RCC_PCLK1Config(RCC_HCLK_Div1);
		/* 配置PCLK2时钟 */
		RCC_PCLK2Config(RCC_HCLK_Div1);
		/* 选择外部时钟作为PLL时钟源 */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
		/* 配置使能PLL时钟 */
		RCC_PLLCmd(ENABLE);
		/* 等待PLL工作稳定 */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);
		/* 选择PLL作为系统的时钟源 */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* 确认PLL已经成为新的系统时钟沿 */
		while(RCC_GetSYSCLKSource()!=0x08);
	}
 }

/***************************************************************
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 
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

