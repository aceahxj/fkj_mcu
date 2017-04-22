/***************************************************************
*  FileName              :    systemwork.h
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    系统控制函数
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#ifndef _SYSTEMWORK_H
#define _SYSTEMWORK_H

#ifdef SYSTEMWORK_GLOBAL
#define SYSTEMWORK_EXT
#else
#define SYSTEMWORK_EXT extern
#endif

#include "lib_type.h"

//#define DEBUG

/*仪器状态*/
typedef enum {
    ST_IDLE             = 0x01,            	/*关闭视频,照片模式，要上传的状态*/
    ST_WORK             = 0x02,            	/*开启视频,视频模式，要上传的状态*/
    ST_CHARGE			= 0x03,				/*充电USB插入充电待机状态*/
    ST_OFFLINE          = 0x04,            	/*离线待机状态*/
    ST_STOP             = 0x05,            	/*睡眠Stop状态*/
    ST_WAKEUP           = 0x06,            	/*唤醒状态*/
    ST_CHARGING         = 0x07,            	/*关机后充电状态*/
}InstState;


/*动作状态*/
typedef enum {
    ST_NO_ACTION        = 0x00,				/*无动作*/
    ST_CAPTURING        = 0x10,				/*抓拍照片*/
    ST_RECORDING        = 0x20,				/*录像*/
    ST_PRINTING         = 0x30,				/*打印照片*/
    ST_GPRS_OPEN        = 0x40,				/*打开GPRS*/
    ST_GPRS_COMM		= 0x50,				/*GPRS上传数据*/
}ActionState;


/*仪器8个按键，键值*/
typedef enum {
    KEY_NULL            = 0x00,            	/*空*/
    KEY_MAIN            = 0x01,            	/*主功能键*/
    KEY_ZOOM_OUT        = 0x02,            	/*放大键*/
    KEY_ZOOM_IN         = 0x03,            	/*缩小键*/
    KEY_UP			    = 0x04,				/*上翻页键*/
    KEY_DOWN			= 0x05,				/*下翻页键*/
    KEY_LED             = 0x06,             /*LED光调节键*/
    KEY_PRINT           = 0x07,             /*打印键*/
    KEY_VIDEO           = 0x08,             /*录像键*/
}KeyValue;


/*用户按键类型*/
typedef enum {
    NONE_KEY            = 0x00,             /*无按键*/
    SIGNLE_KEY          = 0x01,             /*单击*/
    DOUBLE_KEY          = 0x02,             /*双击*/
    LONG_KEY            = 0x03,             /*长按*/
}KeyType;


/*上传键值命令*/
typedef enum {
    CMD_NULL            = 0x00,            	/*空*/
    CMD_VIDEO           = 0x01,            	/*切换到视频*/
    CMD_PHOTO           = 0x02,            	/*切换到照片*/
    CMD_ZOOM_OUT        = 0x03,            	/*放大视频图像*/
    CMD_ZOOM_IN 	    = 0x04,				/*缩小视频图像*/
    CMD_START_RECORD	= 0x05,				/*开始录像*/
    CMD_STOP_RECORD     = 0x06,             /*停止录像*/
    CMD_UP              = 0x07,             /*上翻照片*/
    CMD_DOWN            = 0x08,             /*下翻照片*/
    CMD_PRINT           = 0x09,             /*打印照片*/
    CMD_CAPTURE         = 0x0A,             /*抓拍照片*/
    CMD_PLAY            = 0x0B,             /*播放视频*/
    CMD_SHUT_DOWN       = 0x0C,             /*关机*/
}KeyCmd;


/*用户按键操作*/
typedef struct {
    KeyValue    keyValue;                  /*用户按下的按键*/
    KeyType     keyType;                   /*用户按键类型*/
    KeyCmd      keyCmd;                    //键值命令
    INT16U      timeCount;                 /*每次按键后开始计时*/
}KeyHandleInfor;


/*充电状态STA2STA1*/
typedef enum {
    CHARGE_DONE         = 0x01,             /*01:charge done*/
    CHARGE_IN           = 0x02,             /*10:charge in progress*/
    CHARGE_STOP         = 0x03,             /*11:no battery or off charge or sleep*/
}ChargeState;


/*电池状态*/
typedef struct {
    ChargeState    chargeState;            /*充电状态*/
    INT16U         voltage;                /*电池电压LSB 305uV*/
    INT16U         remainTime;             /*剩余时间--min*/
    INT8U          remainCharge;           /*剩余电量--%*/
    INT8U          alert;                  /*报警状态*/
}CellState;

