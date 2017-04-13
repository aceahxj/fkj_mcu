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
*  Description           :    通信消息解析
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

/*********************消息格式*********************************
*  ------    ------ ------  ----------  -------- ------
*   帧   头 | 长度 | 命令 |   消息参数 | 校验和 | 帧尾 |
*  ----------------------------------------------------
*  0x55|0xAA| 1byte| 1byte| 0~100 bytes| 2bytes | 0x0D |
*  ----------------------------------------------------
***************************************************************/
//命令中数据都为16进制
//命令中多字节数据统一采用低字节在前，高字节在后的形式
//命令长度=命令字+参数+校验和+帧尾的字节数，不包括帧头和长度
//消息参数可以多个字节，也可以为空
//校验和=帧头+长度+命令+消息参数的字节累加和

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
//包解析错误代码
typedef enum
{
    NS_MSG_ERR_NONE       = 0x00u,    //无错误
    NS_MSG_ERR_STX        = 0x01u,    //消息帧头错误
    NS_MSG_ERR_LEN        = 0x02u,    //消息长度错误
    NS_MSG_ERR_PARA       = 0x03u,    //消息参数错误
    NS_MSG_ERR_CHKSUM     = 0x04u,    //校验和错误
    NS_MSG_ERR_END        = 0x05u,    //消息帧尾错误
    NS_MSG_ERR_FRAGMENT   = 0x06u,    //消息不完整错误
    NS_MSG_ERR_UNDEF      = 0x07u,    //消息未定义错误
}NS_ERR;


//蓝牙连接状态
typedef enum
{
   BLE_DISCONNECT         = 0x00,       //BLE断开连接
   BLE_CONNECT            = 0x01,       //BLE成功连接
}BLE_STATUS;


//通信消息命令
typedef enum
{
    //与主机通讯
    SEND_KEY_VALUE          = 0xB1,    //MCU上传键盘
    GET_INST_INFOR          = 0xB2,    //MCU获取仪器信息
    SEND_INST_STATUS        = 0xB3,    //MCU上传仪器状态
    GET_CAL_TIME            = 0xB4,    //HiSi下传校准后的时间
    WIFI_OFFLINE            = 0xB5,    //HiSi下传WiFi通讯中断
    VIDEO_CTRL              = 0xB6,    //HiSi控制视频状态
    PHOTO_CTRL              = 0xB7,    //HiSi控制拍照
    RECORD_CTRL             = 0xB8,    //HiSi控制录像状态
    PRINT_CTRL              = 0xB9,    //HiSi控制打印状态
    //与2G模块通讯
    SEND_INST_LOG           = 0xC1,    //MCU上传仪器日志到服务器，以后考虑加密
    WAIT_NULL               = 0x00,
    ERROR_ACK               = 0x0A,    //MCU错误应答
}Msg_Cmd_E;


//等待应答消息
typedef struct {
    Msg_Cmd_E   cmd;                     //等待应答的命令字
    INT16U      replyFlag;               //等待标志
    INT16U      timeCount;               //等待计时
}WaitReply;


//原始消息数据
typedef struct
{
    INT16U  length;                    //消息长度
    Msg_Cmd_E  cmd;                    //命令
    INT8U  *para;                      //消息参数
}MSG;

//消息解析控制结构
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
            INT8U stxActive     :1;     //消息头接收正确标志
            INT8U msgActive     :1;     //消息接收完成标志
            INT8U bleActive     :1;     //蓝牙消息标志
        }bits;
        INT8U ubyte;
    }msgflag;

    INT8U   predata;                    //前一个字节数据
    INT32U  msgptr;                     //消息计数
    NS_ERR  msgerr;                     //消息错误代码

    void (*RightHandler)(void);         //接收正确处理函数指针
    void (*ErrorHandler)(INT8U err);    //接收错误处理函数指针

}MsgTag;

//定义接收消息包结构体和缓存，并初始化
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

//定义发送消息包结构体和缓存，并初始化
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
MSG_EXT BLE_STATUS blestatus = BLE_DISCONNECT;                               //蓝牙连接状态
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
#define  MSG_STXF  0x55                 //帧头字节1
#define  MSG_STXS  0xAA				    //帧头字节2
#define  MSG_END   0x0D				    //帧尾字节

#define  MSG_RX_MAX_LENGTH      128     //消息长度最大字节数

#define  MSG_LENGTH_SIZE        2		//消息长度字节数
#define  MSG_STX_SIZE			3		//消息长度+命令字节数
#define  MSG_END_SIZE           4		//命令+校验和+帧尾
#define  MSG_HEAD_SIZE          4		//帧头+消息长度


#define		BLE_WAKE_A			0x41
#define		BLE_WAKE_d			0x64
#define  BLE_CONT_RXLEN         14       //蓝牙使能成功回复的字符串长度"Advertising=OK\r\n"



/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void msgprepare(MsgTag* msgtag);                                          //接收包前初始化
void msgpack(MsgTag* msgtag,INT8U length,INT8U cmd,INT8U* para);          //打包数据
void msgunpack(MsgTag *msgtag,INT8U rxdata);                              //消息解析函数
INT16U msgcalculatesum(MsgTag *msgtag);                                   //计算消息校验和函数
void msgunpackcheck(MsgTag *rxmsgtag);                                    //检查是否解析成功
void msgsend(MsgTag *msgtsg,void(*txdata)(INT8U data));                   //发送消息包


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif

