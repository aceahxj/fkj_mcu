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
*  Description           :    flash����
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
__IO UINT8 resetflag = 0;                                    //��λ��־


/**************************************************************
*  Function Name         :   SaveDataToFlash 
*  Param                 :   INT32U addr, INT16U len, void *buf
*  Return Param          :   void
*  Description           :   �������ݵ�Flash
***************************************************************/
void SaveDataToFlash(INT32U addr, INT16U len, void *buf)
{
    FLASH_Status  FLASHStatus;
    INT16U  idx = 0;
    UNION16 tmp;
    INT16U  CheckSum = 0;

    // ���CPU Flash��
    FLASH_Unlock();

    while (len)
    {
        feedwwdg();
        tmp.ubyte[0] = *((INT8U *)buf + idx);
        tmp.ubyte[1] = *((INT8U *)buf + idx + 1);

        CheckSum += tmp.ubyte[0] + tmp.ubyte[1];

        FLASHStatus = FLASH_ProgramHalfWord(addr + idx, tmp.ushort);         // д����
        while (FLASHStatus != FLASH_COMPLETE)
            feedwwdg();
        idx += 2; 
		len -= 2; 
    }
	if((addr == CURTUSERINFOADDR) || (addr == REGUSERINFOADDR) || (addr == DAFUTPARADDR) || (addr == STINFOADDR))              
	{
	   FLASHStatus = FLASH_ProgramHalfWord(addr + HALF_PAGE_SIZE - 2, CheckSum);      //дУ���,�ڰ�ҳ����������ֽ�
	   while (FLASHStatus != FLASH_COMPLETE)
            feedwwdg();       
	}
	else
	{
	   FLASHStatus = FLASH_ProgramHalfWord(addr + idx, CheckSum);               //дУ���
	   while (FLASHStatus != FLASH_COMPLETE)
            feedwwdg();
	}

    FLASH_Lock();
}

/**************************************************************
*  Function Name         :   ReadDataFromFlash 
*  Param                 :   INT32U addr, INT16U len, void *buf
*  Return Param          :   INT8U
*  Description           :   ��ȡflash����
***************************************************************/
INT8U ReadDataFromFlash(INT32U addr, INT16U len, void *buf)
{
    INT8U   rtn;
    INT16U  idx = 0;
    UNION16 tmp;
    INT16U  CheckSum = 0, verify = 0;

	//�ȶ�ȡУ���
	if((addr == CURTUSERINFOADDR) || (addr == REGUSERINFOADDR) || (addr == DAFUTPARADDR) || (addr == STINFOADDR))              
	{
	   tmp.ushort = *(volatile INT16U *)(addr + HALF_PAGE_SIZE -2);  //��ȡУ���
	   if(0xffff == tmp.ushort)
	   {
          rtn = DEF_FAIL;
		  return rtn;
	   }
	}
    //��ȡ����
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
	//��ȡУ���
	if((addr == CURTUSERINFOADDR) || (addr == REGUSERINFOADDR) || (addr == DAFUTPARADDR) || (addr == STINFOADDR))              
	{
	   tmp.ushort = *(volatile INT16U *)(addr + HALF_PAGE_SIZE -2);  //��ȡУ���
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
*  Description           :   ϵͳ��λ����
***************************************************************/
void SoftReset(void)
{
    CLI();                   // �ر������ж�
    NVIC_SystemReset();      // ��λ
}

/**************************************************************
*  Function Name         :   setIAPflag 
*  Param                 :   void
*  Return Param          :   FLASH_Status
*  Description           :   ����IAP��־
***************************************************************/
static FLASH_Status setIAPflag(void)
{	
    FLASH_Status __IO FLASHStatus;

    // ���CPU Flash��
    FLASH_Unlock();
    
    FLASHStatus = FLASH_ErasePage(ApplicationKeywordAddr);
    while (FLASHStatus != FLASH_COMPLETE);

   
	FLASHStatus = FLASH_ProgramWord(ApplicationKeywordAddr, RunIAPKeyword);         // д����
    while (FLASHStatus != FLASH_COMPLETE);
 
    return FLASHStatus;
}

/**************************************************************
*  Function Name         :   firmwareprogram 
*  Param                 :   MsgTag* rxMsg, MsgTag* replyMsg
*  Return Param          :   void
*  Description           :   �̼�����
***************************************************************/
void firmwareprogram(MsgTag* rxMsg, MsgTag* replyMsg)
{
    UpdateAck ack = UPDATE_OK;
	
	switch(rxMsg->pkt.msg.para[0])
	{
		case UPDATE_Ready:                                  //����IAP������־
		     if(FLASH_COMPLETE != setIAPflag())
		     {
                ack = UPDATE_Error;
			 }
			 break;

		case UPDATE_Reset:                                  //ϵͳ��λ����
		     resetflag = 1;
			 break;
        case UPDATE_Start:                                  //��������
    	break;
               
		case UPDATE_Transmit:                               //����bin�ļ�

		break;

		case UPDATE_End:                                    //�������

		break;

		case UPDATE_Query:                                  //��ѯ״̬

		break;

		default:

			break;		
	}
	replyMsg->pkt.msg.para[0] = ack;
	replyMsg->pkt.msg.length = 1 + MSG_END_SIZE;
}	