/*异常代码*/
typedef enum {
    HISI3V2_ERR         = 0xE0,
    HISI1V8_ERR         = 0xE1,
    HISI1V1_ERR         = 0xE2,
    SENSOR3V2_ERR       = 0xE3,
    SENSOR1V8__ERR      = 0xE4,
    CHARGE_ERR          = 0xE5,
    GPRS_REPLY_ERR      = 0xE6,
    GPRS_NET_ERR        = 0xE7,
    GPRS_SIM_ERR        = 0xE8,
    GPRS_SERVER_ERR     = 0xE9,
    HISI_COMM_ERR       = 0xEA,
}ErrorCode;


/*自检信息*/
typedef struct {
    INT16U         hisi3v2;
    INT16U         hisi1v8;
    INT16U         hisi1v1;
    INT16U         sensor3v2;
    INT16U         sensor1v8;
    INT8U          fault;                //异常信息，5位分别表示各个电压不正常
    ErrorCode      errorCode;
}SelfInfor;

//视频光照LED状态
typedef enum
{
	LIGHT_NULL 			= 0,
	LIGHT_WHITE		    = 1,            //白光
	LIGHT_BLUE		    = 2,            //蓝光
}LightState;

//系统状态LED显示
typedef enum
{
	LED_OFF 			= 0,
	LED_ON			    = 1,
	LED_FLASH1000		= 2,            //1秒闪烁
	LED_FLASH300		= 3,            //300毫秒闪烁
	LED_FLASH100		= 4,            //100毫秒闪烁
}LedType;

/*系统状态LED显示*/
typedef struct {
    LedType     ledType;
    LedType     state;                  //0--off,1--on
    INT16U      timeCount;              //led计时，单位100mS
}StateLedDisp;


typedef enum
{
	DUCK_ON 			= 0,
	DUCK_OFF			= 1,
	DUCK_MOVED		    = 2,            //DUCK OFF to ON 动过了
	DUCK_REPLACED		= 3,            //DUCK 动了后还工作过了，算换了一个
}DuckState;

#define LED_RED                 0
#define LED_GREEN               1
#define OFFLINE                 0
#define ONLINE                  1

#define BATTERY_LOWEST       11147u                     //电池关机电压3.4V，0.305mV/LSB
#define BATTERY_LOW          11803u                     //电池退出警告电压3.60V
#define BATTERY_LOW_HOLD     11311u                     //电池警告电压3.45V
#define BATTERY_FULL		 13606u                     //电池正常电压4.15v
#define BATTERY_FULL_HOLD	 13442u                     //电池正常电压4.10V

#define PRE_CHARGE_V1        8196                       //2.5V  预充59秒
#define PRE_CHARGE_V2        8525                       //2.6V  预充30秒
#define PRE_CHARGE_V3        8852                       //2.7V  预充20秒
#define PRE_CHARGE_V4        9180                       //2.8V  预充10秒
#define PRE_CHARGE_V5        9836                       //3.0V  预充5秒

SYSTEMWORK_EXT  InstState           instState; 			//仪器状态
SYSTEMWORK_EXT  ActionState         actionState; 	    //动作状态
SYSTEMWORK_EXT  INT8U               wifiState; 	        //WiFi连接状态
SYSTEMWORK_EXT  KeyHandleInfor      keyHandleInfor; 	//按键信息
SYSTEMWORK_EXT  CellState           cellState;		    //电池状态
SYSTEMWORK_EXT  SelfInfor           selfInfor;		    //自检信息
SYSTEMWORK_EXT  DuckState           duckState;		    //鸭嘴器状态
SYSTEMWORK_EXT  INT32U              duckCnt;            //鸭嘴器计数
SYSTEMWORK_EXT  LightState          lightState;         //光照LED状态
SYSTEMWORK_EXT  StateLedDisp        redLedDisp,greenLedDisp; //状态LED显示控制


#ifdef SYSTEMWORK_GLOBAL
SYSTEMWORK_EXT  INT8U               chargeFlag = 0;		//充电标志
SYSTEMWORK_EXT  INT16U              ledPulse = 12;		//LED亮度范围5~16
SYSTEMWORK_EXT  INT8U               stopFlag = 0;		//进入stop模式标志
SYSTEMWORK_EXT  INT8U               wakeupFlag = 0;		//唤醒标志

#else
SYSTEMWORK_EXT  INT8U               chargeFlag;
SYSTEMWORK_EXT  INT8U               batteryCapcity;
SYSTEMWORK_EXT  INT16U              ledPulse;
SYSTEMWORK_EXT  INT8U               stopFlag;
SYSTEMWORK_EXT  INT8U               wakeupFlag;
#endif

void Time_Configer(void);
void Charge_Test(void);
void reset_system(void);
void key_open_system(void);
void pre_charge_init(void);
void charge_vol(void);
void system_task(void);
static void key_task(void);
static void work_task(void);
void get_cell_state(void);
void led_manage(void);
void power_monitor(void);
void duck_detect(void);
void led_show(INT8U ledId, LedType ledType);
void led_off(void);
void sys_enterstop(void);
void reset_charge(void);

#endif
