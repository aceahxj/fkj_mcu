/***************************************************************
*  FileName              :    cmd.c
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
#define CMD_GLOBAL

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "stm32f0xx.h"
#include "cmd.h"
#include "bsp.h"
#include "lib_type.h"
#include "msg.h"
#include "memory.h"
#include "uart.h"
#include "delay.h"
#include "sys_info.h"
#include "flash.h"
#include "buzzerpwm.h"
#include "atconfig.h"
#include "systemwork.h"


volatile UINT8 g_timeoutflag = 0;
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
DEFINE_RX_MSG(Uart2RxMsg,128);                              //��2Gģ��ͨѶ
DEFINE_TX_MSG(Uart2TxMsg,128);

DEFINE_RX_MSG(Uart1RxMsg,64);                               //������ͨѶ
DEFINE_TX_MSG(Uart1TxMsg,64);


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



/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void key_cmd(MsgTag* rxMsg);



/**************************************************************
*  Function Name         :   set_time_data
*  Param                 :   DATASTRUCT * datastruct
*  Return Param          :   void
*  Description           :   ����rtcʱ��
***************************************************************/
void set_time_data(RTC_t * datastruct)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);

    RTC_TimeStruct.RTC_Hours = datastruct->hour;
	RTC_TimeStruct.RTC_Minutes = datastruct->minute;
	RTC_TimeStruct.RTC_Seconds = datastruct->second;
	RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
	if(ERROR == RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct))
	{
	}

    RTC_DateStruct.RTC_WeekDay = 0;
	RTC_DateStruct.RTC_Year = datastruct->year;
    RTC_DateStruct.RTC_Month = datastruct->month;
	RTC_DateStruct.RTC_Date = datastruct->day;
	RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
	if(ERROR == RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct))
	{
	}
}

/**************************************************************
*  Function Name         :   void key_cmd(MsgTag* rxMsg)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ����Ӧ�����
***************************************************************/
void key_cmd(MsgTag* rxMsg)
{
    switch (rxMsg->pkt.msg.para[0])
    {
        case  CMD_VIDEO:        /*�л�����Ƶ*/

        break;

        case  CMD_PHOTO:        /*�л�����Ƭ*/

        break;

        case  CMD_ZOOM_OUT:     /*�Ŵ���Ƶͼ��*/

        break;

        case  CMD_ZOOM_IN:      /*��С��Ƶͼ��*/

        break;

        case  CMD_START_RECORD: /*��ʼ¼��*/

        break;

        case  CMD_STOP_RECORD:  /*ֹͣ¼��*/

        break;

        case  CMD_UP:           /*�Ϸ���Ƭ*/

        break;

        case  CMD_DOWN:         /*�·���Ƭ*/

        break;

        case  CMD_PRINT:        /*��ӡ��Ƭ*/

        break;

        case  CMD_CAPTURE:      /*ץ����Ƭ*/

        break;

        case CMD_PLAY:          /*������Ƶ�ļ�*/
        break;

        case  CMD_SHUT_DOWN:    /*�ػ�*/
            twoshortbuzze();
            led_off();          //�ȹ���ʾ
            instState = ST_STOP;
        break;

        default:
        break;
    }
}

/**************************************************************
*  Function Name         :   get_inst_infor(InstInforType instInforType)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   �����ȡ��������Ϣ
***************************************************************/
void get_inst_infor(InstInforType instInforType, MsgTag* rxMsg)
{
    char *ptr1, *ptr2;

    ptr1 = (char*)rxMsg->pkt.msg.para;
    switch (instInforType)
    {
        case  IN_FACTORY_NAME:          /* �������� */
        ptr2 = instInfor.FACTORY_NAME;
        break;

        case  IN_INST_NAME:             /* �������� */
        ptr2 = instInfor.INST_NAME;
        break;

        case  IN_INST_TYPE:             /* �����ͺ� */
        ptr2 = instInfor.INST_TYPE;
        break;

        case  IN_INST_SN:               /* �������к� */
        ptr2 = instInfor.INST_SN;
        break;

        case  IN_PRODUCT_DATE:          /* ������������ */
        ptr2 = instInfor.PRODUCT_DATE;
        break;

        case  IN_FIRMWARE_VERSION:      /* Firmware�汾 */
        ptr2 = instInfor.FIRMWARE_VERSION;
        break;

        case  IN_PCB_VERSION:           /* PCB�汾 */
        ptr2 = instInfor.PCB_VERSION;
        break;

        case  IN_PCBA_VERSION:          /* PCBA�汾 */
        ptr2 = instInfor.PCBA_VERSION;
        break;
    }
    while (*ptr1 != '\0')
        *ptr2++ = *ptr1++;
}

