/***************************************************************
*  FileName              :    sys_info.h
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :

*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    仪器信息
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#ifndef _SYS_INFO_H_
#define _SYS_INFO_H_

#ifdef  SYS_INFO_GLOBAL
#define SYS_INFO_EXT
#else
#define SYS_INFO_EXT extern
#endif

#include "msg.h"
#include "stm32f0xx.h"


#define     DFUT_FIRMWARE_VERSION    "V1.0.1"			                /* Firmware版本 */



#define FACTORY_NAME_LEN        23
#define INST_NAME_LEN           23
#define INST_TYPE_LEN           23
#define INST_SN_LEN             23
#define PRODUCT_DATE_LEN        11
#define FIRMWARE_VERSION_LEN    7
#define PCB_VERSION_LEN         7
#define PCBA_VERSION_LEN        7


/*仪器信息总长度*/
#define SYS_INFO_LENGTH     (FACTORY_NAME_LEN + INST_NAME_LEN \
	                         + INST_TYPE_LEN + INST_SN_LEN      \
                             + PRODUCT_DATE_LEN +  \
                             + PCB_VERSION_LEN + PCBA_VERSION_LEN + 7)

#define DATALENGTH             6                         /*日期字节数*/

//仪器默认信息
typedef struct
{
   char FACTORY_NAME[FACTORY_NAME_LEN + 1];
   char INST_NAME[INST_NAME_LEN + 1];
   char INST_TYPE[INST_TYPE_LEN + 1];
   char INST_SN[INST_SN_LEN + 1];
   char PRODUCT_DATE[PRODUCT_DATE_LEN + 1];
   char FIRMWARE_VERSION[FIRMWARE_VERSION_LEN + 1];
   char PCB_VERSION[PCB_VERSION_LEN + 1];
   char PCBA_VERSION[PCBA_VERSION_LEN + 1];
}InstInfor;


//仪器信息
typedef enum
{
	IN_FACTORY_NAME 			    =	0x00,		/* 厂家名称 */
	IN_INST_NAME					=	0x01,		/* 仪器名称 */
	IN_INST_TYPE					=	0x02,		/* 仪器型号 */
	IN_INST_SN						=	0x03,		/* 仪器序列号 */
	IN_PRODUCT_DATE 				=	0x04,		/* 仪器生产日期 */
	IN_FIRMWARE_VERSION 			=	0x05,		/* Firmware版本 */
	IN_PCB_VERSION				    =   0x06, 		/* PCB版本 */
	IN_PCBA_VERSION 				=	0x07,		/* PCBA版本 */
}InstInforType;



SYS_INFO_EXT  InstInfor  instInfor;				            //保存仪器信息
SYS_INFO_EXT  InstInforType  instInforType;				    //保存仪器信息类型


void Erase_FlashPage(INT32U addr);
void sendstationinfo(INT8U instinfo,MsgTag *msgtag);        //读取仪器信息
void sendbatterystus(MsgTag *msgtag);                       //读取电池状态
void readcurrentmonitor(MsgTag *msgtag);                   //读取检测电流
void writeatomizeinfo(void);
void readatomizeinfo(UINT8 num,MsgTag * msgtag);           //读取雾化信息
void readatomizestatus(MsgTag * msgtag);                   //读取雾化杯状态
UINT16 calculate_chksum(UINT8* data,UINT32 len);
void setreguserinfo(MsgTag* rxMsg, MsgTag* replyMsg);      //设置注册用户信息
void setcurtuserinfo(MsgTag* rxMsg, MsgTag* replyMsg);
void appreadfactorypara(MsgTag * msgtag);
void stateinfoprogram(MsgTag* rxMsg, MsgTag* replyMsg);     //烧写仪器信息和默认参数
void SetBtps_ModeTemp(MsgTag* rxMsg, MsgTag* replyMsg);
void  Read_KNfactor(MsgTag* rxMsg, MsgTag* replyMsg);
void  Read_BtpsTemp(MsgTag* rxMsg, MsgTag* replyMsg);
void  SetCalibrate_Type(MsgTag* rxMsg, MsgTag* replyMsg);
void readfactorypara(void);
void resetblepara(void);
UINT8 Getcurt_Userinfo(MsgTag * rxMsg,MsgTag * replyMsg);
void sendtime_datarecord(UINT32 dataaddr, MsgTag *msgtag);
#endif

