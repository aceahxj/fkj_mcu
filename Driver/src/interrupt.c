/***************************************************************
*  FileName              :    interrupt.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    中断函数
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#define INTERRUPT_GLOBAL

#include "interrupt.h"
#include "uart.h"
#include "bsp.h"
#include "delay.h"
#include "adc.h"
#include "buzzerpwm.h"
#include "systemwork.h"
#include "stm32f0xx.h"
#include "atconfig.h"
#include "systemwork.h"
#include "msg.h"



#define  KEY_STATE0  0
#define  KEY_STATE1  1
#define  KEY_STATE2  2
#define  KEY_STATE3  3



/**************************************************************
*  Function Name		 :	 key_driver
*  Param				 :	 void
*  Return Param 		 :	 UINT8
*  Description		 :	 按键驱动
***************************************************************/
static KeyType key_driver(void)
{
	static UINT8 key_state = KEY_STATE0;
	static UINT16 key_time = 0;
	UINT16 key_press;
	KeyType key_return = NONE_KEY;                      //按键类型单、双、长按

    if (keyHandleInfor.keyValue == KEY_MAIN)            //判断要读的按键I/O电平,按下为0
        key_press = KEY_MAIN();
    else if (keyHandleInfor.keyValue == KEY_ZOOM_OUT)
        key_press = KEY_ZOOM_OUT();
    else if (keyHandleInfor.keyValue == KEY_ZOOM_IN)
        key_press = KEY_ZOOM_IN();
	else if (keyHandleInfor.keyValue == KEY_UP)
		key_press = KEY_UP();
	else if (keyHandleInfor.keyValue == KEY_DOWN)
		key_press = KEY_DOWN();
	else if (keyHandleInfor.keyValue == KEY_LED)
		key_press = KEY_LED();
	else if (keyHandleInfor.keyValue == KEY_PRINT)
		key_press = KEY_PRINT();
	else if (keyHandleInfor.keyValue == KEY_VIDEO)
		key_press = KEY_VIDEO();
    else
        return key_return;

	switch (key_state)
	{
      case KEY_STATE0:
            if (!key_press)                         //转到按键消抖态
            {
                key_state = KEY_STATE1;
            }
            break;
      case KEY_STATE1:                              // 按键消抖与确认态
        if (!key_press) {
			key_time = 0;
            key_state = KEY_STATE2;                 // 按键仍然处于按下，消抖完成，状态转换到按下键时间的计时状态
            oneshortbuzze();
        }
        else {
             //keytaskflag = 0;
             key_state = KEY_STATE0;                // 按键已抬起，转换到按键初始态。此处完成和实现软件消抖
        }
        break;
      case KEY_STATE2:
        if(key_press) {                             //按键松开
			 key_return = SIGNLE_KEY;				// 返回单击
             key_state = KEY_STATE0;                // 转换到按键初始态
        } else {
        	if ((keyHandleInfor.keyValue == KEY_MAIN)||(keyHandleInfor.keyValue == KEY_ZOOM_OUT)
                ||(keyHandleInfor.keyValue == KEY_UP)||(keyHandleInfor.keyValue == KEY_LED)
                ||(keyHandleInfor.keyValue == KEY_PRINT)) {
				if (++key_time > 70) { 				// 继续按下，计时加20ms
             		key_return = LONG_KEY;          // 按下时间>2000ms，返回长键事件
             		key_state = KEY_STATE0;
        		} else {
            		key_state = KEY_STATE2;
        		}
        	} else {
        		key_return = SIGNLE_KEY;
				key_state = KEY_STATE0;
        	}
        }
        break;
	  default: break;
    }
    return key_return;
}

/**************************************************************
*  Function Name         :   void red_led_flash(INT16U timeCount)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   状态指示灯闪烁
***************************************************************/
void red_led_flash(INT16U timeCount)
{
    if (++redLedDisp.timeCount == timeCount) {
        redLedDisp.timeCount = 0;
        if (LED_ON == redLedDisp.state) {
            LED_RED_OFF();
            redLedDisp.state = LED_OFF;
        } else {
            LED_RED_ON();
            redLedDisp.state = LED_ON;
        }
    }
}

