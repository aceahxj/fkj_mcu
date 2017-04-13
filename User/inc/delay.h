/***************************************************************
*  FileName              :    delay.h
*  Copyright             :
*  ModuleName            :
*  
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    系统延时函数
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#ifndef __DELAY_H
#define __DELAY_H 

#include "lib_type.h"

#ifdef    DELAY_GLOBAL
#define   DELAY_EXT   
#else
#define   DELAY_EXT     extern
#endif

void delay_init(INT8U SYSCLK);
void delay_ms(INT16U nms);
void delay_us(INT32U nus);
void mydelayms(UINT32 ms);
void mydelayus(UINT32 us);
#endif

