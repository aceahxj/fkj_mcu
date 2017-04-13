/***************************************************************
*  FileName              :    memory.c
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
#include "memory.h"
#include "bsp.h"


/**************************************************************
*  Function Name         :   memoryset 
*  Param                 :   void * des,INT8U value,INT32U length
*  Return Param          :   void
*  Description           :   指定内存写数据
***************************************************************/
void memoryset(void* data, INT8U value, INT32U length)
{
	INT32U idx;
	
	assert_param(data != NULL);
	for(idx = 0; idx < length; idx++)
	{
		((INT8U *)data)[idx] = value;
	}
}

/**************************************************************
*  Function Name         :   memorycpy 
*  Param                 :   void * des,const void * src,INT32U length
*  Return Param          :   void
*  Description           :   内存数据复制
***************************************************************/
void memorycpy(void* dest, void* src, INT32U length)
{
	INT32U idx = 0;

	assert_param(dest != NULL);
	assert_param(src != NULL);
	for(idx = 0; idx < length; idx++)
	{
		feedwwdg();
		((INT8U *)dest)[idx] = ((INT8U*)src)[idx];
	}
}

/**************************************************************
*  Function Name         :   memorycmp 
*  Param                 :   const void * str1,const void * str2,INT32U length
*  Return Param          :   void
*  Description           :   内存数据比较，不同为1
***************************************************************/
INT8U memorycmp(const void* str1,const void* str2,INT32U length)
{
	INT32U idx;
    INT8U temp = 0;
	assert_param(NULL != str1);
	assert_param(NULL != str2);
	for(idx = 0;idx < length;idx++)
	{
            if(((const INT8U*)str1)[idx] != ((const INT8U*)str2)[idx])
           {
            temp = 1;
	    break;
           }
	}
	return temp;
}

/**************************************************************
*  Function Name         :   stringlen 
*  Param                 :   char * str1
*  Return Param          :   void
*  Description           :   字符串长度计算
***************************************************************/
INT32U stringlen(const char* str)
{
    INT32U temp = 0;
    assert_param(NULL != str);
    while(*str++ != '\0')
    {
        temp++;
    }
    return temp;
}

/**************************************************************
*  Function Name         :   bufsort 
*  Param                 :   const void * buffer
*  Return Param          :   void
*  Description           :   数组排序，(大到小)
***************************************************************/
/*void bufsort(UINT16 buffer[],UINT16 len)
{
    UINT8 idx;
	UINT8 jdx;
	UINT16 tmp;
	
	assert_param(NULL != buffer);
	
    for (idx = 1; idx < len; ++idx)
    {
	    tmp = buffer[idx];
	    for (jdx = idx - 1; jdx > 0; --jdx)
	    {
		    if (tmp > buffer[jdx])
		    {			  
			    buffer[jdx + 1] = buffer[jdx];
				buffer[jdx] = tmp;
		    }
		    else
			    break;
	    }
	    buffer[jdx + 1] = tmp;
    }
}*/ 


