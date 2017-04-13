/***************************************************************
*  FileName                  :    msg.c
*  Copyright                  :
*  ModuleName             :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    ͨ����Ϣ����
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#define     MSG_GLOBAL
/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "msg.h"
#include "lib_type.h"
#include "string.h"
#include "stdlib.h"
#include "atconfig.h"
#include "memory.h"
#include "stm32f0xx.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/

#define BLE_STATUS_SIZE   5


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/**************************************************************
*  Function Name         :   msgprepare
*  Param                 :   MsgTag* msgtag
*  Return Param          :   void
*  Description           :   ���հ�ǰ��ʼ��
***************************************************************/
void msgprepare(MsgTag* msgtag)
{
    assert_param(NULL != msgtag);
    msgtag->msgerr = NS_MSG_ERR_NONE;
    msgtag->msgptr = 0;
    msgtag->predata = 0;
    msgtag->msgflag.ubyte = 0;
}

/**************************************************************
*  Function Name         :   msgpack
*  Param                 :   MsgTag * msgtag,INT8U length,INT8U cmd,INT8U * para
*  Return Param          :   void
*  Description           :   �����Ϣ
***************************************************************/
void msgpack(MsgTag *msgtag,INT8U length,Msg_Cmd_E cmd,INT8U *para)
{
    assert_param(NULL != msgtag);
    assert_param(NULL != para);
    msgtag->pkt.msg.length = length;
    msgtag->pkt.msg.cmd = cmd;
    memorycpy(msgtag->pkt.msg.para,para,length - MSG_END_SIZE);
}


/**************************************************************
*  Function Name         :   msgunpack
*  Param                 :   MsgTag * msgtag,INT8U rxdata
*  Return Param          :   void
*  Description           :   ������Ϣ
***************************************************************/
void msgunpack(MsgTag *rxmsgtag,INT8U rxdata)
{
    INT16U chksum;

    assert_param(NULL != rxmsgtag);
    if(rxmsgtag->msgptr >= MSG_STX_SIZE)                                                    //�Ѿ���ȷ��������Ϣͷ�����ȣ���ʼ������Ϣ������У��͡�֡β
    {
        rxmsgtag->pkt.msg.para[rxmsgtag->msgptr++ - MSG_STX_SIZE] = rxdata;
        if(rxmsgtag->msgptr >= (rxmsgtag->pkt.msg.length + MSG_LENGTH_SIZE))                //���յ�ָ�������ֽڵ�����
        {
           if(MSG_END == rxmsgtag->pkt.msg.para[rxmsgtag->msgptr - MSG_STX_SIZE - 1])       //�ж��������Ƿ�Ϊ֡β
           {
              chksum = ((rxmsgtag->pkt.msg.para[rxmsgtag->msgptr - MSG_STX_SIZE - 2]) << 8) +
                       (rxmsgtag->pkt.msg.para[rxmsgtag->msgptr - MSG_STX_SIZE - 3]);
              if(msgcalculatesum(rxmsgtag) == chksum)                                       //У�����ȷ
              {
                 rxmsgtag->msgflag.bits.msgActive = ENABLE;                                 //����������Ϣ��
                 rxmsgtag->msgerr = NS_MSG_ERR_NONE;
                 return;
              }
              else
              {
                 rxmsgtag->msgerr = NS_MSG_ERR_CHKSUM;                                      //У��ʹ���
                 return;
              }
           }
           else
           {
              rxmsgtag->msgerr = NS_MSG_ERR_END;                                            //֡β����
              return;
           }
        }
    }
    else
    {
        if(ENABLE == rxmsgtag->msgflag.bits.stxActive)                                      //�Ѿ���ȷ������Ϣͷ����ʼ������Ϣ�����ֽ�
        {
           rxmsgtag->pkt.part[rxmsgtag->msgptr++] = rxdata;
           if(rxmsgtag->msgptr >= MSG_LENGTH_SIZE)
           {
              if(rxmsgtag->pkt.msg.length > MSG_RX_MAX_LENGTH)                             //�������������󳤶�
              {
                  rxmsgtag->msgerr = NS_MSG_ERR_LEN;
                  return;
              }
           }
        }
        else
        {
           if(MSG_STXS == rxdata)                                                         //�Ѿ�����֡ͷ���ֽڣ����յ�֡ͷ�ڶ����ֽ�
           {
               if(MSG_STXF == rxmsgtag->predata)
               {
                   rxmsgtag->msgflag.bits.stxActive = ENABLE;
                   rxmsgtag->msgflag.bits.msgActive = DISABLE;
                   rxmsgtag->msgerr = NS_MSG_ERR_NONE;
                   rxmsgtag->msgptr = 0;
                   rxmsgtag->predata = rxdata;
                   return;
               }
               else
               {
                  rxmsgtag->msgerr = NS_MSG_ERR_STX;                                      //��Ϣͷ����
                  return;
               }
           }
           else
           {
               if(MSG_STXF == rxdata)                                                     //���յ�֡ͷ��һ���ֽ�
              {
                  rxmsgtag->predata = rxdata;
                  return;
              }
              else
              {
		          return;
              }
            }
         }
      }
}

