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
*  Description           :    �жϺ���
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
volatile INTERRUPT_EXT UINT8 timeset_complete = 0;	/*ʱ��������ɱ�־*/
volatile INTERRUPT_EXT UINT8 datasend_topc = 0;		/*����select�������ݵ�pc*/
volatile INTERRUPT_EXT UINT8  secondsflag = 0;       /*��ʱ�ӱ�־*/
volatile INTERRUPT_EXT UINT8  timeblinkflag = 0;		/*��������/ʱ�����ѱ�־*/
volatile INTERRUPT_EXT UINT8 g_secondcut = 0;		/*Tim14���жϼ���*/
volatile INTERRUPT_EXT UINT8 g_datafresh = 0;		/*ʱ��ˢ�±�־λ*/
volatile INTERRUPT_EXT UINT8 g_wirelessfresh = 0;		/*2g����ˢ�±�־λ*/
volatile INTERRUPT_EXT UINT8 longkeywakeup  = 0;		/*�����³�����������*/
volatile INTERRUPT_EXT UINT8 secondcnt = 0;
//volatile INTERRUPT_EXT UINT8	statictime_showflag = 0;/*��ʾ��̬ʱ���־*/
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
volatile INTERRUPT_EXT UINT8 g_wirelessfresh;		/*2g����ˢ�±�־λ*/
volatile INTERRUPT_EXT UINT8 longkeywakeup;
volatile INTERRUPT_EXT UINT8 secondcnt;
//volatile INTERRUPT_EXT UINT8	statictime_showflag;
volatile INTERRUPT_EXT UINT8 keymainsignal;
volatile INTERRUPT_EXT UINT8 chagreflash_cnt;
volatile INTERRUPT_EXT UINT8 chagreflash_flag;
volatile INTERRUPT_EXT UINT8 batterytest;

#endif




#endif
