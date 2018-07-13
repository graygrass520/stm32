
#include "port.h"
#include "portserial.h"
#include "stm32f10x.h"

#define USART1_DR_Base  0x40013804

/*******************************************************************************
* 函数名				 ：PortSerialInit
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void USART1DMAConfig(UCHAR  *TxBuffer1,USHORT  num)
{
    DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
    /* DMA1 Channel4 (triggered by USART1 Tx event) Config */
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuffer1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize =  num;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

		DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA1_Channel4, ENABLE);
	//USART_Cmd(USART2,ENABLE);
}




/*******************************************************************************
* 函数名				 ：PortSerialInit
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void DmaChannal4Handler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4)==SET)
	{
	    DMA_ClearFlag(DMA1_IT_TC4);
	}
	DMA_Cmd(DMA1_Channel4,DISABLE);
}

/*******************************************************************************
* 函数名				 ：PortSerialEnable
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void PortSerialEnable( BOOL xRxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
	ENTER_CRITICAL_SECTION(  );
    if( xRxEnable )
    {
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    }
    else
    {
       USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    }
    
    EXIT_CRITICAL_SECTION(  );
}

/*******************************************************************************
* 函数名				 ：PortSerialInit
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
BOOL PortSerialInit(  ULONG ulBaudRate )
{
  BOOL    bInitialized = TRUE;
	GPIO_InitTypeDef GPIO_InitStruct; 
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	USART_InitStructure.USART_BaudRate = ulBaudRate;

	USART_InitStructure.USART_Parity = USART_Parity_No;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	if( bInitialized )
	{
		ENTER_CRITICAL_SECTION(  );
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);
		USART_Cmd(USART1, ENABLE);
		EXIT_CRITICAL_SECTION(  );
	}
	return bInitialized;
}

/*******************************************************************************
* 函数名				 ：PortSerialInit
* 描述	         : 停止发送中断
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
BOOL PortSerialPutBytes(UCHAR *bytes,USHORT counter)
{

	USART1DMAConfig((u8*)bytes,(USHORT)counter);
	return TRUE;
}