/**************************************************************
*  Function Name         :   msgcalculatesum
*  Param                 :   MsgTag * msgtag
*  Return Param          :   void
*  Description           :   ������Ϣ��У���
***************************************************************/
INT16U msgcalculatesum(MsgTag *msgtag)
{
    INT16U idx;
    INT16U chksum = 0;
    assert_param(NULL != msgtag);
    chksum = MSG_STXF + MSG_STXS;
    for(idx = 0;idx < MSG_STX_SIZE;idx++)
    {
       chksum += msgtag->pkt.part[idx];
    }
    for(idx = 0;idx < (msgtag->pkt.msg.length - MSG_END_SIZE);idx++)
    {
       chksum += msgtag->pkt.msg.para[idx];
    }
    return chksum;
}

/**************************************************************
*  Function Name         :   msgunpackcheck
*  Param                 :   MsgTag * rxmsgtag
*  Return Param          :   void
*  Description           :   ����Ƿ�����ɹ� 0: ʧ��  1: �ɹ�
***************************************************************/
void msgunpackcheck(MsgTag *rxmsgtag)
{
    assert_param(NULL != rxmsgtag);
    if(NS_MSG_ERR_NONE == rxmsgtag->msgerr) {                                   //������û�д���
       if(rxmsgtag->msgflag.bits.msgActive == ENABLE)                           //���յ������İ�
       {
          rxmsgtag->RightHandler();
          msgprepare(rxmsgtag);
       }
    } else {
       rxmsgtag->ErrorHandler(rxmsgtag->msgerr);
       msgprepare(rxmsgtag);
    }
}

/**************************************************************
*  Function Name         :   msgsend
*  Param                 :   MsgTag * rxmsgtag
*  Return Param          :   void
*  Description           :   ������Ϣ��
***************************************************************/
void msgsend(MsgTag *msgtsg,void(*txdata)(INT8U data))
{
    INT16U idx;
    INT16U chksum;
    INT8U sumlow;
    INT8U sumhigh;

    assert_param(NULL != msgtsg);
    assert_param(NULL != txdata);

    chksum = msgcalculatesum(msgtsg);
    sumhigh = chksum >> 8;
    sumlow = chksum & 0xff;
    //����֡ͷ
    txdata(MSG_STXF);
    txdata(MSG_STXS);
    //������Ϣ�����ֽں�����
    for(idx = 0;idx < MSG_STX_SIZE;idx++)
    {
       txdata(msgtsg->pkt.part[idx]);
    }
    //������Ϣ����
    for(idx = 0;idx < (msgtsg->pkt.msg.length - MSG_END_SIZE);idx++)
    {
       txdata(msgtsg->pkt.msg.para[idx]);
    }
    //����У���
    txdata(sumlow);
    txdata(sumhigh);
    //����֡β
    txdata(MSG_END);
}

