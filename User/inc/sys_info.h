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
*  Description           :    ������Ϣ
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


#define     DFUT_FIRMWARE_VERSION    "V1.0.1"			                /* Firmware�汾 */



#define FACTORY_NAME_LEN        23
#define INST_NAME_LEN           23
#define INST_TYPE_LEN           23
#define INST_SN_LEN             23
#define PRODUCT_DATE_LEN        11
#define FIRMWARE_VERSION_LEN    7
#define PCB_VERSION_LEN         7
#define PCBA_VERSION_LEN        7


/*������Ϣ�ܳ���*/
#define SYS_INFO_LENGTH     (FACTORY_NAME_LEN + INST_NAME_LEN \
	                         + INST_TYPE_LEN + INST_SN_LEN      \
                             + PRODUCT_DATE_LEN +  \
                             + PCB_VERSION_LEN + PCBA_VERSION_LEN + 7)

#define DATALENGTH             6                         /*�����ֽ���*/

//����Ĭ����Ϣ
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


//������Ϣ
typedef enum
{
	IN_FACTORY_NAME 			    =	0x00,		/* �������� */
	IN_INST_NAME					=	0x01,		/* �������� */
	IN_INST_TYPE					=	0x02,		/* �����ͺ� */
	IN_INST_SN						=	0x03,		/* �������к� */
	IN_PRODUCT_DATE 				=	0x04,		/* ������������ */
	IN_FIRMWARE_VERSION 			=	0x05,		/* Firmware�汾 */
	IN_PCB_VERSION				    =   0x06, 		/* PCB�汾 */
	IN_PCBA_VERSION 				=	0x07,		/* PCBA�汾 */
}InstInforType;



SYS_INFO_EXT  InstInfor  instInfor;				            //����������Ϣ
SYS_INFO_EXT  InstInforType  instInforType;				    //����������Ϣ����


void Erase_FlashPage(INT32U addr);
void sendstationinfo(INT8U instinfo,MsgTag *msgtag);        //��ȡ������Ϣ
void sendbatterystus(MsgTag *msgtag);                       //��ȡ���״̬
void readcurrentmonitor(MsgTag *msgtag);                   //��ȡ������
void writeatomizeinfo(void);
void readatomizeinfo(UINT8 num,MsgTag * msgtag);           //��ȡ����Ϣ
void readatomizestatus(MsgTag * msgtag);                   //��ȡ����״̬
UINT16 calculate_chksum(UINT8* data,UINT32 len);
void setreguserinfo(MsgTag* rxMsg, MsgTag* replyMsg);      //����ע���û���Ϣ
void setcurtuserinfo(MsgTag* rxMsg, MsgTag* replyMsg);
void appreadfactorypara(MsgTag * msgtag);
void stateinfoprogram(MsgTag* rxMsg, MsgTag* replyMsg);     //��д������Ϣ��Ĭ�ϲ���
void SetBtps_ModeTemp(MsgTag* rxMsg, MsgTag* replyMsg);
void  Read_KNfactor(MsgTag* rxMsg, MsgTag* replyMsg);
void  Read_BtpsTemp(MsgTag* rxMsg, MsgTag* replyMsg);
void  SetCalibrate_Type(MsgTag* rxMsg, MsgTag* replyMsg);
void readfactorypara(void);
void resetblepara(void);
UINT8 Getcurt_Userinfo(MsgTag * rxMsg,MsgTag * replyMsg);
void sendtime_datarecord(UINT32 dataaddr, MsgTag *msgtag);
#endif