/**************************************************************
*  Function Name         :   msg1dispatch
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ������յ�HiSi��Ϣ
***************************************************************/
static void msg1dispatch(MsgTag* rxMsg, MsgTag* replyMsg)
{
    //�ȴ�HiSiӦ��
    if ((waitReply.replyFlag)&&(waitReply.cmd == rxMsg->pkt.msg.cmd)) {
        waitReply.replyFlag = FALSE;
        waitReply.cmd = WAIT_NULL;
        waitReply.timeCount = 0;
    }

    switch(rxMsg->pkt.msg.cmd)
    {
    	case SEND_KEY_VALUE:						    //MCU�ϴ�����,HiSiӦ��
    	key_cmd(rxMsg);
    	break;

    	case GET_INST_INFOR:                            //MCU��ȡ������Ϣ,HiSiӦ��
    	get_inst_infor(instInforType, rxMsg);
    	break;

    	case SEND_INST_STATUS:							//MCU�ϴ�����״̬,HiSiӦ��
    	break;

    	case GET_CAL_TIME:							    //HiSi�´�У׼���ʱ��,MCUӦ��
        memorycpy(&current_time,rxMsg->pkt.msg.para,sizeof(current_time));
		set_time_data(&current_time);
		//RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_BKP_VALUE);
		RTC_WaitForSynchro();
        replyMsg->pkt.msg.cmd = GET_CAL_TIME;
        replyMsg->pkt.msg.length = MSG_END_SIZE;
        msgsend(&Uart1TxMsg,uart1_send_char);
        wifiState = ONLINE;
    	break;

    	case WIFI_OFFLINE:							    //HiSi�´�WiFiͨѶ�ж�,MCUӦ��
        wifiState = OFFLINE;
        replyMsg->pkt.msg.cmd = WIFI_OFFLINE;
        replyMsg->pkt.msg.length = MSG_END_SIZE;
        msgsend(&Uart1TxMsg,uart1_send_char);
        wifiState = OFFLINE;
        //���APP����
        actionState = ST_NO_ACTION;
        waitAction.replyFlag = FALSE;
    	break;

    	case VIDEO_CTRL:							    //HiSi������Ƶ״̬
    	replyMsg->pkt.msg.cmd = VIDEO_CTRL;
        replyMsg->pkt.msg.para[0] = rxMsg->pkt.msg.para[0];
        replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
        if (chargeFlag == 0) {
            if (rxMsg->pkt.msg.para[0] == 0x01) {
                instState = ST_WORK;
            } else {
                instState = ST_IDLE;
            }
        }
        wifiState = ONLINE;
        msgsend(&Uart1TxMsg,uart1_send_char);
        //���APP����
        actionState = ST_NO_ACTION;
        waitAction.replyFlag = FALSE;
    	break;

    	case PHOTO_CTRL:							    //HiSi��������
    	replyMsg->pkt.msg.cmd = PHOTO_CTRL;
        replyMsg->pkt.msg.para[0] = rxMsg->pkt.msg.para[0];
        replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
        if (rxMsg->pkt.msg.para[0] == 0x01) {
            if ((ST_WORK == instState)&&(actionState == ST_NO_ACTION)) {
                actionState = ST_CAPTURING;
                //�ȴ��رն�����ʱ
                waitAction.cmd = PHOTO_CTRL;
                waitAction.timeCount = 0;
                waitAction.replyFlag = TRUE;
            }
        } else {
            actionState = ST_NO_ACTION;
            waitAction.replyFlag = FALSE;
        }
        wifiState = ONLINE;
        msgsend(&Uart1TxMsg,uart1_send_char);
    	break;

    	case RECORD_CTRL:							    //HiSi����¼��״̬
    	replyMsg->pkt.msg.cmd = RECORD_CTRL;
        replyMsg->pkt.msg.para[0] = rxMsg->pkt.msg.para[0];
        replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
        if (rxMsg->pkt.msg.para[0] == 0x01) {
            if ((ST_WORK == instState)&&(ST_NO_ACTION == actionState)) {
                actionState = ST_RECORDING;
            }
        } else {
            actionState = ST_NO_ACTION;
        }
        wifiState = ONLINE;
        msgsend(&Uart1TxMsg,uart1_send_char);
    	break;

    	case PRINT_CTRL:							    //HiSi���ƴ�ӡ״̬
    	replyMsg->pkt.msg.cmd = PRINT_CTRL;
        replyMsg->pkt.msg.para[0] = rxMsg->pkt.msg.para[0];
        replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
        if (rxMsg->pkt.msg.para[0] == 0x01) {
            if ((ST_WORK != instState)&&(ST_NO_ACTION == actionState)) {
                actionState = ST_PRINTING;
                //�ȴ��رն�����ʱ
                waitAction.cmd = PRINT_CTRL;
                waitAction.timeCount = 0;
                waitAction.replyFlag = TRUE;
            }
        } else {
            actionState = ST_NO_ACTION;
            waitAction.replyFlag = FALSE;
        }
        wifiState = ONLINE;
        msgsend(&Uart1TxMsg,uart1_send_char);
    	break;

    	default:
    	break;
    }
}

