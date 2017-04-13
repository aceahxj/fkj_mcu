/***************************************************************
*  FileName              :    memory.h
*  Copyright             :
*  ModuleName            :
*  
*  CPU                   :
*  RTOS                  :

*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    内存操作函数
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "lib_type.h"


void memoryset(void* des,INT8U value,INT32U length);                //向指定内存地址写入指定数据
void memorycpy(void* des, void* src,INT32U length);                //内存数据复制
INT8U memorycmp(const void* str1,const void* str2,INT32U length);   //内存数据比较

//字符串复制
INT32U stringlen(const char* str);                                        //计算字符串长度
//void bufsort(UINT16 buffer[],UINT16 len);


#endif

