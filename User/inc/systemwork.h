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
*  Description           :    ϵͳ���ƺ���
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

/*����״̬*/
typedef enum {
    ST_IDLE             = 0x01,            	/*�ر���Ƶ,��Ƭģʽ��Ҫ�ϴ���״̬*/
    ST_WORK             = 0x02,            	/*������Ƶ,��Ƶģʽ��Ҫ�ϴ���״̬*/
    ST_CHARGE			= 0x03,				/*���USB���������״̬*/
    ST_OFFLINE          = 0x04,            	/*���ߴ���״̬*/
    ST_STOP             = 0x05,            	/*˯��Stop״̬*/
    ST_WAKEUP           = 0x06,            	/*����״̬*/
    ST_CHARGING         = 0x07,            	/*�ػ�����״̬*/
}InstState;


/*����״̬*/
typedef enum {
    ST_NO_ACTION        = 0x00,				/*�޶���*/
    ST_CAPTURING        = 0x10,				/*ץ����Ƭ*/
    ST_RECORDING        = 0x20,				/*¼��*/
    ST_PRINTING         = 0x30,				/*��ӡ��Ƭ*/
    ST_GPRS_OPEN        = 0x40,				/*��GPRS*/
    ST_GPRS_COMM		= 0x50,				/*GPRS�ϴ�����*/
}ActionState;


/*����8����������ֵ*/
typedef enum {
    KEY_NULL            = 0x00,            	/*��*/
    KEY_MAIN            = 0x01,            	/*�����ܼ�*/
    KEY_ZOOM_OUT        = 0x02,            	/*�Ŵ��*/
    KEY_ZOOM_IN         = 0x03,            	/*��С��*/
    KEY_UP			    = 0x04,				/*�Ϸ�ҳ��*/
    KEY_DOWN			= 0x05,				/*�·�ҳ��*/
    KEY_LED             = 0x06,             /*LED����ڼ�*/
    KEY_PRINT           = 0x07,             /*��ӡ��*/
    KEY_VIDEO           = 0x08,             /*¼���*/
}KeyValue;


/*�û���������*/
typedef enum {
    NONE_KEY            = 0x00,             /*�ް���*/
    SIGNLE_KEY          = 0x01,             /*����*/
    DOUBLE_KEY          = 0x02,             /*˫��*/
    LONG_KEY            = 0x03,             /*����*/
}KeyType;


/*�ϴ���ֵ����*/
typedef enum {
    CMD_NULL            = 0x00,            	/*��*/
    CMD_VIDEO           = 0x01,            	/*�л�����Ƶ*/
    CMD_PHOTO           = 0x02,            	/*�л�����Ƭ*/
    CMD_ZOOM_OUT        = 0x03,            	/*�Ŵ���Ƶͼ��*/
    CMD_ZOOM_IN 	    = 0x04,				/*��С��Ƶͼ��*/
    CMD_START_RECORD	= 0x05,				/*��ʼ¼��*/
    CMD_STOP_RECORD     = 0x06,             /*ֹͣ¼��*/
    CMD_UP              = 0x07,             /*�Ϸ���Ƭ*/
    CMD_DOWN            = 0x08,             /*�·���Ƭ*/
    CMD_PRINT           = 0x09,             /*��ӡ��Ƭ*/
    CMD_CAPTURE         = 0x0A,             /*ץ����Ƭ*/
    CMD_PLAY            = 0x0B,             /*������Ƶ*/
    CMD_SHUT_DOWN       = 0x0C,             /*�ػ�*/
}KeyCmd;


/*�û���������*/
typedef struct {
    KeyValue    keyValue;                  /*�û����µİ���*/
    KeyType     keyType;                   /*�û���������*/
    KeyCmd      keyCmd;                    //��ֵ����
    INT16U      timeCount;                 /*ÿ�ΰ�����ʼ��ʱ*/
}KeyHandleInfor;


/*���״̬STA2STA1*/
typedef enum {
    CHARGE_DONE         = 0x01,             /*01:charge done*/
    CHARGE_IN           = 0x02,             /*10:charge in progress*/
    CHARGE_STOP         = 0x03,             /*11:no battery or off charge or sleep*/
}ChargeState;