/**************************************************************
*  Function Name         :   checkuartpacket
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ������յ���Ϣ
***************************************************************/
void checkuartpacket(void)
{
    INT8U data;
	while(1 == GetUart1RxData(&data))
	{
        feedwwdg();
        msgunpack(&Uart1RxMsg,data);
		msgunpackcheck(&Uart1RxMsg);
	}
}

/**************************************************************
*  Function Name         :   Uart2RxMsgRightHandler
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ������Ϣ����ȷ����
***************************************************************/
void Uart1RxMsgRightHandler(void)
{
    msg1dispatch(&Uart1RxMsg,&Uart1TxMsg);

}

/**************************************************************
*  Function Name         :   msgdispatch
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ���������Ϣ���󲢻ظ�
***************************************************************/
static void msg1dispatcherror(MsgTag* rxMsg, MsgTag* replyMsg)
{
    replyMsg->pkt.msg.cmd = ERROR_ACK;
    replyMsg->pkt.msg.para[0] = rxMsg->msgerr;
    replyMsg->pkt.msg.length = MSG_END_SIZE + 1;
}

/**************************************************************
*  Function Name         :   Uart1RxMsgErrorHandler
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ������Ϣ��������
***************************************************************/
void Uart1RxMsgErrorHandler(INT8U msgerr)
{
	msg1dispatcherror(&Uart1RxMsg,&Uart1TxMsg);
	delay_ms(3);
	msgsend(&Uart1TxMsg,uart1_send_char);
	delay_ms(3);
}


/**************************************************************
*  Function Name         :   checkwirelesspacket
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ����2Gģ���AT�ظ�
***************************************************************/
void checkwirelesspacket(void)
{
    INT8U data;

	while(1 == GetUart1RxData(&data))
	{
        feedwwdg();
		Check_Wireless_Rreceive(data);
	}
}

/**************************************************************
*  Function Name         :   void send_key_value()
*  Param                 :
*  Return Param          :   void
*  Description           :   ���ͼ�ֵ
***************************************************************/
void send_key_value(void)
{
    sendmsgpatch(SEND_KEY_VALUE, &Uart1TxMsg);      //�����ֵ
    msgsend(&Uart1TxMsg,uart1_send_char);           //�����Ѵ��������
    waitReply.cmd = SEND_KEY_VALUE;                 //�����ȴ�Ӧ��
    waitReply.timeCount = 0;
    waitReply.replyFlag = TRUE;
}

/**************************************************************
*  Function Name         :   void send_status()
*  Param                 :
*  Return Param          :   void
*  Description           :   ���͵�ǰ״̬
***************************************************************/
void send_status(void)
{
    sendmsgpatch(SEND_INST_STATUS, &Uart1TxMsg);    //�����ֵ
    msgsend(&Uart1TxMsg,uart1_send_char);           //�����Ѵ��������
    waitReply.cmd = SEND_INST_STATUS;   //�����ȴ�Ӧ��
    waitReply.timeCount = 0;
    waitReply.replyFlag = TRUE;
}

