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
*  Description           :    �ڴ��������
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "lib_type.h"


void memoryset(void* des,INT8U value,INT32U length);                //��ָ���ڴ��ַд��ָ������
void memorycpy(void* des, void* src,INT32U length);                //�ڴ����ݸ���
INT8U memorycmp(const void* str1,const void* str2,INT32U length);   //�ڴ����ݱȽ�

//�ַ�������
INT32U stringlen(const char* str);                                        //�����ַ�������
//void bufsort(UINT16 buffer[],UINT16 len);


#endif

