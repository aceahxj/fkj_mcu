/***************************************************************
*  FileName              :    flash.h
*  Copyright             :
*  ModuleName            :
*  
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    flash驱动
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#ifndef _FLASH_H
#define _FLASH_H

#include "stm32f0xx.h"
#include "lib_type.h"
#include "msg.h"

#define   PAGE_SIZE                  0x400                          //1K
#define   HALF_PAGE_SIZE             0x200                          //512 bytes
#define   FLASH_SIZE                 0x10000                         //32K

#define   ERR_MAX_NUM                85                             //一页Flash最多存储的错误信息数量


#define   STINFOADDR                 ((uint32_t)0x0800FC00)                  //仪器信息首地址
#define   STINFO_CHKSUMADDR          ((uint32_t)0x0800FDFE)                  //仪器信息校验和保存地址

#define   DAFUTPARADDR               ((uint32_t)0x0800FE00)                  //默认参数首地址
#define   DAFUTPAR_CHKSUMADDR        ((uint32_t)0x0800FFFE)                  //默认参数校验和保存地址

#define   CURTUSERINFOADDR           ((uint32_t)0x0800F800)                  //使用用户信息首地址
#define   CURTINFO_CHKSUMADDR        ((uint32_t)0x0800F9FE)                  //使用用户信息校验和保存地址

#define   REGUSERINFOADDR            ((uint32_t)0x0800FA00)                  //注册用户信息首地址
#define   REGINFO_CHKSUMADDR         ((uint32_t)0x0800FBFE)                  //注册用户信息校验和保存地址

#define   ERRORINFOADDR              ((uint32_t)0x0800F400)                  //错误信息首地址

#define   ApplicationKeywordAddr     ((uint32_t)0x08001C00)                  //IAP标志保存Flash地址        

#define   RunIAPKeyword               0xA5A55A5A

#define   DEF_FAIL   0u
#define   DEF_OK     1u


//firmware固件更新状态
typedef enum {
UPDATE_Ready            = 0x00,         //设置进入IAP标志
UPDATE_Reset            = 0X01,         //系统复位重启
UPDATE_Start    		= 0x02,  		// 传输启动
UPDATE_Transmit    	    = 0x03,  		// 传输bin文件
UPDATE_End    			= 0x04,   		// 传输结束
UPDATE_Query    		= 0x05,  		// 查询状态
} UpdateState;

//firmware固件更新从机应答
typedef enum {
UPDATE_OK    		= 0x01,  		// 正常
UPDATE_Error    	= 0x02,  		// 错误
UPDATE_Busy    		= 0x03,   		// 忙
} UpdateAck;

__IO extern UINT8 resetflag;                                    //复位标志

void SaveDataToFlash(INT32U addr, INT16U len, void *buf);
INT8U ReadDataFromFlash(INT32U addr, INT16U len, void *buf);
void SoftReset(void);
void firmwareprogram(MsgTag* rxMsg, MsgTag* replyMsg);     //固件更新



#endif