/**************************************************************
*  Function Name         :   sendmsgpatch
*  Param                 :   Msg_Cmd_E cmd, MsgTag* txdMsg
*  Return Param          :   void
*  Description           :   ������跢�͵���Ϣ��
***************************************************************/
void sendmsgpatch(Msg_Cmd_E cmd, MsgTag* txdMsg)
{
	txdMsg->pkt.msg.cmd = cmd;
	switch(cmd)
	{
        case SEND_KEY_VALUE:            //�ϴ���ֵ
        txdMsg->pkt.msg.para[0] = keyHandleInfor.keyCmd;
    	txdMsg->pkt.msg.length = 1 + MSG_END_SIZE;
		break;

		case GET_INST_INFOR:            //��ȡ������Ϣ
		txdMsg->pkt.msg.para[0] = instInforType;
    	txdMsg->pkt.msg.length = 1 + MSG_END_SIZE;
		break;

		case SEND_INST_STATUS:          //�ϴ�����״̬
		txdMsg->pkt.msg.para[0] = instState;
        txdMsg->pkt.msg.para[1] = selfInfor.errorCode;
        txdMsg->pkt.msg.para[2] = cellState.remainCharge;
    	txdMsg->pkt.msg.length = 3 + MSG_END_SIZE;
		break;

		default:
		break;
	}
}


/**************************************************************
*  Function Name         :   void SendCmd_Switch(void)
*  Param                 :   Msg_Cmd_E cmd
*  Return Param          :   void
*  Description           :   ������Ϣ�ϴ������л�
***************************************************************/
void SendCmd_Switch(void)
{
	switch(cmddatauptype)
	{   /*
		case SEND_STATION_INFORMA:    //�ϴ�������Ϣ
		if (instinfo <= IN_PCBA_VERSION) {
			instinfo++;
		} else {
			cmddatauptype = READ_CRUTUSER_INFO;
		}
		break;
		case SEND_STATION_STATUS:    //�ϴ�����״̬

		break;
		case SEND_BATTERY_STATUS:    //�ϴ����״̬

		break;
		case SEND_RECORD_DATA:	   //�ϴ���¼����

		break;
		case SEND_RECORD_CURVE:    //�ϴ���¼����

		break;
		case SEND_SYNCHRON_DATA:    //�ϴ�ͬ��������

		break;
		case SEND_ERROR_MESSAGE:    //�ϴ�������Ϣ

		break;
		case READ_REGUSER_INFO:    //��ȡע���û���Ϣ

		break;
		case READ_CRUTUSER_INFO:    //��ȡ��ǰ�û���Ϣ
		cmddatauptype = SEND_SYNCHRON_DATA;
		gc65workstate = GCSTATE_NONE;
		g_lengthsendflag = 0;
		if (g_dbmvalue >= 11) {
			ShowPicture_2gwireless(4);
		} else if (g_dbmvalue >= 5) {
			ShowPicture_2gwireless(3);
		} else if (g_dbmvalue >= 3) {
			ShowPicture_2gwireless(2);
		} else if (g_dbmvalue >= 1) {
			ShowPicture_2gwireless(1);
		}
		blestatus = BLE_CONNECT;

		break;
		case CLOCK_CALIBRATION:    //У׼ʱ��
		cmddatauptype = SEND_STATION_INFORMA;
        instinfo = IN_FACTORY_NAME;
		break;  */
		default:
		break;
	}
}



/**************************************************************
*  Function Name         :   UINT8 SendLen_CmdMsg(Msg_Cmd_E cmd)
*  Param                 :   Msg_Cmd_E cmd
*  Return Param          :   void
*  Description           :   �ϴ�cmd��Ӧ��������Ϣ�ĳ���
***************************************************************/
void SendLen_CmdMsg(Msg_Cmd_E cmd)
{
	Set_DataLen_ToWire(Uart2TxMsg.pkt.msg.length + 3);
}

