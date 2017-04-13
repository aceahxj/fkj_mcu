/***************************************************************
*  FileName              :    interrupt.h
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    中断函数
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#ifdef  INTERRUPT_GLOBAL
#define INTERRUPT_EXT
#else
#define INTERRUPT_EXT   extern
#endif


#include "lib_type.h"
#include "uart.h"

#ifdef  INTERRUPT_GLOBAL
volatile INTERRUPT_EXT UINT8 timeset_complete = 0;	/*时间设置完成标志*/
volatile INTERRUPT_EXT UINT8 datasend_topc = 0;		/*长按select发送数据到pc*/
volatile INTERRUPT_EXT UINT8  secondsflag = 0;       /*秒时钟标志*/
volatile INTERRUPT_EXT UINT8  timeblinkflag = 0;		/*设置日期/时间提醒标志*/
volatile INTERRUPT_EXT UINT8 g_secondcut = 0;		/*Tim14秒中断计数*/
volatile INTERRUPT_EXT UINT8 g_datafresh = 0;		/*时间刷新标志位*/
volatile INTERRUPT_EXT UINT8 g_wirelessfresh = 0;		/*2g命令刷新标志位*/
volatile INTERRUPT_EXT UINT8 longkeywakeup  = 0;		/*待机下长按唤醒仪器*/
volatile INTERRUPT_EXT UINT8 secondcnt = 0;
//volatile INTERRUPT_EXT UINT8	statictime_showflag = 0;/*显示静态时间标志*/
volatile INTERRUPT_EXT UINT8 keymainsignal = 0;
volatile INTERRUPT_EXT UINT8 chagreflash_cnt = 0;
volatile INTERRUPT_EXT UINT8 chagreflash_flag = 0;
volatile INTERRUPT_EXT UINT8 batterytest = 0;

#else
volatile INTERRUPT_EXT UINT8 timeset_complete;
volatile INTERRUPT_EXT UINT8 datasend_topc;
volatile INTERRUPT_EXT UINT8  secondsflag;
volatile INTERRUPT_EXT UINT8  timeblinkflag;
volatile INTERRUPT_EXT UINT8 g_secondcut;
volatile INTERRUPT_EXT UINT8 g_datafresh;
volatile INTERRUPT_EXT UINT8 g_wirelessfresh;		/*2g命令刷新标志位*/
volatile INTERRUPT_EXT UINT8 longkeywakeup;
volatile INTERRUPT_EXT UINT8 secondcnt;
//volatile INTERRUPT_EXT UINT8	statictime_showflag;
volatile INTERRUPT_EXT UINT8 keymainsignal;
volatile INTERRUPT_EXT UINT8 chagreflash_cnt;
volatile INTERRUPT_EXT UINT8 chagreflash_flag;
volatile INTERRUPT_EXT UINT8 batterytest;

#endif




#endif
