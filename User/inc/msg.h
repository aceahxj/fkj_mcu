/***************************************************************
*  FileName              :    msg.h
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

/*********************��Ϣ��ʽ*********************************
*  ------    ------ ------  ----------  -------- ------
*   ֡   ͷ | ���� | ���� |   ��Ϣ���� | У��� | ֡β |
*  ----------------------------------------------------
*  0x55|0xAA| 1byte| 1byte| 0~100 bytes| 2bytes | 0x0D |
*  ----------------------------------------------------
***************************************************************/
//���������ݶ�Ϊ16����
//�����ж��ֽ�����ͳһ���õ��ֽ���ǰ�����ֽ��ں����ʽ
//�����=������+����+У���+֡β���ֽ�����������֡ͷ�ͳ���
//��Ϣ�������Զ���ֽڣ�Ҳ����Ϊ��
//У���=֡ͷ+����+����+��Ϣ�������ֽ��ۼӺ�

#ifndef  _MSG_H_
#define  _MSG_H_

#ifdef  MSG_GLOBAL
#define MSG_EXT
#else
#define MSG_EXT     extern

#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "lib_type.h"
#include "stm32f0xx.h"
#include "uart.h"

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
//�������������
typedef enum
{
    NS_MSG_ERR_NONE       = 0x00u,    //�޴���
    NS_MSG_ERR_STX        = 0x01u,    //��Ϣ֡ͷ����
    NS_MSG_ERR_LEN        = 0x02u,    //��Ϣ���ȴ���
    NS_MSG_ERR_PARA       = 0x03u,    //��Ϣ��������
    NS_MSG_ERR_CHKSUM     = 0x04u,    //У��ʹ���
    NS_MSG_ERR_END        = 0x05u,    //��Ϣ֡β����
    NS_MSG_ERR_FRAGMENT   = 0x06u,    //��Ϣ����������
    NS_MSG_ERR_UNDEF      = 0x07u,    //��Ϣδ�������
}NS_ERR;


//��������״̬
typedef enum
{
   BLE_DISCONNECT         = 0x00,       //BLE�Ͽ�����
   BLE_CONNECT            = 0x01,       //BLE�ɹ�����
}BLE_STATUS;


//ͨ����Ϣ����
typedef enum
{
    //������ͨѶ
    SEND_KEY_VALUE          = 0xB1,    //MCU�ϴ�����
    GET_INST_INFOR          = 0xB2,    //MCU��ȡ������Ϣ
    SEND_INST_STATUS        = 0xB3,    //MCU�ϴ�����״̬
    GET_CAL_TIME            = 0xB4,    //HiSi�´�У׼���ʱ��
    WIFI_OFFLINE            = 0xB5,    //HiSi�´�WiFiͨѶ�ж�
    VIDEO_CTRL              = 0xB6,    //HiSi������Ƶ״̬
    PHOTO_CTRL              = 0xB7,    //HiSi��������
    RECORD_CTRL             = 0xB8,    //HiSi����¼��״̬
    PRINT_CTRL              = 0xB9,    //HiSi���ƴ�ӡ״̬
    //��2Gģ��ͨѶ
    SEND_INST_LOG           = 0xC1,    //MCU�ϴ�������־�����������Ժ��Ǽ���
    WAIT_NULL               = 0x00,
    ERROR_ACK               = 0x0A,    //MCU����Ӧ��
}Msg_Cmd_E;


//�ȴ�Ӧ����Ϣ
typedef struct {
    Msg_Cmd_E   cmd;                     //�ȴ�Ӧ���������
    INT16U      replyFlag;               //�ȴ���־
    INT16U      timeCount;               //�ȴ���ʱ
}WaitReply;


//ԭʼ��Ϣ����
typedef struct
{
    INT16U  length;                    //��Ϣ����
    Msg_Cmd_E  cmd;                    //����
    INT8U  *para;                      //��Ϣ����
}MSG;