/**************************************************************
*  Function Name         :   void BleSend_CmdMsg(Msg_Cmd_E cmd)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   �ϴ�cmd��Ӧ��������Ϣ
***************************************************************/
void SendData_CmdMsg(void)
{
	msgsend(&Uart2TxMsg,uart2_send_char);//�����Ѵ��������
}


/**************************************************************
*  Function Name         :   Uart2RxMsgRightHandler
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ������Ϣ����ȷ����
***************************************************************/
void Uart2RxMsgRightHandler(void)
{
    msg1dispatch(&Uart1RxMsg,&Uart1TxMsg);
}


/**************************************************************
*  Function Name         :   msgdispatch
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ���������Ϣ���󲢻ظ�
***************************************************************/
static void msg2dispatcherror(MsgTag* rxMsg, MsgTag* replyMsg)
{
    replyMsg->pkt.msg.cmd = ERROR_ACK;
    replyMsg->pkt.msg.para[0] = rxMsg->msgerr;
    replyMsg->pkt.msg.length = MSG_END_SIZE + 1;
}

/**************************************************************
*  Function Name         :   Uart2RxMsgErrorHandler
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   ������Ϣ��������
***************************************************************/
void Uart2RxMsgErrorHandler(INT8U msgerr)
{
	msg2dispatcherror(&Uart2RxMsg,&Uart2TxMsg);
	msgsend(&Uart2TxMsg,uart2_send_char);
}


/**************************************************************
*  Function Name         :   msg2dispatch
*  Param                 :   MsgTag* rxMsg, MsgTag* txdMsg
*  Return Param          :   void
*  Description           :   �ж�2G�����Ƿ��ϴ��ɹ� 0: ʧ�� 1: �ɹ�
***************************************************************/
static UINT8 msg2dispatch(MsgTag* rxMsg, MsgTag* txdMsg)
{
    Msg_Cmd_E cmd;
	UINT8 reparm = 0;
	//RTC_t clocktime;

	cmd = txdMsg->pkt.msg.cmd;
	if (rxMsg->pkt.msg.cmd != cmd)
		return reparm;
	else {
		switch(cmd)
		{
            /*
	        case SEND_STATION_INFORMA:
			case SEND_STATION_STATUS:
			case SEND_SYNCHRON_DATA:							//�ϴ���¼ʱ��
			case SEND_RECORD_DATA:				   				//�ϴ���¼����
			case SEND_RECORD_CURVE:				   				//�ϴ���������
	        if(rxMsg->pkt.msg.para[0])
				reparm = 1;
			break;

			case READ_CRUTUSER_INFO:                             //��ȡapp�´���ʹ���û���Ϣ
			if (Getcurt_Userinfo(rxMsg, txdMsg))
				reparm = 1;
			break;

			case CLOCK_CALIBRATION:                             //У׼ʱ��
			Rtc_Read(&clocktime);                               //��ȡ����
	        memorycpy(&clocktime,&(rxMsg->pkt.msg.para[0]),DATALENGTH);
			set_time_data(&clocktime);
			//Times_Show(1, &(g_timeset.currentdata));
			reparm = 1;
			break;
*/
			default:                                            //������ϢУ�����
			break;
		}
	}
	return reparm;
}



/**************************************************************
*  Function Name         :   UINT8 Get_CmdMsg_Receive(Msg_Cmd_E cmd)
*  Param                 :   UINT8 data
*  Return Param          :   void
*  Description           :   ��ȡ2G�ظ�����Ϣ
***************************************************************/
UINT8 Get_CmdMsg_Receive(UINT8 data)
{
	UINT8 reparm = 0;

    msgunpack(&Uart2RxMsg,data);  	//����app�Ļظ���
    if(NS_MSG_ERR_NONE == Uart2RxMsg.msgerr) {   //������û�д���
       if(Uart2RxMsg.msgflag.bits.msgActive == ENABLE) //���յ������İ�
       {
          if (msg2dispatch(&Uart2RxMsg,&Uart2TxMsg)) {
		  	reparm = 1;
			msgprepare(&Uart2RxMsg);
          } else {
          	msgprepare(&Uart2RxMsg);
		  }
       }
    } else {
       //UartRxMsg.ErrorHandler(UartRxMsg.msgerr);
    }
	return reparm;
}