/**************************************************************
*  Function Name         :   void green_led_flash(INT16U timeCount)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   状态指示灯闪烁
***************************************************************/
void green_led_flash(INT16U timeCount)
{
    if (++greenLedDisp.timeCount == timeCount) {
        greenLedDisp.timeCount = 0;
        if (LED_ON == greenLedDisp.state) {
            LED_GREEN_OFF();
            greenLedDisp.state = LED_OFF;
        } else {
            LED_GREEN_ON();
            greenLedDisp.state = LED_ON;
        }
    }
}

/**************************************************************
*  Function Name         :   void state_led_show(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   状态指示灯显示
***************************************************************/
void state_led_show(void)
{
    //同步Flash
    if ((redLedDisp.ledType == greenLedDisp.ledType)
        &&(redLedDisp.ledType > LED_ON)) {
        if (redLedDisp.timeCount > greenLedDisp.timeCount) {
            greenLedDisp.state = redLedDisp.state;
            greenLedDisp.timeCount = redLedDisp.timeCount;
        } else {
            redLedDisp.state = greenLedDisp.state;
            redLedDisp.timeCount = greenLedDisp.timeCount;
        }
    }
    switch(redLedDisp.ledType)
    {
    	case LED_OFF:
    	LED_RED_OFF();
        redLedDisp.state = LED_OFF;
        redLedDisp.timeCount = 0;
    	break;

        case LED_ON:
        LED_RED_ON();
        redLedDisp.state = LED_ON;
        redLedDisp.timeCount = 0;
        break;

        case LED_FLASH1000:
        red_led_flash(10);
        break;

        case LED_FLASH300:
        red_led_flash(3);
        break;

        case LED_FLASH100:
        red_led_flash(1);
        break;

        default:
        break;
    }

    switch(greenLedDisp.ledType)
    {
    	case LED_OFF:
    	LED_GREEN_OFF();
        greenLedDisp.state = LED_OFF;
        greenLedDisp.timeCount = 0;
    	break;

        case LED_ON:
        LED_GREEN_ON();
        greenLedDisp.state = LED_ON;
        greenLedDisp.timeCount = 0;
        break;

        case LED_FLASH1000:
        green_led_flash(10);
        break;

        case LED_FLASH300:
        green_led_flash(3);
        break;

        case LED_FLASH100:
        green_led_flash(1);
        break;

        default:
        break;
    }
}

/**************************************************************
*  Function Name         :   TIM3_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   timer3中断函数
***************************************************************/
void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

/**************************************************************
*  Function Name         :   TIM14_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   timer14中断函数 秒中断
***************************************************************/
void TIM14_IRQHandler(void)
{

}

/**************************************************************
*  Function Name         :   TIM16_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   timer16中断函数 100ms中断
***************************************************************/
void TIM16_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM16,TIM_IT_Update);
    state_led_show();
    if (waitReply.replyFlag)
        waitReply.timeCount++;
    if (waitAction.replyFlag)
        waitAction.timeCount++;
}

/**************************************************************
*  Function Name         :   TIM17_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   timer17中断函数，扫描按键 20ms中断
***************************************************************/
void TIM17_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM17,TIM_IT_Update);

    keyHandleInfor.keyType = key_driver();
    if (keyHandleInfor.keyType != NONE_KEY)
        TIM_Cmd(TIM17, DISABLE);
    #if 0
    if(keyHandleInfor.keyType == SIGNLE_KEY)                        /*用户单击*/
    {
    	TIM_Cmd(TIM17, DISABLE);
    } else if (keyHandleInfor.keyType == LONG_KEY)    				/*用户长按*/
    {
    	TIM_Cmd(TIM17, DISABLE);
    }
    #endif
}

/**************************************************************
*  Function Name         :   USART1_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   uart1中断函数 USB通信
***************************************************************/
void USART1_IRQHandler(void)
{
	INT8U data = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART1);
	    Uart1RxDataHandle(data);
	}
	USART_ClearITPendingBit(USART1,USART_IT_ORE);
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

/**************************************************************
*  Function Name         :   USART2_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   uart2中断函数 蓝牙通信
***************************************************************/
void USART2_IRQHandler(void)
{
	INT8U data = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART2);
	    Uart2RxDataHandle(data);
	}
	USART_ClearITPendingBit(USART2,USART_IT_ORE);
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}

/**************************************************************
*  Function Name         :   RTC_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   RTC中断
***************************************************************/
void RTC_IRQHandler(void)
{
    pre_charge_init();
}

