/***************************************************************
*  FileName              :    atconfig.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :

*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    ATָ��
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#define	ATCONFIG_GLOBAL

#include "atconfig.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "memory.h"
#include "flash.h"
#include "uart.h"
#include "bsp.h"
#include "cmd.h"
#include "sys_info.h"
#include "stm32f0xx.h"
#include "delay.h"
#include "interrupt.h"
#include "systemwork.h"

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

UINT8  MRBuffer[RX_OK_SIZE];                          //BLE�ظ�����OK
UINT8  MRBufferIndex = 0;
char   at_cmd_buf[TX_CMD_MAX_SIZE];  		         // BLE�����
char   blenamebuf[14] = {"eLinkCarePF"};



/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/

#define  READOKSTATUS1      1                                //��һ�ν���0D 0A
#define  READOKSTATUS2      2                                //�ڶ��ν���0D 0A
#define  READOKSTATUS3      3                                //�����ν���0D 0A
#define  READOKSTATUS4      4                                //���Ĵν���0D 0A
#define  READOKSTATUS5      5                                //�жϽ��յ�OK

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
/**************************************************************
*  Function Name         :   rtc_read
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ��ȡrtcʱ��,�ȶ�ȡTR,�ٶ�ȡDR
***************************************************************/
void Rtc_Read(RTC_t *datastruct)
{
    RTC_TimeTypeDef  RTC_TimeStruct;
    RTC_DateTypeDef  RTC_DateStruct;

    RTC_ClearFlag(RTC_FLAG_RSF);
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
    datastruct->second = RTC_TimeStruct.RTC_Seconds;
    datastruct->minute = RTC_TimeStruct.RTC_Minutes;
    datastruct->hour = RTC_TimeStruct.RTC_Hours;

    RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
    datastruct->day = RTC_DateStruct.RTC_Date;
    datastruct->month = RTC_DateStruct.RTC_Month;
    datastruct->year = RTC_DateStruct.RTC_Year;
}


/**************************************************************
*  Function Name         :   At_FlushRxBuffer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ��������ظ����ջ�����
***************************************************************/
static void At_FlushRxBuffer(void)
{
	MRBufferIndex = 0;
	memset(MRBuffer,'\0',RX_OK_SIZE);
}

/**************************************************************
*  Function Name         :   At_FlushCmdBuffer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ���ATָ��ͻ�����
***************************************************************/
static void At_FlushCmdBuffer(void)
{
	memset((void*)at_cmd_buf,'\0',TX_CMD_MAX_SIZE);
}

/**************************************************************
*  Function Name         :   At_Set_Echo
*  Param                 :   UINT8 mode
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ���û���	0 : �ر�  1: ��
***************************************************************/
void At_Set_Echo(UINT8 mode)
{
	UINT32    command_length;

	MRBufferIndex = 0;
	sprintf((char*)at_cmd_buf,"ATE%d\r\n",mode);
	command_length = strlen((const char *)at_cmd_buf);
	NRF_Uart2Transfer((UINT8 *)&at_cmd_buf[0], command_length);
}

