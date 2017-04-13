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
*  Description           :   ������ջ���
***************************************************************/
void Flush1recbuf(void)
{
   headone   = 0;
   tailone   = 0;
   memset((void *)Uart1RxBuf,'\0',UART1_RX_BUF_LEN); 			//�����������
}

/**************************************************************
*  Function Name         :   uart_send_char
*  Param                 :   INT8U data
*  Return Param          :   void
*  Description           :   uart�����ַ�
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
*  Description           :   uart�����ַ���
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
*  Description           :   �Ѵ��ڽ��յ������ݱ��浽���� (�����жϴ����������)
***************************************************************/
 void Uart1RxDataHandle(INT8U rxdata)
{
    INT8U  mod;
    mod = (headone + 1) % UART1_RX_BUF_LEN;
    if(mod != tailone)                                    // ������ڻ���δ��
    {
        Uart1RxBuf[headone] = rxdata;                     // ������յ�������
        headone = mod;
    }
    else											    //����δ��ʱ����,�������ǣ�����
	{                                                   // �������ʧ�ܼ���
        rxFailone++;
    }
}

/**************************************************************
*  Function Name         :   GetUartRxData
*  Param                 :   INT8U *data
*  Return Param          :   INT8U
*  Description           :   �Ӵ��ڽ��ջ���ȡһ������
***************************************************************/
INT8U GetUart1RxData(INT8U *data)
{
    INT8U ret = 0;
    if(tailone != headone)
    {
        *data = Uart1RxBuf[tailone];
        tailone = (tailone + 1) % UART1_RX_BUF_LEN;              // ȡ��һ�ֽ�����
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
*  Description           :   ������ջ���
***************************************************************/
void Flush2recbuf(void)
{
   head   = 0;
   tail   = 0;
   memset((void *)Uart2RxBuf,'\0',UART2_RX_BUF_LEN); 			//�����������
}

/**************************************************************
*  Function Name         :   uart_send_char
*  Param                 :   INT8U data
*  Return Param          :   void
*  Description           :   uart�����ַ�
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
*  Description           :   uart�����ַ���
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
*  Description           :   �Ѵ��ڽ��յ������ݱ��浽���� (�����жϴ����������)
***************************************************************/
 void Uart2RxDataHandle(INT8U rxdata)
{
    INT8U  mod;
    mod = (head + 1) % UART2_RX_BUF_LEN;
    if(mod != tail)                                    // ������ڻ���δ��
    {
        Uart2RxBuf[head] = rxdata;                      // ������յ�������
        head = mod;
    }
    else											   //����δ��ʱ����,�������ǣ�����
	{                                                  // �������ʧ�ܼ���
        rxFail++;
    }
}

/**************************************************************
*  Function Name         :   GetUartRxData
*  Param                 :   INT8U *data
*  Return Param          :   INT8U
*  Description           :   �Ӵ��ڽ��ջ���ȡһ������
***************************************************************/
INT8U GetUart2RxData(INT8U *data)
{
    INT8U ret = 0;
    if(tail != head)
    {
        *data = Uart2RxBuf[tail];
        tail = (tail + 1) % UART2_RX_BUF_LEN;              // ȡ��һ�ֽ�����
        ret  = 1;
    }
	else
	{
        ret = 0;
    }
    return ret;
}

