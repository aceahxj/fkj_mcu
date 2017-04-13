/***************************************************************
*  FileName              :    delay.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    ϵͳ��ʱ����
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#include "stm32f0xx.h"
#include "delay.h"
#include "bsp.h"



static UINT8  fac_us=0;//us��ʱ������
static UINT16 fac_ms=0;//ms��ʱ������



//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8=6M
//SYSCLK:ϵͳʱ��
void delay_init(UINT8 SYSCLK)
{
	SysTick->CTRL &= 0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
	fac_us = SYSCLK/8;
	fac_ms = (UINT16)fac_us * 1000;
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��48M������,nms<=2796
void delay_ms(UINT16 nms)
{
	volatile UINT32 temp = 0;
	SysTick->LOAD = (UINT32)nms * fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL  = 0x00;           //��ռ�����
	SysTick->CTRL = 0x01 ;          //��ʼ����
	do
	{
		feedwwdg();
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && (!(temp & (1 << 16))));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����
}
//��ʱnus
//nusΪҪ��ʱ��us��.
void delay_us(UINT32 nus)
{
	volatile UINT32 temp = 0;
	SysTick->LOAD=nus*fac_us; //ʱ�����
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ����
	do
	{
		feedwwdg();
		temp=SysTick->CTRL;
	}
	while((temp & 0x01) && (!(temp & (1 << 16))));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����
}

/**************************************************************
*  Function Name         :   mydelayms
*  Param                 :   UINT32 ms
*  Return Param          :   void
*  Description           :   ����ʱms��
***************************************************************/
void mydelayms(UINT32 ms)
{
	volatile UINT16 idx;
	volatile UINT16 jdx;

    for(idx = ms; idx > 0; idx--) {
		feedwwdg();
        for(jdx = 4580; jdx > 0; jdx--);
    }
}

/**************************************************************
*  Function Name         :   mydelayus
*  Param                 :   UINT32 us
*  Return Param          :   void
*  Description           :   ����ʱus��
***************************************************************/
void mydelayus(UINT32 us)
{
	volatile UINT8 idx;
	volatile UINT8 jdx;

    for(idx = us; idx > 0; idx--) {
		feedwwdg();
        for(jdx = 60; jdx > 0; jdx--);
    }
}

