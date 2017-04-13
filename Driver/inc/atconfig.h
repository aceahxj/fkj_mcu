/***************************************************************
*  FileName              :    atconfig.h
*  Copyright             :
*  ModuleName            :
*  
*  CPU                   :
*  RTOS                  :

*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    ������ʼ��
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

#ifndef __ATCONFIG_H
#define __ATCONFIG_H

#ifdef  ATCONFIG_GLOBAL
#define ATCONFIG_EXT
#else
#define ATCONFIG_EXT    extern
#endif

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "lib_type.h"

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

typedef enum
{
	HOST_APP_MSG_ID_NONE    =0,
	HOST_APP_MSG_ID_OK      =1,
	HOST_APP_MSG_ID_ERROR   =2,
}HOST_APP_MSG_ID_E;


/*2Gģ�����״̬*/
typedef enum {
	GCSTATE_NONE		= 0x00,		/*��*/
	GCSTATE_CLOSEVW		= 0x01,		/*�رջ���*/
    GCSTATE_SIGNAL		= 0x02,		/*����ź�ǿ��*/
    GCSTATE_SIMCARD		= 0x03,		/*���SIM��*/
    GCSTATE_CDMINIT		= 0x04,		/*2Gģ���ʼ������*/
    GCSTATE_CNTSEVER	= 0x05,		/*���Ӻ�̨������*/
    GCSTATE_SETLENGTH	= 0x06,		/*�������ݳ���*/
    GCSTATE_SENDDATA	= 0x07,		/*��������*/
    GCSTATE_CLOSETCP	= 0x08,		/*�ر�TCP/UDP*/
    GCSTATE_CLOSEGRS	= 0x09,		/*�Ͽ�GPRS*/
}GCWIRESTATE;

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/
#ifdef  ATCONFIG_GLOBAL
ATCONFIG_EXT   UINT8 g_severconnet = 0;
ATCONFIG_EXT   UINT8 g_dbmvalue = 0;
ATCONFIG_EXT   UINT8 g_lengthsendflag = 0;
ATCONFIG_EXT   UINT8 g_datasendflag = 0;
ATCONFIG_EXT   UINT8 g_gcalredycnt = 0;
volatile ATCONFIG_EXT GCWIRESTATE	gc65workstate = GCSTATE_NONE;
#else
ATCONFIG_EXT   UINT8 g_severconnet;
ATCONFIG_EXT   UINT8 g_dbmvalue;
ATCONFIG_EXT   UINT8 g_lengthsendflag;
ATCONFIG_EXT   UINT8 g_datasendflag;
ATCONFIG_EXT   UINT8 g_gcalredycnt;
volatile ATCONFIG_EXT GCWIRESTATE	gc65workstate;
#endif

/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/
#define NULL_CHAR 0x00

#define AT_RX_O   0x4f     
#define AT_RX_K   0x4b

#define AT_RX_CR  0x0D
#define AT_RX_FD  0x0A

#define BLE_CONT_C  0x43
#define BLE_CONT_O  0x4F

#define BLE_SEND_S  0x53 
#define BLE_SEND_E  0x45
    
#define TX_CMD_MAX_SIZE             32                    //����ATָ������ֽ���
#define RX_OK_SIZE                  32                    //����ATָ��ظ�OK


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

UINT8 At_Cmd_Init(void);
void Disconnect_GC65Cmd(void);
UINT8 Test_Wireless_Signal(void);
void At_Set_Echo(UINT8 mode);
void At_Register_System(void);
HOST_APP_MSG_ID_E  at_commandsend(void);
HOST_APP_MSG_ID_E Data_Length_Set(UINT8 length);
HOST_APP_MSG_ID_E AtLib_ResponseHandle(void);
HOST_APP_MSG_ID_E At_RecieveDataProcess(UINT8 rxData);
HOST_APP_MSG_ID_E At_checkEOFMessage(UINT8 * pBuffer);
void Set_DataLen_ToWire(UINT8 length);
void GCwire_CmdSend(void);
void Check_Wireless_Rreceive(UINT8 data);
HOST_APP_MSG_ID_E Data_SendOK_Ack(void);

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/



#endif
