/***************************************************************
*  FileName              :    uart.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :

*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    uart
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#include "uart.h"
#include "bsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f0xx_usart.h"



INT8U   Uart2RxBuf[UART2_RX_BUF_LEN];
INT8U   head   = 0;
INT8U   tail   = 0;
INT32U  rxFail = 0;


INT8U   Uart1RxBuf[UART1_RX_BUF_LEN];
INT8U   headone   = 0;
INT8U   tailone   = 0;
INT32U  rxFailone = 0;

/**************************************************************
*  Function Name         :   Flush1recbuf
*  Param                 :   void
*  Return Param          :   void
*  Description           :   清除接收缓存
***************************************************************/
void Flush1recbuf(void)
{
   headone   = 0;
   tailone   = 0;
   memset((void *)Uart1RxBuf,'\0',UART1_RX_BUF_LEN); 			//清除接收数组
}

/**************************************************************
*  Function Name         :   uart_send_char
*  Param                 :   INT8U data
*  Return Param          :   void
*  Description           :   uart发送字符
***************************************************************/
void uart1_send_char(INT8U data)
{
    USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

/**************************************************************
*  Function Name         :   NRF_Uart1Transfer
*  Param                 :   INT8U *pTxData, INT32U TxDataLen
*  Return Param          :   void
*  Description           :   uart发送字符串
***************************************************************/
void NRF_Uart1Transfer(INT8U *pTxData, INT32U TxDataLen)
{
	int i;
	for(i = 0; i < TxDataLen; i++)
	{
		 uart1_send_char(pTxData[i]);
	}
}

/**************************************************************
*  Function Name         :   Uart1RxDataHandle
*  Param                 :   void
*  Return Param          :   INT8U
*  Description           :   把串口接收到的数据保存到缓存 (串口中断处理函数里调用)
***************************************************************/
 void Uart1RxDataHandle(INT8U rxdata)
{
    INT8U  mod;
    mod = (headone + 1) % UART1_RX_BUF_LEN;
    if(mod != tailone)                                    // 如果串口缓存未满
    {
        Uart1RxBuf[headone] = rxdata;                     // 保存接收到的数据
        headone = mod;
    }
    else											    //数据未及时读走,发生覆盖，出错
	{                                                   // 否则更新失败计数
        rxFailone++;
    }
}

/**************************************************************
*  Function Name         :   GetUartRxData
*  Param                 :   INT8U *data
*  Return Param          :   INT8U
*  Description           :   从串口接收缓存取一节数据
***************************************************************/
INT8U GetUart1RxData(INT8U *data)
{
    INT8U ret = 0;
    if(tailone != headone)
    {
        *data = Uart1RxBuf[tailone];
        tailone = (tailone + 1) % UART1_RX_BUF_LEN;              // 取出一字节数据
        ret  = 1;
    }
	else
	{
        ret = 0;
    }
    return ret;
}

/**************************************************************
*  Function Name         :   Flushrecbuf
*  Param                 :   void
*  Return Param          :   void
*  Description           :   清除接收缓存
***************************************************************/
void Flush2recbuf(void)
{
   head   = 0;
   tail   = 0;
   memset((void *)Uart2RxBuf,'\0',UART2_RX_BUF_LEN); 			//清除接收数组
}

/**************************************************************
*  Function Name         :   uart_send_char
*  Param                 :   INT8U data
*  Return Param          :   void
*  Description           :   uart发送字符
***************************************************************/
void uart2_send_char(INT8U data)
{
    USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

/**************************************************************
*  Function Name         :   NRF_UartTransfer
*  Param                 :   INT8U *pTxData, INT32U TxDataLen
*  Return Param          :   void
*  Description           :   uart发送字符串
***************************************************************/
void NRF_Uart2Transfer(INT8U *pTxData, INT32U TxDataLen)
{
	int i;
	for(i = 0; i < TxDataLen; i++)
	{
		 uart2_send_char(pTxData[i]);
	}
}

/**************************************************************
*  Function Name         :   UartRxDataHandle
*  Param                 :   void
*  Return Param          :   INT8U
*  Description           :   把串口接收到的数据保存到缓存 (串口中断处理函数里调用)
***************************************************************/
 void Uart2RxDataHandle(INT8U rxdata)
{
    INT8U  mod;
    mod = (head + 1) % UART2_RX_BUF_LEN;
    if(mod != tail)                                    // 如果串口缓存未满
    {
        Uart2RxBuf[head] = rxdata;                      // 保存接收到的数据
        head = mod;
    }
    else											   //数据未及时读走,发生覆盖，出错
	{                                                  // 否则更新失败计数
        rxFail++;
    }
}

/**************************************************************
*  Function Name         :   GetUartRxData
*  Param                 :   INT8U *data
*  Return Param          :   INT8U
*  Description           :   从串口接收缓存取一节数据
***************************************************************/
INT8U GetUart2RxData(INT8U *data)
{
    INT8U ret = 0;
    if(tail != head)
    {
        *data = Uart2RxBuf[tail];
        tail = (tail + 1) % UART2_RX_BUF_LEN;              // 取出一字节数据
        ret  = 1;
    }
	else
	{
        ret = 0;
    }
    return ret;
}

