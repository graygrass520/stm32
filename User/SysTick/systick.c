/*********************************************************************************************************
*
* File                : systick.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include <core_cm3.h>
#include "TimeEvt.h"

static const USHORT CtrlTimeBaseCnt = 20;

static volatile USHORT Counter=0;



void SysTick_Init(void)
{
		/* SystemFrequency / 1000    1ms�ж�һ��
		 * SystemFrequency / 100000	 10us�ж�һ��
		 * SystemFrequency / 1000000 1us�ж�һ��
		 */
		
		/*100ms�ж�һ��*/
      
		if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0��汾
		{ 
			/* Capture error */ 
			while (1);
		}
		SYS_DISENABLE;
}


void AllTimeBaseCallBack(void)
{
	Counter++;
	if(CtrlTimeBaseCnt<=Counter)
	{
		Counter=0;
		TimeEventPost();
	}
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
