/*���״̬*/
typedef struct {
    ChargeState    chargeState;            /*���״̬*/
    INT16U         voltage;                /*��ص�ѹLSB 305uV*/
    INT16U         remainTime;             /*ʣ��ʱ��--min*/
    INT8U          remainCharge;           /*ʣ�����--%*/
    INT8U          alert;                  /*����״̬*/
}CellState;

/*�쳣����*/
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


/*�Լ���Ϣ*/
typedef struct {
    INT16U         hisi3v2;
    INT16U         hisi1v8;
    INT16U         hisi1v1;
    INT16U         sensor3v2;
    INT16U         sensor1v8;
    INT8U          fault;                //�쳣��Ϣ��5λ�ֱ��ʾ������ѹ������
    ErrorCode      errorCode;
}SelfInfor;

//��Ƶ����LED״̬
typedef enum
{
	LIGHT_NULL 			= 0,
	LIGHT_WHITE		    = 1,            //�׹�
	LIGHT_BLUE		    = 2,            //����
}LightState;

//ϵͳ״̬LED��ʾ
typedef enum
{
	LED_OFF 			= 0,
	LED_ON			    = 1,
	LED_FLASH1000		= 2,            //1����˸
	LED_FLASH300		= 3,            //300������˸
	LED_FLASH100		= 4,            //100������˸
}LedType;

/*ϵͳ״̬LED��ʾ*/
typedef struct {
    LedType     ledType;
    LedType     state;                  //0--off,1--on
    INT16U      timeCount;              //led��ʱ����λ100mS
}StateLedDisp;


typedef enum
{
	DUCK_ON 			= 0,
	DUCK_OFF			= 1,
	DUCK_MOVED		    = 2,            //DUCK OFF to ON ������
	DUCK_REPLACED		= 3,            //DUCK ���˺󻹹������ˣ��㻻��һ��
}DuckState;

#define LED_RED                 0
#define LED_GREEN               1
#define OFFLINE                 0
#define ONLINE                  1

#define BATTERY_LOWEST       11147u                     //��عػ���ѹ3.4V��0.305mV/LSB
#define BATTERY_LOW          11803u                     //����˳������ѹ3.60V
#define BATTERY_LOW_HOLD     11311u                     //��ؾ����ѹ3.45V
#define BATTERY_FULL		 13606u                     //���������ѹ4.15v
#define BATTERY_FULL_HOLD	 13442u                     //���������ѹ4.10V

#define PRE_CHARGE_V1        8196                       //2.5V  Ԥ��59��
#define PRE_CHARGE_V2        8525                       //2.6V  Ԥ��30��
#define PRE_CHARGE_V3        8852                       //2.7V  Ԥ��20��
#define PRE_CHARGE_V4        9180                       //2.8V  Ԥ��10��
#define PRE_CHARGE_V5        9836                       //3.0V  Ԥ��5��

SYSTEMWORK_EXT  InstState           instState; 			//����״̬
SYSTEMWORK_EXT  ActionState         actionState; 	    //����״̬
SYSTEMWORK_EXT  INT8U               wifiState; 	        //WiFi����״̬
SYSTEMWORK_EXT  KeyHandleInfor      keyHandleInfor; 	//������Ϣ
SYSTEMWORK_EXT  CellState           cellState;		    //���״̬
SYSTEMWORK_EXT  SelfInfor           selfInfor;		    //�Լ���Ϣ
SYSTEMWORK_EXT  DuckState           duckState;		    //Ѽ����״̬
SYSTEMWORK_EXT  INT32U              duckCnt;            //Ѽ��������
SYSTEMWORK_EXT  LightState          lightState;         //����LED״̬
SYSTEMWORK_EXT  StateLedDisp        redLedDisp,greenLedDisp; //״̬LED��ʾ����


#ifdef SYSTEMWORK_GLOBAL
SYSTEMWORK_EXT  INT8U               chargeFlag = 0;		//����־
SYSTEMWORK_EXT  INT16U              ledPulse = 12;		//LED���ȷ�Χ5~16
SYSTEMWORK_EXT  INT8U               stopFlag = 0;		//����stopģʽ��־
SYSTEMWORK_EXT  INT8U               wakeupFlag = 0;		//���ѱ�־

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