//��Ϣ�������ƽṹ
typedef struct
{
    union
    {
        MSG    msg;
        INT8U  part[sizeof(MSG)];
    }pkt;

    union
    {
        struct
        {
            INT8U stxActive     :1;     //��Ϣͷ������ȷ��־
            INT8U msgActive     :1;     //��Ϣ������ɱ�־
            INT8U bleActive     :1;     //������Ϣ��־
        }bits;
        INT8U ubyte;
    }msgflag;

    INT8U   predata;                    //ǰһ���ֽ�����
    INT32U  msgptr;                     //��Ϣ����
    NS_ERR  msgerr;                     //��Ϣ�������

    void (*RightHandler)(void);         //������ȷ������ָ��
    void (*ErrorHandler)(INT8U err);    //���մ�������ָ��

}MsgTag;

//���������Ϣ���ṹ��ͻ��棬����ʼ��
#define DEFINE_RX_MSG(MSG_NAME,MSG_RX_LENGTH)\
	INT8U MSG_NAME##Buf[MSG_RX_LENGTH];\
	void MSG_NAME##RightHandler(void);\
	void MSG_NAME##ErrorHandler(INT8U err);\
	MsgTag MSG_NAME =     \
    {                     \
       .msgerr = NS_MSG_ERR_NONE,\
       .msgptr = 0,       \
       .predata = 0,   \
       .RightHandler = MSG_NAME##RightHandler,\
       .ErrorHandler = MSG_NAME##ErrorHandler,\
       .pkt = {           \
            .msg = {      \
                 .para = MSG_NAME##Buf\
                   }      \
              },          \
       .msgflag = {       \
                .ubyte = 0\
                  }       \
    };

//���巢����Ϣ���ṹ��ͻ��棬����ʼ��
#define DEFINE_TX_MSG(MSG_NAME,MSG_TX_LENGTH)\
	INT8U MSG_NAME##Buf[MSG_TX_LENGTH];\
	MsgTag MSG_NAME =  \
    {                  \
         .msgerr = NS_MSG_ERR_NONE,\
         .pkt = {      \
              .msg = { \
                  .para = MSG_NAME##Buf\
                     } \
                }      \
    };

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/
MSG_EXT WaitReply   waitReply;
MSG_EXT WaitReply   waitAction;

#ifdef  MSG_GLOBAL
MSG_EXT BLE_STATUS blestatus = BLE_DISCONNECT;                               //��������״̬
MSG_EXT Msg_Cmd_E  cmddatauptype = SEND_INST_LOG;
#else
MSG_EXT BLE_STATUS blestatus;
MSG_EXT Msg_Cmd_E  cmddatauptype;
#endif

/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/
#define  MSG_STXF  0x55                 //֡ͷ�ֽ�1
#define  MSG_STXS  0xAA				    //֡ͷ�ֽ�2
#define  MSG_END   0x0D				    //֡β�ֽ�

#define  MSG_RX_MAX_LENGTH      128     //��Ϣ��������ֽ���

#define  MSG_LENGTH_SIZE        2		//��Ϣ�����ֽ���
#define  MSG_STX_SIZE			3		//��Ϣ����+�����ֽ���
#define  MSG_END_SIZE           4		//����+У���+֡β
#define  MSG_HEAD_SIZE          4		//֡ͷ+��Ϣ����


#define		BLE_WAKE_A			0x41
#define		BLE_WAKE_d			0x64
#define  BLE_CONT_RXLEN         14       //����ʹ�ܳɹ��ظ����ַ�������"Advertising=OK\r\n"



/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void msgprepare(MsgTag* msgtag);                                          //���հ�ǰ��ʼ��
void msgpack(MsgTag* msgtag,INT8U length,INT8U cmd,INT8U* para);          //�������
void msgunpack(MsgTag *msgtag,INT8U rxdata);                              //��Ϣ��������
INT16U msgcalculatesum(MsgTag *msgtag);                                   //������ϢУ��ͺ���
void msgunpackcheck(MsgTag *rxmsgtag);                                    //����Ƿ�����ɹ�
void msgsend(MsgTag *msgtsg,void(*txdata)(INT8U data));                   //������Ϣ��


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif

