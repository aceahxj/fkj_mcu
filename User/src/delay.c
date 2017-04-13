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
*  Description           :    系统延时函数
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#include "stm32f0xx.h"
#include "delay.h"
#include "bsp.h"



static UINT8  fac_us=0;//us延时倍乘数
static UINT16 fac_ms=0;//ms延时倍乘数



//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8=6M
//SYSCLK:系统时钟
void delay_init(UINT8 SYSCLK)
{
	SysTick->CTRL &= 0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us = SYSCLK/8;
	fac_ms = (UINT16)fac_us * 1000;
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对48M条件下,nms<=2796
void delay_ms(UINT16 nms)
{
	volatile UINT32 temp = 0;
	SysTick->LOAD = (UINT32)nms * fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL  = 0x00;           //清空计数器
	SysTick->CTRL = 0x01 ;          //开始倒数
	do
	{
		feedwwdg();
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && (!(temp & (1 << 16))));//等待时间到达
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器
}
//延时nus
//nus为要延时的us数.
void delay_us(UINT32 nus)
{
	volatile UINT32 temp = 0;
	SysTick->LOAD=nus*fac_us; //时间加载
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数
	do
	{
		feedwwdg();
		temp=SysTick->CTRL;
	}
	while((temp & 0x01) && (!(temp & (1 << 16))));//等待时间到达
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器
}

/**************************************************************
*  Function Name         :   mydelayms
*  Param                 :   UINT32 ms
*  Return Param          :   void
*  Description           :   简单延时ms级
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
*  Description           :   简单延时us级
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

