/*********************************************************************************************************
*
* File                : systick.h
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

#ifndef __SYSTICK_H
#define __SYSTICK_H 

/* Includes ------------------------------------------------------------------*/	   
#include "stm32f10x.h"
#include "port.h"

#define SYS_ENABLE 	{\
    SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;\
}
             

#define SYS_DISENABLE  {\
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;\
}


/* Private function prototypes -----------------------------------------------*/
void SysTick_Init(void);

void AllTimeBaseCallBack(void);


#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






























