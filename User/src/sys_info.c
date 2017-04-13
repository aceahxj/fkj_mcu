/***************************************************************
*  FileName              :    sys_info.c
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
#define	SYS_INFO_GLOBAL

#include "memory.h"
#include "lib_type.h"
#include "adc.h"
#include "cmd.h"
#include "flash.h"
#include "delay.h"
#include "sys_info.h"
#include "atconfig.h"
#include "bsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f0xx.h"
#include "stm32f0xx_flash.h"


/**************************************************************
*  Function Name         :   Erase_FlashPage
*  Param                 :   INT32U addr
*  Return Param          :   void
*  Description           :   ����ָ����flashҳ
***************************************************************/
void Erase_FlashPage(INT32U addr)
{
	FLASH_Status FLASHStatus;

	//���CPU Flash��
	FLASH_Unlock();
    CLI();
	//����Flash
	FLASHStatus = FLASH_ErasePage(addr);
	while (FLASHStatus != FLASH_COMPLETE) {
	}
    SEI();
	//��סFlash
	FLASH_Lock();
}

/**************************************************************
*  Function Name         :   calculate_chksum
*  Param                 :   void * data,UINT32 len
*  Return Param          :   UINT16
*  Description           :   ����У���
***************************************************************/
UINT16 calculate_chksum(UINT8* data,UINT32 len)
{
	UINT16 chksum = 0;

	while(len--)
    {
       chksum += *(data++);
	}
	return chksum;
}


/**************************************************************
*  Function Name         :   erasepageinfo
*  Param                 :   INT32U addr
*  Return Param          :   FlashAck
*  Description           :   ����ҳ��Ϣ
***************************************************************/
UINT8 erasepageinfo(INT32U addr)
{
    FLASH_Status __IO FLASHStatus;
	UINT8 ack = 0;

    //���CPU Flash��
    FLASH_Unlock();

	//����Flash
    FLASHStatus = FLASH_ErasePage(addr);
    while (FLASHStatus != FLASH_COMPLETE) {
         feedwwdg();
    }
	//��סFlash
	FLASH_Lock();
	return ack;
}

