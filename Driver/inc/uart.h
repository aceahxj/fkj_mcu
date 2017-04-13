/***************************************************************
*  FileName              :    uart.h
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

#ifndef _UART_H
#define _UART_H

#include "lib_type.h"


#define  UART1_RX_BUF_LEN 64
#define  UART2_RX_BUF_LEN 128

extern INT8U   Uart1RxBuf[UART1_RX_BUF_LEN];
void Flush1recbuf(void);
void uart1_send_char(INT8U data);
void NRF_Uart1Transfer(INT8U *pTxData, INT32U TxDataLen);
INT8U GetUart1RxData(INT8U *data);
void Uart1RxDataHandle(INT8U rxdata);


extern INT8U   Uart2RxBuf[UART2_RX_BUF_LEN];
void Flush2recbuf(void);
void uart2_send_char(INT8U data);
void NRF_Uart2Transfer(INT8U *pTxData, INT32U TxDataLen);
INT8U GetUart2RxData(INT8U *data);
void Uart2RxDataHandle(INT8U rxdata);


#endif