/**************************************************************
*  Function Name         :   Get_Echo_Receive
*  Param                 :   UINT8 rxdata
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ��ȡ2g�ظ�
***************************************************************/
HOST_APP_MSG_ID_E Get_Echo_Receive(UINT8 rxdata)
{
	HOST_APP_MSG_ID_E rxMsgId = HOST_APP_MSG_ID_NONE;

	rxMsgId = At_RecieveDataProcess(rxdata);
	if(rxMsgId != HOST_APP_MSG_ID_NONE)
	{
       	At_FlushRxBuffer();
		At_FlushCmdBuffer();
	}
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   At_Register_System
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ��ѯע���������
***************************************************************/
void At_Register_System(void)
{
	UINT32    command_length;

	sprintf((char*)at_cmd_buf,"AT+COPS?\r\n");
	command_length = strlen((const char *)at_cmd_buf);
	NRF_Uart2Transfer((UINT8 *)&at_cmd_buf[0], command_length);
}

/**************************************************************
*  Function Name         :   Get_Register_Receive
*  Param                 :   UINT8 rxdata
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ��ȡ2G�ظ�
***************************************************************/
HOST_APP_MSG_ID_E Get_Register_Receive(UINT8 rxdata)
{
	HOST_APP_MSG_ID_E rxMsgId = HOST_APP_MSG_ID_NONE;

	rxMsgId = At_RecieveDataProcess(rxdata);
	if(rxMsgId != HOST_APP_MSG_ID_NONE)
	{
		if (strstr((const char *)MRBuffer,"CHINA") != NULL) {
			rxMsgId = HOST_APP_MSG_ID_OK;
		} else {
			rxMsgId = HOST_APP_MSG_ID_ERROR;
		}
		At_FlushRxBuffer();
		At_FlushCmdBuffer();
	}
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   GPRS_Connect_Set
*  Param                 :   void
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ����GPRS�����
***************************************************************/
HOST_APP_MSG_ID_E GPRS_Connect_Set(void)
{
	HOST_APP_MSG_ID_E rxMsgId;
	sprintf((char*)at_cmd_buf,"AT+QICSGP=1,\"CMNET\"\r\n");
	rxMsgId = at_commandsend();
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   Connect_Mode_Set
*  Param                 :   void
*  Return Param          :   void
*  Description           :   �������ӷ�ʽ 0: IP����  1: ��������
***************************************************************/
HOST_APP_MSG_ID_E Connect_Mode_Set(UINT8 mode)
{
	HOST_APP_MSG_ID_E rxMsgId;
	sprintf((char*)at_cmd_buf,"AT+QIDNSIP=%d\r\n",mode);
	rxMsgId = at_commandsend();
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   Enable_GPRS_System
*  Param                 :   void
*  Return Param          :   void
*  Description           :   �����ƶ�����������GPRS����
***************************************************************/
HOST_APP_MSG_ID_E Enable_GPRS_System(void)
{
	HOST_APP_MSG_ID_E rxMsgId;
	sprintf((char*)at_cmd_buf,"AT+QIACT\r\n");
	rxMsgId = at_commandsend();
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   Connect_Sever_IP
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ���ӵ�������
***************************************************************/
HOST_APP_MSG_ID_E Connect_Sever_IP(void)
{
	HOST_APP_MSG_ID_E rxMsgId;
	sprintf((char*)at_cmd_buf,"AT+QIOPEN=\"TCP\",\"121.41.45.196\",\"7989\"\r\n");
	rxMsgId = at_commandsend();
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   At_Close_IPhead
*  Param                 :   UINT8 mode
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ����IPͷ	0 : �ر�  1: ��
***************************************************************/
HOST_APP_MSG_ID_E At_Close_IPhead(UINT8 mode)
{
	HOST_APP_MSG_ID_E rxMsgId;
	sprintf((char*)at_cmd_buf,"AT+QIHEAD=%d\r\n",mode);
	rxMsgId = at_commandsend();
 	return rxMsgId;
}


/**************************************************************
*  Function Name         :   Set_DataLen_ToWire
*  Param                 :   UINT8 length
*  Return Param          :   void
*  Description           :   ���÷��͵����ݳ���
***************************************************************/
void Set_DataLen_ToWire(UINT8 length)
{
	UINT32    command_length;

	MRBufferIndex = 0;
	sprintf((char*)at_cmd_buf,"AT+QISEND=%d\r\n",length);
	command_length = strlen((const char *)at_cmd_buf);
	NRF_Uart2Transfer((UINT8 *)&at_cmd_buf[0], command_length);
}

/**************************************************************
*  Function Name         :   Data_Length_Set
*  Param                 :   UINT8 length
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ���÷��͵����ݳ���
***************************************************************/
HOST_APP_MSG_ID_E Get_DataLen_Receive(UINT8 rxdata)
{
	HOST_APP_MSG_ID_E rxMsgId = HOST_APP_MSG_ID_NONE;
    static UINT8 lengthackstep = READOKSTATUS1;

	if(NULL_CHAR != rxdata)
	{
	   MRBuffer[MRBufferIndex++] = rxdata;
	}
    switch(lengthackstep)
    {
        case READOKSTATUS1:
            if((1 == MRBufferIndex) && (AT_RX_CR == MRBuffer[0])) { //0D
                lengthackstep = READOKSTATUS2;
            } else {
                MRBufferIndex = 0;
            }
            break;

        case READOKSTATUS2:
            if((2 == MRBufferIndex) && (AT_RX_FD == MRBuffer[1])) { //0A
                lengthackstep = READOKSTATUS3;
            } else {
                if (AT_RX_CR == MRBuffer[1]) {
                    MRBuffer[0] = AT_RX_CR;
                    MRBufferIndex = 1;
                } else {
                    MRBufferIndex = 0;
                    lengthackstep = READOKSTATUS1;
                }
            }
            break;

        case READOKSTATUS3:
            if ((3 == MRBufferIndex) && (0x3e == MRBuffer[2])) {    //3E
                lengthackstep = READOKSTATUS4;
            } else {
                if (AT_RX_CR == MRBuffer[2]) {
                    MRBuffer[0] = AT_RX_CR;
                    MRBufferIndex = 1;
                    lengthackstep = READOKSTATUS2;
                } else {
                    MRBufferIndex = 0;
                    lengthackstep = READOKSTATUS1;
                }
            }
            break;

        case READOKSTATUS4:
			rxMsgId = HOST_APP_MSG_ID_OK;
            lengthackstep = READOKSTATUS1;
			At_FlushRxBuffer();
			At_FlushCmdBuffer();
            break;
    }
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   TCP_Close_Connect
*  Param                 :   void
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   �ر�TCP UDP ����
***************************************************************/
HOST_APP_MSG_ID_E TCP_Close_Connect(void)
{
	HOST_APP_MSG_ID_E rxMsgId;
	sprintf((char*)at_cmd_buf,"AT+QICLOSE\r\n");
	rxMsgId = at_commandsend();
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   GPRS_Close_Connect
*  Param                 :   void
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   �ر�GPRS����
***************************************************************/
HOST_APP_MSG_ID_E GPRS_Close_Connect(void)
{
	HOST_APP_MSG_ID_E rxMsgId;
	sprintf((char*)at_cmd_buf,"AT+QIDEACT\r\n");
	rxMsgId = at_commandsend();
 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   Signal_Strength_Check
*  Param                 :   void
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ��ѯ�ź�ǿ��
***************************************************************/
void Signal_Strength_Check(void)
{
	UINT32    command_length;

	MRBufferIndex = 0;
	sprintf((char*)at_cmd_buf,"AT+CSQ\r\n");
	command_length = strlen((const char *)at_cmd_buf);
	NRF_Uart2Transfer((UINT8 *)&at_cmd_buf[0], command_length);
}

/**************************************************************
*  Function Name         :   Get_SignalCheck_Receive
*  Param                 :   UINT8 rxdata
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ��ȡ2G�ظ�
***************************************************************/
HOST_APP_MSG_ID_E Get_SignalCheck_Receive(UINT8 rxdata)
{
	HOST_APP_MSG_ID_E rxMsgId = HOST_APP_MSG_ID_NONE;
	UNION16 dbmdata;

	rxMsgId = At_RecieveDataProcess(rxdata);
	if (rxMsgId == HOST_APP_MSG_ID_OK) {
		if ((0x3a == MRBuffer[6])
			&& (0x20 == MRBuffer[7])) {
			dbmdata.ubyte[0] = MRBuffer[8];
			dbmdata.ubyte[1] = MRBuffer[9];
			if (0x2c == dbmdata.ubyte[1]) {
				g_dbmvalue = MRBuffer[8] - 48;
			} else {
				g_dbmvalue = (MRBuffer[8] - 48)*10 + (MRBuffer[9] - 48);
			}
            At_FlushRxBuffer();
			At_FlushCmdBuffer();
		}
	}

 	return rxMsgId;
}

/**************************************************************
*  Function Name         :   at_commandsend
*  Param                 :   void
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ����AT����
***************************************************************/
HOST_APP_MSG_ID_E  at_commandsend()
{
	UINT32    command_length;
	HOST_APP_MSG_ID_E rxMsgId = HOST_APP_MSG_ID_NONE;
	command_length = strlen((const char *)at_cmd_buf);
	NRF_Uart2Transfer((UINT8 *)&at_cmd_buf[0], command_length);
    rxMsgId = AtLib_ResponseHandle();
	At_FlushRxBuffer();
	At_FlushCmdBuffer();
	return rxMsgId;
}


/**************************************************************
*  Function Name         :   AtLib_ResponseHandle
*  Param                 :   void
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ������յ��Ļظ�
***************************************************************/
HOST_APP_MSG_ID_E AtLib_ResponseHandle()
{
	UINT8 Response_falg=0;
	UINT8 rxdata = 0;
	HOST_APP_MSG_ID_E responseMsgId;
	RTC_t datastruct;
	UINT8 pretime = 0;
	responseMsgId = HOST_APP_MSG_ID_NONE;					 // Reset the message ID
    Rtc_Read(&datastruct);
	pretime = datastruct.second;
	while(1 != Response_falg)                                // ��������ݿɶ��������û���յ��ظ���ȴ��ظ�
    {
        feedwwdg();
		if(1 == GetUart2RxData(&rxdata))
		{
  		    responseMsgId = At_RecieveDataProcess(rxdata);
			if(responseMsgId != HOST_APP_MSG_ID_NONE)
			{
                Response_falg = 1;
			}
		}
        #ifndef KEVIN_DEBUG
		if (1 != Response_falg) {
			Rtc_Read(&datastruct);
			if(is_timeout(pretime,datastruct.second,5))        //�������ճ�ʱ
			{
	           Response_falg = 1;
			}
		}
        #endif
    }
	return responseMsgId;
}


/**************************************************************
*  Function Name         :   At_RecieveDataProcess
*  Param                 :   UINT8 rxData
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ��AT�ظ�����MRBuffer
***************************************************************/
HOST_APP_MSG_ID_E At_RecieveDataProcess(UINT8 rxData)
{
    HOST_APP_MSG_ID_E   rxMsgId = HOST_APP_MSG_ID_NONE;
	static UINT8 readokflag = READOKSTATUS1;

	if(NULL_CHAR != rxData) {
       MRBuffer[MRBufferIndex++] = rxData;
	} else {
       return rxMsgId;
	}
	switch(readokflag)
	{
        case  READOKSTATUS1:							//���յ�0D 0A
		if(AT_RX_CR == MRBuffer[0]) {
			if (MRBufferIndex > 1) {
				if (AT_RX_FD == MRBuffer[1]) {
	 			    readokflag = READOKSTATUS2;         //ת������ڶ��ν���0D 0A
	 		    } else {
	                rxMsgId = HOST_APP_MSG_ID_ERROR;
	 		    }
			}
		} else {
			MRBufferIndex = 0;
		}
		break;

		case  READOKSTATUS2:							//�ٴν��յ�0D 0A
        if((AT_RX_FD== MRBuffer[MRBufferIndex - 1]) && (AT_RX_CR== MRBuffer[MRBufferIndex - 2])) {
			rxMsgId = At_checkEOFMessage(MRBuffer);
			if (HOST_APP_MSG_ID_OK == rxMsgId) {
	        	readokflag = READOKSTATUS1;
	        	MRBufferIndex = 0;
			} else if (HOST_APP_MSG_ID_ERROR == rxMsgId){
	        	readokflag = READOKSTATUS1;
	        	MRBufferIndex = 0;
			} else {
				readokflag = READOKSTATUS3;
			}
        }
		break;

		case  READOKSTATUS3:							//�����ν��յ� 0D
		if(AT_RX_CR == MRBuffer[MRBufferIndex - 1]) {
 			readokflag = READOKSTATUS4;                 //ת��������Ĵν���0D 0A
		} else if(AT_RX_O == MRBuffer[MRBufferIndex - 1]) {
			readokflag = READOKSTATUS5;
		} else {
			readokflag = READOKSTATUS1;
			rxMsgId = HOST_APP_MSG_ID_ERROR;
		}
		break;

		case  READOKSTATUS4:							//���Ĵν��յ� 0A
		if(AT_RX_FD == MRBuffer[MRBufferIndex - 1]) {
 			readokflag = READOKSTATUS5;                 //ת���������OK
		} else {
			readokflag = READOKSTATUS1;
			rxMsgId = HOST_APP_MSG_ID_ERROR;
		}
		break;

		case  READOKSTATUS5:							//�������OK
        if((AT_RX_FD== MRBuffer[MRBufferIndex - 1]) && (AT_RX_CR== MRBuffer[MRBufferIndex - 2])) {
			if (HOST_APP_MSG_ID_OK == At_checkEOFMessage(MRBuffer)) {
	        	readokflag = READOKSTATUS1;
            	rxMsgId = HOST_APP_MSG_ID_OK;
	        	MRBufferIndex = 0;
			} else {
				readokflag = READOKSTATUS1;
				rxMsgId = HOST_APP_MSG_ID_ERROR;
			}
        }
		break;

		default:
			break;
	}
	return rxMsgId;
}

/**************************************************************
*  Function Name         :   At_checkEOFMessage
*  Param                 :   UINT8 rxData
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   �ж�AT�ظ�
***************************************************************/
HOST_APP_MSG_ID_E At_checkEOFMessage(UINT8 * pBuffer)
{
	if(strstr((const char *)pBuffer,"OK") != NULL)
	{
        if (strstr((const char *)pBuffer,"CONNECT") != NULL)
        {
            g_gcalredycnt = 1;                              //GCģ���Ѿ�����
        }
		return HOST_APP_MSG_ID_OK;
	}
	else if (strstr((const char *)pBuffer,"ERROR") != NULL)
	{
		return HOST_APP_MSG_ID_ERROR;
	}
    else if (strstr((const char *)pBuffer,"FAIL") != NULL)
    {
        return HOST_APP_MSG_ID_ERROR;
    }
	else return HOST_APP_MSG_ID_NONE;
}

/**************************************************************
*  Function Name         :   GCwireCont_Check(void)
*  Param                 :   void
*  Return Param          :   HOST_APP_MSG_ID_E
*  Description           :   ��������Ƿ�������  0:δ����  1:������
***************************************************************/
HOST_APP_MSG_ID_E GCwireCont_Check(INT8U rxData)
{
    HOST_APP_MSG_ID_E   rxMsgId = HOST_APP_MSG_ID_NONE;
	static UINT8 readflag = READOKSTATUS1;

	if(NULL_CHAR != rxData) {
       MRBuffer[MRBufferIndex++] = rxData;
	} else {
       return rxMsgId;
	}
	switch(readflag)
	{
        case  READOKSTATUS1:							//���յ�CO
		if(BLE_CONT_C == MRBuffer[0]) {
			if (MRBufferIndex > 1) {
				if (BLE_CONT_O == MRBuffer[1]) {
	 			    readflag = READOKSTATUS2;         //ת������ڶ��ν���0D 0A
	 		    } else {
	                MRBufferIndex = 0;
	 		    }
			}
		} else {
			MRBufferIndex = 0;
		}
		break;

		case  READOKSTATUS2:							//���յ�0D 0A
        if((AT_RX_FD== MRBuffer[MRBufferIndex - 1]) && (AT_RX_CR== MRBuffer[MRBufferIndex - 2])) {
			rxMsgId = At_checkEOFMessage(MRBuffer);
        	readflag = READOKSTATUS1;
        	MRBufferIndex = 0;
    		At_FlushRxBuffer();
    	    At_FlushCmdBuffer();
        }
		break;

		default:
			break;
	}
	return rxMsgId;
}

/**************************************************************
*  Function Name         :   At_Cmd_Init
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ������ʼ�� 0:��ʼ��ʧ�� 1:��ʼ���ɹ�
***************************************************************/
UINT8 At_Cmd_Init(void)
{
	UINT8 rxMsgId = 0;

	if (HOST_APP_MSG_ID_OK == GPRS_Connect_Set()) {					//����GPRS������CHINA MOBILE
		if (HOST_APP_MSG_ID_NONE != Enable_GPRS_System()) {			//����GPRS����
			if (HOST_APP_MSG_ID_NONE != Connect_Mode_Set(0)) {		//ͨ��IP��ַ����
				if (HOST_APP_MSG_ID_OK == At_Close_IPhead(0)) {		//�ر�IPͷ
					if (HOST_APP_MSG_ID_OK == Connect_Sever_IP()) {	//��ʽ��ȷ�ȷ���OK
						rxMsgId = 1;
                        return rxMsgId;
					} else {
						return rxMsgId;
					}
				} else {
					return rxMsgId;
				}
			} else {
				return rxMsgId;
			}
		} else {
			return rxMsgId;
		}
	} else {
		return rxMsgId;
	}
}




/**************************************************************
*  Function Name         :   Disconnect_GC65Cmd
*  Param                 :   void
*  Return Param          :   void
*  Description           :   �Ͽ�2G����
***************************************************************/
void Disconnect_GC65Cmd(void)
{
	TCP_Close_Connect();
	GPRS_Close_Connect();
}


/**************************************************************
*  Function Name         :   GCwire_CmdSend
*  Param                 :   void
*  Return Param          :   void
*  Description           :   2Gģ�������
***************************************************************/
void GCwire_CmdSend(void)
{
	switch(gc65workstate)
	{
		case GCSTATE_CLOSEVW:				/*�رջ���*/
			if (g_wirelessfresh) {
				g_wirelessfresh = 0;
				At_Set_Echo(0);
			}
			break;
		case GCSTATE_SIGNAL:				/*����ź�ǿ��*/
			if (g_wirelessfresh) {
				g_wirelessfresh = 0;
				Signal_Strength_Check();
			}
			break;
		case GCSTATE_SIMCARD:				/*���SIM��*/
			if (g_wirelessfresh) {
				g_wirelessfresh = 0;
				At_Register_System();
			}
			break;
		case GCSTATE_CDMINIT:				/*2Gģ���ʼ��*/
			if (At_Cmd_Init()) {
				gc65workstate = GCSTATE_CNTSEVER;
			} else {
				gc65workstate = GCSTATE_NONE;
				//GC65_POWER_OFF;
			}
			break;
		case GCSTATE_CNTSEVER:				/*���Ӻ�̨������*/

			break;
		case GCSTATE_SETLENGTH:				/*�������ݳ���*/
			if (g_lengthsendflag) {
				g_lengthsendflag = 0;
				SendLen_CmdMsg(cmddatauptype);
			}
			break;
		case GCSTATE_SENDDATA:				/*��������*/
			if (g_datasendflag) {
				g_datasendflag = 0;
				SendData_CmdMsg();
			}
			break;
		case GCSTATE_CLOSETCP:				/*�ر�TCP/UDP*/

			break;
		case GCSTATE_CLOSEGRS:				/*�Ͽ�GPRS*/

			break;
		case GCSTATE_NONE:                  /*2G���Ӻ�,GC����ʱ,ÿ������ź�ǿ��*/
            /*if (blestatus == BLE_CONNECT)
            {
    			if (g_wirelessfresh) {
    				g_wirelessfresh = 0;
    				Signal_Strength_Check();
    			}
            }*/
			break;
		default:

			break;
	}
}

/**************************************************************
*  Function Name         :   Check_Wireless_Rreceive
*  Param                 :   UINT8 data
*  Return Param          :   void
*  Description           :   2Gģ���������
***************************************************************/
void Check_Wireless_Rreceive(UINT8 data)
{
    HOST_APP_MSG_ID_E rxMsgId = HOST_APP_MSG_ID_NONE;

	switch(gc65workstate)
	{
		case GCSTATE_CLOSEVW:				/*�رջ���*/
			if (HOST_APP_MSG_ID_OK == Get_Echo_Receive(data)) {
				gc65workstate = GCSTATE_SIGNAL;
			}
			break;
		case GCSTATE_SIGNAL:				/*����ź�ǿ��*/
			if (HOST_APP_MSG_ID_OK == Get_SignalCheck_Receive(data)) {
				if ((g_dbmvalue > 31)
					|| (g_dbmvalue < 1)) {
					break;
				} else {
				 	gc65workstate = GCSTATE_SIMCARD;
				}
			}
			break;
		case GCSTATE_SIMCARD:				/*���SIM��*/
			if (HOST_APP_MSG_ID_OK == Get_Register_Receive(data)) {
				gc65workstate = GCSTATE_CDMINIT;
			}
			break;
		case GCSTATE_CDMINIT:				/*2Gģ���ʼ��*/

			break;
		case GCSTATE_CNTSEVER:				/*���Ӻ�̨������*/
            rxMsgId = GCwireCont_Check(data);
			if (g_gcalredycnt || (HOST_APP_MSG_ID_OK == rxMsgId)) {	//����connect ok
				/*if (g_dbmvalue >= 11) {
					ShowPicture_2gwireless(4);
				} else if (g_dbmvalue >= 5) {
					ShowPicture_2gwireless(3);
				} else if (g_dbmvalue >= 3) {
					ShowPicture_2gwireless(2);
				} else if (g_dbmvalue >= 1) {
					ShowPicture_2gwireless(1);
				}
				blestatus = BLE_CONNECT;*/
				g_gcalredycnt = 0;
				gc65workstate = GCSTATE_SETLENGTH;
				g_lengthsendflag = 1;
				//cmddatauptype = CLOCK_CALIBRATION;
                g_severconnet = 1;              //���2Gģ�������ӵ�������
			} else if (HOST_APP_MSG_ID_ERROR == rxMsgId)
			{
                g_severconnet = 0;
                gc65workstate = GCSTATE_NONE;
                blestatus = BLE_DISCONNECT;
            }
			break;
		case GCSTATE_SETLENGTH:				/*�������ݳ���*/
			if (HOST_APP_MSG_ID_OK == Get_DataLen_Receive(data)) {
				gc65workstate = GCSTATE_SENDDATA;
				g_datasendflag = 1;
			}
			break;
		case GCSTATE_SENDDATA:				/*��������*/
			if (Get_CmdMsg_Receive(data)) {
				gc65workstate = GCSTATE_SETLENGTH;
				g_lengthsendflag = 1;
				SendCmd_Switch();
			}
			break;
		case GCSTATE_CLOSETCP:				/*�ر�TCP/UDP*/

			break;
		case GCSTATE_CLOSEGRS:				/*�Ͽ�GPRS*/

			break;
		case GCSTATE_NONE:                  /*GC����ʱ,ˢ���ź�ǿ��*/
			/*if (HOST_APP_MSG_ID_OK == Get_SignalCheck_Receive(data)) {
				if ((g_dbmvalue > 31)
					|| (g_dbmvalue < 1)) {
					break;
				} else {
    				if (g_dbmvalue >= 11) {
    					ShowPicture_2gwireless(4);
    				} else if (g_dbmvalue >= 5) {
    					ShowPicture_2gwireless(3);
    				} else if (g_dbmvalue >= 3) {
    					ShowPicture_2gwireless(2);
    				} else if (g_dbmvalue >= 1) {
    					ShowPicture_2gwireless(1);
    				}
				}
			}*/
			break;
		default:
			break;
	}
}



