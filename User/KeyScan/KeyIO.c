
/***************************************************************
* 文件名		 ：
* 描述	         : 
* 输入           : 
* 输入           : 
* 输入           : 
* 输出           : 
* 返回           : 
***************************************************************/

#include "KeyIO.h"
#include "stm32f10x_gpio.h"

/****************************  宏定义 **************************/

#define KEY_MAX_NUM	64

#define KEY_DOWN_VALUE 0

/**************************** 类型构造**************************/

typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}BaseIoObj;


/**************************** 内部声明**************************/


/**************************** 私有变量**************************/

static const BaseIoObj US_KeyBoardIoMap[KEY_MAX_NUM]={
	
				{GPIOD,GPIO_Pin_13},//1
				{GPIOD,GPIO_Pin_14},//2
				{GPIOD,GPIO_Pin_15},//3
				{GPIOC,GPIO_Pin_6},//4
				
				{GPIOC,GPIO_Pin_7},//5
				{GPIOC,GPIO_Pin_8},//6
				{GPIOC,GPIO_Pin_9},//7
				{GPIOA,GPIO_Pin_8},//8
				
				{GPIOC,GPIO_Pin_10},//9
				{GPIOC,GPIO_Pin_11},//10
				{GPIOC,GPIO_Pin_12},//11
				{GPIOD,GPIO_Pin_0},//12
				
				{GPIOD,GPIO_Pin_1},//13
				{GPIOD,GPIO_Pin_2},//14
				{GPIOD,GPIO_Pin_3},//15
				{GPIOD,GPIO_Pin_4},//16
				
				{GPIOD,GPIO_Pin_5},//17
				{GPIOD,GPIO_Pin_6},//18
				{GPIOD,GPIO_Pin_7},//19
				{GPIOB,GPIO_Pin_5},//20
				
				{GPIOB,GPIO_Pin_6},//21
				{GPIOB,GPIO_Pin_7},//22
				{GPIOB,GPIO_Pin_8},//23
				{GPIOB,GPIO_Pin_9},//24
				
				{GPIOE,GPIO_Pin_0},//25
				{GPIOE,GPIO_Pin_1},//26
				{GPIOE,GPIO_Pin_2},//27
				{GPIOE,GPIO_Pin_3},//28
				
				{GPIOE,GPIO_Pin_4},//29
				{GPIOE,GPIO_Pin_5},//30
				{GPIOE,GPIO_Pin_6},//31
				{GPIOC,GPIO_Pin_13},//32

				{GPIOA,GPIO_Pin_0},//33
				{GPIOA,GPIO_Pin_1},//34
				{GPIOA,GPIO_Pin_2},//35
				{GPIOA,GPIO_Pin_3},//36
				
				{GPIOA,GPIO_Pin_4},//37
				{GPIOA,GPIO_Pin_5},//38
				{GPIOA,GPIO_Pin_6},//39
				{GPIOA,GPIO_Pin_7},//40
				
				{GPIOC,GPIO_Pin_4},//41
				{GPIOC,GPIO_Pin_5},//42
				{GPIOB,GPIO_Pin_0},//43
				{GPIOB,GPIO_Pin_1},//44
				
				{GPIOE,GPIO_Pin_7},//45
				{GPIOE,GPIO_Pin_8},//46
				{GPIOE,GPIO_Pin_9},//47
				{GPIOE,GPIO_Pin_10},//48
				
				{GPIOE,GPIO_Pin_11},//49
				{GPIOE,GPIO_Pin_12},//50
				{GPIOE,GPIO_Pin_13},//51
				{GPIOE,GPIO_Pin_14},//52
				
				{GPIOE,GPIO_Pin_15},//53
				{GPIOB,GPIO_Pin_10},//54
				{GPIOB,GPIO_Pin_11},//55
				{GPIOB,GPIO_Pin_12},//56
				
				{GPIOB,GPIO_Pin_13},//57
				{GPIOB,GPIO_Pin_14},//58
				{GPIOB,GPIO_Pin_15},//59
				{GPIOD,GPIO_Pin_8},//60
				
				{GPIOD,GPIO_Pin_9},//61
				{GPIOD,GPIO_Pin_10},//62
				{GPIOD,GPIO_Pin_11},//63
				{GPIOD,GPIO_Pin_12},//64
};
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

void InitBaseIO(void)
{
	int i=0;
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	
	for(i=0;i<KEY_MAX_NUM;i++)
	{

		GPIO_InitStructure.GPIO_Pin=US_KeyBoardIoMap[i].GPIO_Pin;
		GPIO_Init(US_KeyBoardIoMap[i].GPIOx,&GPIO_InitStructure);
	}
}

KeyState  ReadKeyIoState(UCHAR coord)
{
	KeyState sta=Down;
	BitAction tmp=Bit_RESET;
	
	tmp=(BitAction)GPIO_ReadInputDataBit(US_KeyBoardIoMap[coord].GPIOx,US_KeyBoardIoMap[coord].GPIO_Pin);
	
	if(tmp==KEY_DOWN_VALUE)
	{
		sta=Down;
	}
	else
	{
		sta=Up;
	}
	return sta;
}


