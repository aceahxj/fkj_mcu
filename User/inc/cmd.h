/***************************************************************
*  FileName              :    cmd.h
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :

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
#ifndef _CMD_H
#define _CMD_H

#ifdef  CMD_GLOBAL
#define CMD_EXT
#else
#define CMD_EXT extern
#endif

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "lib_type.h"
#include "stm32f0xx.h"
#include "msg.h"
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

typedef struct
{
    UINT8 year;
    UINT8 month;
    UINT8 day;
    UINT8 hour;
    UINT8 minute;
    UINT8 second;
}RTC_t;

#define is_timeout(pretime, curtime, timeout)	(((curtime)+60-(pretime))%60 >= (timeout))


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

CMD_EXT  RTC_t               current_time; 		//��ǰʱ��

/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/


void checkuartpacket(void);             //uart1 ����������Ϣ
void send_key_value(void);
void send_status(void);
void sendmsgpatch(Msg_Cmd_E cmd, MsgTag* txdMsg);
UINT8 BleSend_CmdMsg(Msg_Cmd_E cmd);
void checkwirelesspacket(void);
void SendLen_CmdMsg(Msg_Cmd_E cmd);
void SendData_CmdMsg(void);
void SendCmd_Switch(void);
UINT8 Get_CmdMsg_Receive(UINT8 data);
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif