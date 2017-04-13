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
*  Description           :    通信消息处理
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
DEFINE_RX_MSG(Uart2RxMsg,128);                              //与2G模块通讯
DEFINE_TX_MSG(Uart2TxMsg,128);

DEFINE_RX_MSG(Uart1RxMsg,64);                               //与主板通讯
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
*  Description           :   设置rtc时间
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
*  Description           :   按键应答后处理
***************************************************************/
void key_cmd(MsgTag* rxMsg)
{
    switch (rxMsg->pkt.msg.para[0])
    {
        case  CMD_VIDEO:        /*切换到视频*/

        break;

        case  CMD_PHOTO:        /*切换到照片*/

        break;

        case  CMD_ZOOM_OUT:     /*放大视频图像*/

        break;

        case  CMD_ZOOM_IN:      /*缩小视频图像*/

        break;

        case  CMD_START_RECORD: /*开始录像*/

        break;

        case  CMD_STOP_RECORD:  /*停止录像*/

        break;

        case  CMD_UP:           /*上翻照片*/

        break;

        case  CMD_DOWN:         /*下翻照片*/

        break;

        case  CMD_PRINT:        /*打印照片*/

        break;

        case  CMD_CAPTURE:      /*抓拍照片*/

        break;

        case CMD_PLAY:          /*播放视频文件*/
        break;

        case  CMD_SHUT_DOWN:    /*关机*/
            twoshortbuzze();
            led_off();          //先关显示
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
*  Description           :   保存读取的仪器信息
***************************************************************/
void get_inst_infor(InstInforType instInforType, MsgTag* rxMsg)
{
    char *ptr1, *ptr2;

    ptr1 = (char*)rxMsg->pkt.msg.para;
    switch (instInforType)
    {
        case  IN_FACTORY_NAME:          /* 厂家名称 */
        ptr2 = instInfor.FACTORY_NAME;
        break;

        case  IN_INST_NAME:             /* 仪器名称 */
        ptr2 = instInfor.INST_NAME;
        break;

        case  IN_INST_TYPE:             /* 仪器型号 */
        ptr2 = instInfor.INST_TYPE;
        break;

        case  IN_INST_SN:               /* 仪器序列号 */
        ptr2 = instInfor.INST_SN;
        break;

        case  IN_PRODUCT_DATE:          /* 仪器生产日期 */
        ptr2 = instInfor.PRODUCT_DATE;
        break;

        case  IN_FIRMWARE_VERSION:      /* Firmware版本 */
        ptr2 = instInfor.FIRMWARE_VERSION;
        break;

        case  IN_PCB_VERSION:           /* PCB版本 */
        ptr2 = instInfor.PCB_VERSION;
        break;

        case  IN_PCBA_VERSION:          /* PCBA版本 */
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
*  Description           :   处理接收的HiSi消息
***************************************************************/
static void msg1dispatch(MsgTag* rxMsg, MsgTag* replyMsg)
{
    //等待HiSi应答
    if ((waitReply.replyFlag)&&(waitReply.cmd == rxMsg->pkt.msg.cmd)) {
        waitReply.replyFlag = FALSE;
        waitReply.cmd = WAIT_NULL;
        waitReply.timeCount = 0;
    }

    switch(rxMsg->pkt.msg.cmd)
    {
    	case SEND_KEY_VALUE:						    //MCU上传键盘,HiSi应答
    	key_cmd(rxMsg);
    	break;

    	case GET_INST_INFOR:                            //MCU获取仪器信息,HiSi应答
    	get_inst_infor(instInforType, rxMsg);
    	break;

    	case SEND_INST_STATUS:							//MCU上传仪器状态,HiSi应答
    	break;

    	case GET_CAL_TIME:							    //HiSi下传校准后的时间,MCU应答
        memorycpy(&current_time,rxMsg->pkt.msg.para,sizeof(current_time));
		set_time_data(&current_time);
		//RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_BKP_VALUE);
		RTC_WaitForSynchro();
        replyMsg->pkt.msg.cmd = GET_CAL_TIME;
        replyMsg->pkt.msg.length = MSG_END_SIZE;
        msgsend(&Uart1TxMsg,uart1_send_char);
        wifiState = ONLINE;
    	break;

    	case WIFI_OFFLINE:							    //HiSi下传WiFi通讯中断,MCU应答
        wifiState = OFFLINE;
        replyMsg->pkt.msg.cmd = WIFI_OFFLINE;
        replyMsg->pkt.msg.length = MSG_END_SIZE;
        msgsend(&Uart1TxMsg,uart1_send_char);
        wifiState = OFFLINE;
        //清除APP控制
        actionState = ST_NO_ACTION;
        waitAction.replyFlag = FALSE;
    	break;

    	case VIDEO_CTRL:							    //HiSi控制视频状态
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
        //清除APP控制
        actionState = ST_NO_ACTION;
        waitAction.replyFlag = FALSE;
    	break;

    	case PHOTO_CTRL:							    //HiSi控制拍照
    	replyMsg->pkt.msg.cmd = PHOTO_CTRL;
        replyMsg->pkt.msg.para[0] = rxMsg->pkt.msg.para[0];
        replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
        if (rxMsg->pkt.msg.para[0] == 0x01) {
            if ((ST_WORK == instState)&&(actionState == ST_NO_ACTION)) {
                actionState = ST_CAPTURING;
                //等待关闭动作计时
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

    	case RECORD_CTRL:							    //HiSi控制录像状态
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

    	case PRINT_CTRL:							    //HiSi控制打印状态
    	replyMsg->pkt.msg.cmd = PRINT_CTRL;
        replyMsg->pkt.msg.para[0] = rxMsg->pkt.msg.para[0];
        replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
        if (rxMsg->pkt.msg.para[0] == 0x01) {
            if ((ST_WORK != instState)&&(ST_NO_ACTION == actionState)) {
                actionState = ST_PRINTING;
                //等待关闭动作计时
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
*  Description           :   处理接收到消息
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
*  Description           :   接收消息包正确处理
***************************************************************/
void Uart1RxMsgRightHandler(void)
{
    msg1dispatch(&Uart1RxMsg,&Uart1TxMsg);

}

/**************************************************************
*  Function Name         :   msgdispatch
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   处理解析消息错误并回复
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
*  Description           :   接收消息包错误处理
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
*  Description           :   处理2G模块的AT回复
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
*  Description           :   发送键值
***************************************************************/
void send_key_value(void)
{
    sendmsgpatch(SEND_KEY_VALUE, &Uart1TxMsg);      //打包键值
    msgsend(&Uart1TxMsg,uart1_send_char);           //发送已打包的数据
    waitReply.cmd = SEND_KEY_VALUE;                 //开启等待应答
    waitReply.timeCount = 0;
    waitReply.replyFlag = TRUE;
}

/**************************************************************
*  Function Name         :   void send_status()
*  Param                 :
*  Return Param          :   void
*  Description           :   发送当前状态
***************************************************************/
void send_status(void)
{
    sendmsgpatch(SEND_INST_STATUS, &Uart1TxMsg);    //打包键值
    msgsend(&Uart1TxMsg,uart1_send_char);           //发送已打包的数据
    waitReply.cmd = SEND_INST_STATUS;   //开启等待应答
    waitReply.timeCount = 0;
    waitReply.replyFlag = TRUE;
}

/**************************************************************
*  Function Name         :   sendmsgpatch
*  Param                 :   Msg_Cmd_E cmd, MsgTag* txdMsg
*  Return Param          :   void
*  Description           :   打包所需发送的消息包
***************************************************************/
void sendmsgpatch(Msg_Cmd_E cmd, MsgTag* txdMsg)
{
	txdMsg->pkt.msg.cmd = cmd;
	switch(cmd)
	{
        case SEND_KEY_VALUE:            //上传键值
        txdMsg->pkt.msg.para[0] = keyHandleInfor.keyCmd;
    	txdMsg->pkt.msg.length = 1 + MSG_END_SIZE;
		break;

		case GET_INST_INFOR:            //获取仪器信息
		txdMsg->pkt.msg.para[0] = instInforType;
    	txdMsg->pkt.msg.length = 1 + MSG_END_SIZE;
		break;

		case SEND_INST_STATUS:          //上传仪器状态
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
*  Description           :   仪器信息上传命令切换
***************************************************************/
void SendCmd_Switch(void)
{
	switch(cmddatauptype)
	{   /*
		case SEND_STATION_INFORMA:    //上传仪器信息
		if (instinfo <= IN_PCBA_VERSION) {
			instinfo++;
		} else {
			cmddatauptype = READ_CRUTUSER_INFO;
		}
		break;
		case SEND_STATION_STATUS:    //上传仪器状态

		break;
		case SEND_BATTERY_STATUS:    //上传电池状态

		break;
		case SEND_RECORD_DATA:	   //上传记录数据

		break;
		case SEND_RECORD_CURVE:    //上传记录曲线

		break;
		case SEND_SYNCHRON_DATA:    //上传同步的数据

		break;
		case SEND_ERROR_MESSAGE:    //上传错误信息

		break;
		case READ_REGUSER_INFO:    //读取注册用户信息

		break;
		case READ_CRUTUSER_INFO:    //读取当前用户信息
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
		case CLOCK_CALIBRATION:    //校准时钟
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
*  Description           :   上传cmd对应的数据信息的长度
***************************************************************/
void SendLen_CmdMsg(Msg_Cmd_E cmd)
{
	Set_DataLen_ToWire(Uart2TxMsg.pkt.msg.length + 3);
}

/**************************************************************
*  Function Name         :   void BleSend_CmdMsg(Msg_Cmd_E cmd)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   上传cmd对应的数据信息
***************************************************************/
void SendData_CmdMsg(void)
{
	msgsend(&Uart2TxMsg,uart2_send_char);//发送已打包的数据
}


/**************************************************************
*  Function Name         :   Uart2RxMsgRightHandler
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   接收消息包正确处理
***************************************************************/
void Uart2RxMsgRightHandler(void)
{
    msg1dispatch(&Uart1RxMsg,&Uart1TxMsg);
}


/**************************************************************
*  Function Name         :   msgdispatch
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   处理解析消息错误并回复
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
*  Description           :   接收消息包错误处理
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
*  Description           :   判断2G数据是否上传成功 0: 失败 1: 成功
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
			case SEND_SYNCHRON_DATA:							//上传记录时间
			case SEND_RECORD_DATA:				   				//上传记录数据
			case SEND_RECORD_CURVE:				   				//上传曲线数据
	        if(rxMsg->pkt.msg.para[0])
				reparm = 1;
			break;

			case READ_CRUTUSER_INFO:                             //读取app下传的使用用户信息
			if (Getcurt_Userinfo(rxMsg, txdMsg))
				reparm = 1;
			break;

			case CLOCK_CALIBRATION:                             //校准时钟
			Rtc_Read(&clocktime);                               //获取星期
	        memorycpy(&clocktime,&(rxMsg->pkt.msg.para[0]),DATALENGTH);
			set_time_data(&clocktime);
			//Times_Show(1, &(g_timeset.currentdata));
			reparm = 1;
			break;
*/
			default:                                            //命令信息校验错误
			break;
		}
	}
	return reparm;
}



/**************************************************************
*  Function Name         :   UINT8 Get_CmdMsg_Receive(Msg_Cmd_E cmd)
*  Param                 :   UINT8 data
*  Return Param          :   void
*  Description           :   获取2G回复的信息
***************************************************************/
UINT8 Get_CmdMsg_Receive(UINT8 data)
{
	UINT8 reparm = 0;

    msgunpack(&Uart2RxMsg,data);  	//解析app的回复包
    if(NS_MSG_ERR_NONE == Uart2RxMsg.msgerr) {   //解析包没有错误
       if(Uart2RxMsg.msgflag.bits.msgActive == ENABLE) //接收到完整的包
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


