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
//#include "tBaseNode.h"
#include "stdlib.h"
#include "string.h"

/****************************  宏定义 **************************/

#define TIMING_TASK_MAX  5

/**************************** 类型构造**************************/


typedef struct
{
	TimingSetObj TimObj;
	BOOL 				 IsPren;
}TimingMgObj;


/**************************** 内部声明**************************/



/**************************** 私有变量**************************/

static TimingMgObj TimingTaskSet[TIMING_TASK_MAX] ;

/**************************** 功能函数**************************/


/***************************************************************
* 函数名		 ：
* 描述	         : 
* 输入           : 
* 输入           : 
* 输入           : 
* 输出           : 
* 返回           : 
***************************************************************/
void InitBaseTiming(void)
{
	int i=0;
	for(i=0;i<TIMING_TASK_MAX;i++)
	{
		TimingTaskSet[i].IsPren=FALSE;
	}
}


BOOL PutTask(TimingSetObj task)
{
	BOOL err=FALSE;
	int i=0;
	
	err=FALSE;
	for(i=0;i<TIMING_TASK_MAX;i++)
	{
		if(TimingTaskSet[i].IsPren==FALSE)
		{
			TimingTaskSet[i].TimObj=task;
			TimingTaskSet[i].IsPren=TRUE;
			err=TRUE;
			break;
		}
	}
	return err;
}

void TaskcollectionCycle(void)
{
	int i=0;
	
	for(i=0;i<TIMING_TASK_MAX;i++)
	{
		if(TimingTaskSet[i].IsPren==TRUE)
		{
			if(TimingTaskSet[i].TimObj.pTaskcbf!=NULL)
			{
				TimingTaskSet[i].TimObj.cnt--;
				if(TimingTaskSet[i].TimObj.cnt==0)
				{
				 	TimingTaskSet[i].TimObj.pTaskcbf();
					TimingTaskSet[i].IsPren=FALSE;
					TimingTaskSet[i].TimObj.pTaskcbf=NULL;
				}
			}
		}
	}
	
}