/**************************************************************
*  Function Name         :   EXTI0_1_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   主功能按键中断
***************************************************************/
void EXTI0_1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET) {       	//KEY_MAIN

    	if (instState != ST_STOP)
    	{
        	keyHandleInfor.keyValue = KEY_MAIN;
    	} else {
    		keyHandleInfor.keyValue = KEY_MAIN;
            key_open_system();
            instState = ST_WAKEUP;
            wakeupFlag = 0;         //如果不是长按重新进入Stop
    	}
        TIM_Cmd(TIM17, ENABLE);
 		EXTI_ClearFlag(EXTI_Line0);
		EXTI_ClearITPendingBit(EXTI_Line0);
    } else if (EXTI_GetITStatus(EXTI_Line1) == SET) {    //KEY_ZOOM_OUT

        keyHandleInfor.keyValue = KEY_ZOOM_OUT;
        TIM_Cmd(TIM17, ENABLE);
        EXTI_ClearFlag(EXTI_Line1);
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

/***************************************************************
*  Function Name         :   EXTI4_15_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   usb中断
***************************************************************/
void EXTI4_15_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line7) == SET) {       	//KEY_LED
        keyHandleInfor.keyValue = KEY_LED;
		TIM_Cmd(TIM17, ENABLE);
 		EXTI_ClearFlag(EXTI_Line7);
		EXTI_ClearITPendingBit(EXTI_Line7);
    } else if (EXTI_GetITStatus(EXTI_Line8) == SET) {    //KEY_VIDEO
        keyHandleInfor.keyValue = KEY_VIDEO;
        TIM_Cmd(TIM17, ENABLE);
        EXTI_ClearFlag(EXTI_Line8);
        EXTI_ClearITPendingBit(EXTI_Line8);
    } else if (EXTI_GetITStatus(EXTI_Line9) == SET) {    //KEY_PRINT
        keyHandleInfor.keyValue = KEY_PRINT;
        TIM_Cmd(TIM17, ENABLE);
        EXTI_ClearFlag(EXTI_Line9);
        EXTI_ClearITPendingBit(EXTI_Line9);
    } else if (EXTI_GetITStatus(EXTI_Line11) == SET) {    //USB_IN OUT
        if (ST_STOP == instState) {
            key_open_system();
            delay_ms(50);                   //延时去抖动
            if (USB_IN()) {
                pre_charge_init();
                chargeFlag = 1;
                instState = ST_CHARGING;    //关机充电
            } else {
                led_off();                  //先关显示
                instState = ST_STOP;        //重新进入待机
            }
	    } else if (ST_CHARGING == instState) {
            delay_ms(50);                   //延时去抖动
            if (USB_IN()) {

            } else {
                chargeFlag = 0;
                led_off();                  //先关显示
                instState = ST_STOP;        //重新进入待机
            }
	    } else {
            delay_ms(50);                   //延时去抖动
            if (USB_IN()) {
                chargeFlag = 1;
            } else {
                chargeFlag = 0;
            }
	    }
        EXTI_ClearFlag(EXTI_Line11);
        EXTI_ClearITPendingBit(EXTI_Line11);
    } else if (EXTI_GetITStatus(EXTI_Line13) == SET) {    //KEY_DOWN
        keyHandleInfor.keyValue = KEY_DOWN;
        TIM_Cmd(TIM17, ENABLE);
        EXTI_ClearFlag(EXTI_Line13);
        EXTI_ClearITPendingBit(EXTI_Line13);
    } else if (EXTI_GetITStatus(EXTI_Line14) == SET) {    //KEY_UP
        keyHandleInfor.keyValue = KEY_UP;
        TIM_Cmd(TIM17, ENABLE);
        EXTI_ClearFlag(EXTI_Line14);
        EXTI_ClearITPendingBit(EXTI_Line14);
    } else if (EXTI_GetITStatus(EXTI_Line15) == SET) {    //KEY_ZOOM_IN
        keyHandleInfor.keyValue = KEY_ZOOM_IN;
        TIM_Cmd(TIM17, ENABLE);
        EXTI_ClearFlag(EXTI_Line15);
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}

/***************************************************************
*  Function Name         :   WWDG_IRQHandler
*  Param                 :   void
*  Return Param          :   void
*  Description           :   WWDG看门狗"死前"中断
***************************************************************/
void WWDG_IRQHandler(void)
{
    //WWDG_SetCounter(0x7F);
    //WWDG_ClearFlag();
}



