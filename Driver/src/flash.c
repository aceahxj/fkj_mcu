/***************************************************************
*  FileName              :    flash.c
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
#include "flash.h"
#include "bsp.h"
#include "lib_type.h"


static FLASH_Status setIAPflag(void);
__IO UINT8 resetflag = 0;                                    //复位标志


/**************************************************************
*  Function Name         :   SaveDataToFlash 
*  Param                 :   INT32U addr, INT16U len, void *buf
*  Return Param          :   void
*  Description           :   保存数据到Flash
***************************************************************/
void SaveDataToFlash(INT32U addr, INT16U len, void *buf)
{
    FLASH_Status  FLASHStatus;
    INT16U  idx = 0;
    UNION16 tmp;
    INT16U  CheckSum = 0;

    // 解除CPU Flash锁
    FLASH_Unlock();

    while (len)
    {
        feedwwdg();
        tmp.ubyte[0] = *((INT8U *)buf + idx);
        tmp.ubyte[1] = *((INT8U *)buf + idx + 1);

        CheckSum += tmp.ubyte[0] + tmp.ubyte[1];

        FLASHStatus = FLASH_ProgramHalfWord(addr + idx, tmp.ushort);         // 写数据
        while (FLASHStatus != FLASH_COMPLETE)
            feedwwdg();
        idx += 2; 
		len -= 2; 
    }
	if((addr == CURTUSERINFOADDR) || (addr == REGUSERINFOADDR) || (addr == DAFUTPARADDR) || (addr == STINFOADDR))              
	{
	   FLASHStatus = FLASH_ProgramHalfWord(addr + HALF_PAGE_SIZE - 2, CheckSum);      //写校验和,在半页的最后两个字节
	   while (FLASHStatus != FLASH_COMPLETE)
            feedwwdg();       
	}
	else
	{
	   FLASHStatus = FLASH_ProgramHalfWord(addr + idx, CheckSum);               //写校验和
	   while (FLASHStatus != FLASH_COMPLETE)
            feedwwdg();
	}

    FLASH_Lock();
}

/**************************************************************
*  Function Name         :   ReadDataFromFlash 
*  Param                 :   INT32U addr, INT16U len, void *buf
*  Return Param          :   INT8U
*  Description           :   读取flash数据
***************************************************************/
INT8U ReadDataFromFlash(INT32U addr, INT16U len, void *buf)
{
    INT8U   rtn;
    INT16U  idx = 0;
    UNION16 tmp;
    INT16U  CheckSum = 0, verify = 0;

	//先读取校验和
	if((addr == CURTUSERINFOADDR) || (addr == REGUSERINFOADDR) || (addr == DAFUTPARADDR) || (addr == STINFOADDR))              
	{
	   tmp.ushort = *(volatile INT16U *)(addr + HALF_PAGE_SIZE -2);  //读取校验和
	   if(0xffff == tmp.ushort)
	   {
          rtn = DEF_FAIL;
		  return rtn;
	   }
	}
    //读取数据
    while (len)
    {
        feedwwdg();
        tmp.ushort = *(volatile INT16U *)(addr + idx);
        CheckSum += tmp.ubyte[0] + tmp.ubyte[1];
		
        *((INT8U *)buf + idx)     = tmp.ubyte[0];
        *((INT8U *)buf + idx + 1) = tmp.ubyte[1];
        idx += 2;
        len -= 2;        
    }
	//读取校验和
	if((addr == CURTUSERINFOADDR) || (addr == REGUSERINFOADDR) || (addr == DAFUTPARADDR) || (addr == STINFOADDR))              
	{
	   tmp.ushort = *(volatile INT16U *)(addr + HALF_PAGE_SIZE -2);  //读取校验和
	}
	else
	{
       tmp.ushort = *(volatile INT16U *)(addr + idx);
	}

    verify = tmp.ushort;
	if (CheckSum == verify) 
		rtn = DEF_OK;
	else 
		rtn = DEF_FAIL;

    return rtn;
}

/**************************************************************
*  Function Name         :   SoftReset 
*  Param                 :   void
*  Return Param          :   void
*  Description           :   系统复位重启
***************************************************************/
void SoftReset(void)
{
    CLI();                   // 关闭所有中断
    NVIC_SystemReset();      // 复位
}

/**************************************************************
*  Function Name         :   setIAPflag 
*  Param                 :   void
*  Return Param          :   FLASH_Status
*  Description           :   设置IAP标志
***************************************************************/
static FLASH_Status setIAPflag(void)
{	
    FLASH_Status __IO FLASHStatus;

    // 解除CPU Flash锁
    FLASH_Unlock();
    
    FLASHStatus = FLASH_ErasePage(ApplicationKeywordAddr);
    while (FLASHStatus != FLASH_COMPLETE);

   
	FLASHStatus = FLASH_ProgramWord(ApplicationKeywordAddr, RunIAPKeyword);         // 写数据
    while (FLASHStatus != FLASH_COMPLETE);
 
    return FLASHStatus;
}

/**************************************************************
*  Function Name         :   firmwareprogram 
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   固件更新
***************************************************************/
void firmwareprogram(MsgTag* rxMsg, MsgTag* replyMsg)
{
    UpdateAck ack = UPDATE_OK;
	
	switch(rxMsg->pkt.msg.para[0])
	{
		case UPDATE_Ready:                                  //设置IAP启动标志
		     if(FLASH_COMPLETE != setIAPflag())
		     {
                ack = UPDATE_Error;
			 }
			 break;

		case UPDATE_Reset:                                  //系统复位重启
		     resetflag = 1;
			 break;
        case UPDATE_Start:                                  //启动传输
    	break;
               
		case UPDATE_Transmit:                               //传输bin文件

		break;

		case UPDATE_End:                                    //传输结束

		break;

		case UPDATE_Query:                                  //查询状态

		break;

		default:

			break;		
	}
	replyMsg->pkt.msg.para[0] = ack;
	replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
}	

