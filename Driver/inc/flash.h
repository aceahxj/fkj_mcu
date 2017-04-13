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
*  Description           :    flash����
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

#define   ERR_MAX_NUM                85                             //һҳFlash���洢�Ĵ�����Ϣ����


#define   STINFOADDR                 ((uint32_t)0x0800FC00)                  //������Ϣ�׵�ַ
#define   STINFO_CHKSUMADDR          ((uint32_t)0x0800FDFE)                  //������ϢУ��ͱ����ַ

#define   DAFUTPARADDR               ((uint32_t)0x0800FE00)                  //Ĭ�ϲ����׵�ַ
#define   DAFUTPAR_CHKSUMADDR        ((uint32_t)0x0800FFFE)                  //Ĭ�ϲ���У��ͱ����ַ

#define   CURTUSERINFOADDR           ((uint32_t)0x0800F800)                  //ʹ���û���Ϣ�׵�ַ
#define   CURTINFO_CHKSUMADDR        ((uint32_t)0x0800F9FE)                  //ʹ���û���ϢУ��ͱ����ַ

#define   REGUSERINFOADDR            ((uint32_t)0x0800FA00)                  //ע���û���Ϣ�׵�ַ
#define   REGINFO_CHKSUMADDR         ((uint32_t)0x0800FBFE)                  //ע���û���ϢУ��ͱ����ַ

#define   ERRORINFOADDR              ((uint32_t)0x0800F400)                  //������Ϣ�׵�ַ

#define   ApplicationKeywordAddr     ((uint32_t)0x08001C00)                  //IAP��־����Flash��ַ        

#define   RunIAPKeyword               0xA5A55A5A

#define   DEF_FAIL   0u
#define   DEF_OK     1u


//firmware�̼�����״̬
typedef enum {
UPDATE_Ready            = 0x00,         //���ý���IAP��־
UPDATE_Reset            = 0X01,         //ϵͳ��λ����
UPDATE_Start    		= 0x02,  		// ��������
UPDATE_Transmit    	    = 0x03,  		// ����bin�ļ�
UPDATE_End    			= 0x04,   		// �������
UPDATE_Query    		= 0x05,  		// ��ѯ״̬
} UpdateState;

//firmware�̼����´ӻ�Ӧ��
typedef enum {
UPDATE_OK    		= 0x01,  		// ����
UPDATE_Error    	= 0x02,  		// ����
UPDATE_Busy    		= 0x03,   		// æ
} UpdateAck;

__IO extern UINT8 resetflag;                                    //��λ��־

void SaveDataToFlash(INT32U addr, INT16U len, void *buf);
INT8U ReadDataFromFlash(INT32U addr, INT16U len, void *buf);
void SoftReset(void);
void firmwareprogram(MsgTag* rxMsg, MsgTag* replyMsg);     //�̼�����



#endif
