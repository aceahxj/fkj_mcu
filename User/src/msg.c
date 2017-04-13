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
*  Description           :    通信消息解析
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
*  Description           :   接收包前初始化
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
*  Description           :   打包消息
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
*  Description           :   解析消息
***************************************************************/
void msgunpack(MsgTag *rxmsgtag,INT8U rxdata)
{
    INT16U chksum;

    assert_param(NULL != rxmsgtag);
    if(rxmsgtag->msgptr >= MSG_STX_SIZE)                                                    //已经正确接受完消息头、长度，开始接受消息参数、校验和、帧尾
    {
        rxmsgtag->pkt.msg.para[rxmsgtag->msgptr++ - MSG_STX_SIZE] = rxdata;
        if(rxmsgtag->msgptr >= (rxmsgtag->pkt.msg.length + MSG_LENGTH_SIZE))                //接收到指定长度字节的数据
        {
           if(MSG_END == rxmsgtag->pkt.msg.para[rxmsgtag->msgptr - MSG_STX_SIZE - 1])       //判断最后接收是否为帧尾
           {
              chksum = ((rxmsgtag->pkt.msg.para[rxmsgtag->msgptr - MSG_STX_SIZE - 2]) << 8) +
                       (rxmsgtag->pkt.msg.para[rxmsgtag->msgptr - MSG_STX_SIZE - 3]);
              if(msgcalculatesum(rxmsgtag) == chksum)                                       //校验和正确
              {
                 rxmsgtag->msgflag.bits.msgActive = ENABLE;                                 //接收完整消息包
                 rxmsgtag->msgerr = NS_MSG_ERR_NONE;
                 return;
              }
              else
              {
                 rxmsgtag->msgerr = NS_MSG_ERR_CHKSUM;                                      //校验和错误
                 return;
              }
           }
           else
           {
              rxmsgtag->msgerr = NS_MSG_ERR_END;                                            //帧尾错误
              return;
           }
        }
    }
    else
    {
        if(ENABLE == rxmsgtag->msgflag.bits.stxActive)                                      //已经正确接收消息头，开始接收消息长度字节
        {
           rxmsgtag->pkt.part[rxmsgtag->msgptr++] = rxdata;
           if(rxmsgtag->msgptr >= MSG_LENGTH_SIZE)
           {
              if(rxmsgtag->pkt.msg.length > MSG_RX_MAX_LENGTH)                             //超出允许接受最大长度
              {
                  rxmsgtag->msgerr = NS_MSG_ERR_LEN;
                  return;
              }
           }
        }
        else
        {
           if(MSG_STXS == rxdata)                                                         //已经接收帧头首字节，接收到帧头第二个字节
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
                  rxmsgtag->msgerr = NS_MSG_ERR_STX;                                      //消息头错误
                  return;
               }
           }
           else
           {
               if(MSG_STXF == rxdata)                                                     //接收到帧头第一个字节
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
*  Description           :   计算消息包校验和
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
*  Description           :   检查是否解析成功 0: 失败  1: 成功
***************************************************************/
void msgunpackcheck(MsgTag *rxmsgtag)
{
    assert_param(NULL != rxmsgtag);
    if(NS_MSG_ERR_NONE == rxmsgtag->msgerr) {                                   //解析包没有错误
       if(rxmsgtag->msgflag.bits.msgActive == ENABLE)                           //接收到完整的包
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
*  Description           :   发送消息包
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
    //发送帧头
    txdata(MSG_STXF);
    txdata(MSG_STXS);
    //发送消息长度字节和命令
    for(idx = 0;idx < MSG_STX_SIZE;idx++)
    {
       txdata(msgtsg->pkt.part[idx]);
    }
    //发送消息参数
    for(idx = 0;idx < (msgtsg->pkt.msg.length - MSG_END_SIZE);idx++)
    {
       txdata(msgtsg->pkt.msg.para[idx]);
    }
    //发送校验和
    txdata(sumlow);
    txdata(sumhigh);
    //发送帧尾
    txdata(MSG_END);
}

