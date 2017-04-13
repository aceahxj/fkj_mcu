/***************************************************************
*  FileName              :    systemwork.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    系统控制函数
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#define SYSTEMWORK_GLOBAL

#include "systemwork.h"
#include "bsp.h"
#include "adc.h"
#include "delay.h"
#include "msg.h"
#include "cmd.h"
#include "uart.h"
#include "atconfig.h"
#include "sys_info.h"
#include "memory.h"
#include "interrupt.h"
#include "flash.h"
#include "cw2015_i2c.h"
#include "buzzerpwm.h"
#include "flash.h"

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void state_led_manage(void);
void misc_task(void);


/**************************************************************
*  Function Name         :   void PowerOn_Test(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   系统上电检测
***************************************************************/
void PowerOn_Test(void)
{
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_BKP_VALUE) {
        RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_BKP_VALUE);
		instState = ST_OFFLINE;//instState = ST_STOP;;	//系统掉电复位准备进入Stop
	} else {
		instState = ST_OFFLINE;  //开门狗复位，转到唤醒状态
	}
}

/**************************************************************
*  Function Name         :   void Charge_Test(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   系统上电检测充电
***************************************************************/
void Charge_Test(void)
{
	if (USB_IN()) {				                    //开机充电检测
        delay_ms(100);
        if (USB_IN()) {                             //确认插入USB
            #if 0
            if (CHARGE_STOP == cellState.chargeState) {
                //电池电量过低没有进入充电，必须关机来充电
                delay_ms(50);
				if (USB_IN()) {                     //再次确认插入USB
					CHARGE_OFF();
					RTC_WakeUp_Configer(10);        //定时10秒唤醒
					instState = ST_STOP;
				}
            }
            #endif
            chargeFlag = 1;
        }
	} else {
        delay_ms(100);
        if (!USB_IN()) {                            //确认拔出USB
            chargeFlag = 0;
        }
	}
}



/**************************************************************
*  Function Name         :   void Reset_Init(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   复位初始化
***************************************************************/
void Reset_Init(void)
{
    //初始化LED
    LED_RED_OFF();
    LED_GREEN_OFF();
    //状态指示灯
    redLedDisp.ledType = LED_OFF;
    redLedDisp.state = LED_OFF;
    redLedDisp.timeCount = 0;
    greenLedDisp.ledType = LED_OFF;
    greenLedDisp.state = LED_OFF;
    greenLedDisp.timeCount = 0;
    //关光照，默认白光
    LIGHT_OFF();
    BOTH_OFF();
    lightState = LIGHT_WHITE;
    wifiState = OFFLINE;
    duckState = DUCK_ON;
    //MB_POWER_OFF();
    GC65_POWER_OFF();
    GC65_PWRKEY_OFF();
    //SENSOR_POWER_OFF();
    TIM_Cmd(TIM16,ENABLE);      //100mS定时开启
}

static void cell_vol_test(void)
{
    cellState.voltage = cell_vol_read();
    if (cellState.voltage < BATTERY_LOWEST) {
        led_off();          //先关显示
        instState = ST_STOP;
	} else {
	    instState = ST_OFFLINE;
	}
}

/**************************************************************
*  Function Name         :   void reset_system(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   系统初始化
***************************************************************/
void reset_system(void)
{
	mydelayms(10);
    RCC_Configer();
	//wwdg_init(0x7F, 0x50, WWDG_Prescaler_8);
    delay_init(8);             //系统内部时钟8MHz,未倍频
    GPIO_Configuration();
    CHARGE_ON();
    MB_POWER_ON();             //开启主板电源，等待主机启动
    SENSOR_POWER_ON();         //开启Sensor电源，准备进入视频模式
	//wwdg_init(0x7F, 0x50, WWDG_Prescaler_8);
    ADC_Configer();
    Key_Configer();
    Uart1_Configer();
    cw_alert_config(1);         //报警设置0%电量
    cw_mode_set(CW_WAKEUP);     //唤醒电量计
    cell_vol_test();            //电池电压测试
    Timer_Configer();
    Rtc_Configer();

    onelongbuzze();             //启动长鸣
	//ResetMode_Test();
	Reset_Init();
    PowerOn_Test();    //检测是否掉电更换电池还是看门狗复位
    Charge_Test();
}

/**************************************************************
*  Function Name         :   void key_open_system(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   长按main唤醒系统
***************************************************************/
void key_open_system(void)
{
    RCC_Configer();
    delay_init(8);
    Timer_Configer();
}

/**************************************************************
*  Function Name         :   void pre_charge_init(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   定时唤醒预充电
***************************************************************/
void pre_charge_init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    //禁止Main键唤醒中断
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	//USB输入分压用于开启充电
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//OUT   LED_RED--PB1,LED_GREEN--PB2,CHARGE--PB10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    //CW2015 I2C1
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;          //用IO模拟
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    cw_mode_set(CW_WAKEUP);               //唤醒电量计
    TIM_Cmd(TIM16,ENABLE);                //100mS定时开启
}

/**************************************************************
*  Function Name         :   void stop_with_red_on(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   Stop时红灯亮
***************************************************************/
void stop_with_red_on(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
}

/**************************************************************
*  Function Name         :   void charge_vol(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   电池预充电电压测试
***************************************************************/
void charge_vol(void)
{
    cellState.chargeState = CHARGE_STOP;
    if (!CHRG_STA1())
        cellState.chargeState &= 0xfe;
    if (!CHRG_STA2())
        cellState.chargeState &= 0xfd;
    cellState.remainTime = cell_rrt_read()&0x7fff;  //最高位报警位
    cellState.voltage = cell_vol_read();
    #if 0
    wakeupFlag = 0;
    if (cellState.voltage < PRE_CHARGE_V1) {
        RTC_WakeUp_Configer(59);   //定时59秒唤醒
	} else if (cellState.voltage < PRE_CHARGE_V2) {
        RTC_WakeUp_Configer(30);   //定时30秒唤醒
	} else if (cellState.voltage < PRE_CHARGE_V3) {
        RTC_WakeUp_Configer(20);   //定时20秒唤醒
	} else if (cellState.voltage < PRE_CHARGE_V4) {
        RTC_WakeUp_Configer(10);   //定时10秒唤醒
	} else if (cellState.voltage < PRE_CHARGE_V5) {
        RTC_WakeUp_Configer(5);   //定时5秒唤醒
	} else {
	    wakeupFlag = 1;
	}
    #endif
}

/**************************************************************
*  Function Name         :   void lightOn(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   开光源
***************************************************************/
static void lightOn(void)
{
    if (LIGHT_WHITE == lightState)
        WHITE_ON();
    else
        BLUE_ON();
    LIGHT_ON();
}

/**************************************************************
*  Function Name         :   led_off(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   进入停机前先关LED显示
***************************************************************/
void led_off(void)
{
    redLedDisp.state = LED_OFF;
    redLedDisp.timeCount = 0;
    greenLedDisp.state = LED_OFF;
    greenLedDisp.timeCount = 0;
    LED_RED_OFF();
    LED_GREEN_OFF();
}

/**************************************************************
*  Function Name         :   sys_enterstop
*  Param                 :   void
*  Return Param          :   void
*  Description           :   系统进入stop模式
***************************************************************/
void sys_enterstop(void)
{
	GC65_POWER_OFF();
	TIM_Cmd(TIM1,DISABLE);          //关闭定时器
	TIM_Cmd(TIM3,DISABLE);
    TIM_Cmd(TIM16,DISABLE);
    TIM_Cmd(TIM17,DISABLE);
	USART_Cmd(USART1,DISABLE);      //关闭串口
	USART_Cmd(USART2,DISABLE);
    ADC_Cmd(ADC1,DISABLE);          //关闭adc
    MB_POWER_OFF();
    SENSOR_POWER_OFF();
    cw_mode_set(CW_SLEEP);          //电量计休眠
	Lowpower_Configer();            //进入stop前设置所有IO口
	Wakeup_Configer();              //设置唤醒
	if (1 == chargeFlag) {
        pre_charge_init();
        instState = ST_CHARGING;    //关机充电
	} else {
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    	WWDG_DeInit();
        PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//进入低功耗模式
	}
}

/**************************************************************
*  Function Name         :   void timeout_polling(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :
***************************************************************/
static void timeout_polling(void)
{
    if (waitReply.replyFlag) {
        if (waitReply.timeCount > 30) {           //超时3秒
            selfInfor.errorCode = HISI_COMM_ERR;
            waitReply.replyFlag = FALSE;
        }
    }
    if (waitAction.replyFlag) {
        if((waitAction.cmd == PHOTO_CTRL)&&(waitAction.timeCount > 30)) {     //拍照超时3秒
            actionState = ST_NO_ACTION;
            waitAction.replyFlag = FALSE;
        }
        if((waitAction.cmd == PRINT_CTRL)&&(waitAction.timeCount > 300)) {    //拍照超时30秒
            actionState = ST_NO_ACTION;
            waitAction.replyFlag = FALSE;
        }
    }
}

/**************************************************************
*  Function Name         :   void systemtask(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   系统任务函数
***************************************************************/
void system_task(void)
{
    if ((instState != ST_CHARGING)&&(instState != ST_STOP)) {
        //按键处理
        key_task();
        //串口数据处理
        checkuartpacket();
        get_cell_state();
        //等待HiSi应答
        timeout_polling();
        //power_monitor();
        duck_detect();
    }
    if (instState != ST_STOP) {
        led_manage();
    }
    work_task();
}

/**************************************************************
*  Function Name         :   LED_pulse
*  Param                 :   INT16U pulse
*  Return Param          :   void
*  Description           :   设置占空比
***************************************************************/
void LED_pulse(INT8U upDown)
{
    if (DOWN == upDown) {
        ledPulse--;                          //CCR占空比
    } else {
        ledPulse++;                          //CCR占空比
    }
    if ((ledPulse > 4)&&(ledPulse < 17)) {   //5~16
        TIM3->CCR2 = ledPulse;
    } else {
        //恢复到最大范围内
        if (ledPulse < 5)
            ledPulse = 5;
        else
            ledPulse = 16;
    }
}

/**************************************************************
*  Function Name         :   reseet_charge
*  Param                 :   void
*  Return Param          :   void
*  Description           :   复位充电芯片
***************************************************************/
void reseet_charge(void)
{
    CLI();
    CHARGE_OFF();
    delay_ms(10);
    CHARGE_ON();
    EXTI_ClearFlag(EXTI_Line11);
    EXTI_ClearITPendingBit(EXTI_Line11);
    SEI();
}

/**************************************************************
*  Function Name         :   void key_task(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   按键处理，串口发送键值给HiSi
***************************************************************/
static void key_task(void)
{
    if (keyHandleInfor.keyType != NONE_KEY) {
        if ((ONLINE == wifiState)||(KEY_MAIN == keyHandleInfor.keyValue)) {
            switch (keyHandleInfor.keyValue)
            {
                //主键
                case  KEY_MAIN:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if (actionState == ST_NO_ACTION) {
                        if (ST_WORK == instState)
                            keyHandleInfor.keyCmd = CMD_CAPTURE;
                        else if (ST_IDLE == instState)
                            keyHandleInfor.keyCmd = CMD_PLAY;
                    }
                } else if (keyHandleInfor.keyType == LONG_KEY) {
                    if (ST_WAKEUP == instState) {
                        wakeupFlag = 1;             //唤醒
                    } else {
                        if (ONLINE == wifiState)
                            keyHandleInfor.keyCmd = CMD_SHUT_DOWN;
                        else
                            twoshortbuzze();
                            led_off();          //先关显示
                            instState = ST_STOP;
                    }
                }
                break;

                //视频或照片放大
        		case KEY_ZOOM_OUT:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if ((ST_WORK == instState)||(ST_IDLE == instState)) {
                        keyHandleInfor.keyCmd = CMD_ZOOM_OUT;
                    }
                } else if (keyHandleInfor.keyType == LONG_KEY) {
                    if (ST_IDLE == instState) {
                        keyHandleInfor.keyCmd = CMD_VIDEO;      //切换到视频模式
                    } else if (ST_WORK == instState) {
                        keyHandleInfor.keyCmd = CMD_PHOTO;      //切换到照片模式
                    }
                }
        		break;

                //视频或照片缩小
                case  KEY_ZOOM_IN:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if ((ST_WORK == instState)||(ST_IDLE == instState)) {
                        keyHandleInfor.keyCmd = CMD_ZOOM_IN;
                    }
                } else if (keyHandleInfor.keyType == LONG_KEY) {

                }
                break;

                //照片上翻
                case  KEY_UP:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if (ST_IDLE== instState) {
                        keyHandleInfor.keyCmd = CMD_UP;
                    } else if (ST_WORK == instState) {
                        LED_pulse(DOWN);
                    }
                } else if (keyHandleInfor.keyType == LONG_KEY) {

                }
        	    break;

                //照片下翻
                case  KEY_DOWN:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if (ST_IDLE== instState) {
                        keyHandleInfor.keyCmd = CMD_DOWN;
                    } else if (ST_WORK == instState) {
                        LED_pulse(UP);
                    }
                } else if (keyHandleInfor.keyType == LONG_KEY) {

                }
        	    break;

                //开关切换LED光照
                case  KEY_LED:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if (ST_WORK == instState) {
                        if (LIGHT_WHITE == lightState) {
                            BLUE_ON();
                            lightState = LIGHT_BLUE;
                        } else if (LIGHT_BLUE == lightState) {
                            lightState = LIGHT_NULL;
                        } else {
                            WHITE_ON();
                            lightState = LIGHT_WHITE;
                        }
                    }
                }
        		break;

                //打印
                case  KEY_PRINT:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {


                } else if (keyHandleInfor.keyType == LONG_KEY) {
                    if ((ST_IDLE == instState)&&(actionState == ST_NO_ACTION)){
                        keyHandleInfor.keyCmd = CMD_PRINT;
                    }
                }
        	    break;

                //录像
                case  KEY_VIDEO:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if (ST_WORK == instState) {
                        if (ST_NO_ACTION == actionState) {
                            keyHandleInfor.keyCmd = CMD_START_RECORD;
                        } else if (ST_RECORDING == actionState) {
                            keyHandleInfor.keyCmd = CMD_STOP_RECORD;
                        }
                    }
                }
        		break;

                default:
                break;
            }
        }
        //清除键值
        keyHandleInfor.keyValue = KEY_NULL;
        keyHandleInfor.keyType = NONE_KEY;
    }
    if (keyHandleInfor.keyCmd != CMD_NULL) {
        send_key_value();   //发送命令
        //清除命令
        keyHandleInfor.keyCmd = CMD_NULL;
    }
}

/**************************************************************
*  Function Name         :   void work_task(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :
***************************************************************/
static void work_task(void)
{
    switch(instState)
    {
        case ST_IDLE:               //照片模式
            LIGHT_OFF();
            BOTH_OFF();
            if (OFFLINE == wifiState) { //主机已经关闭Sensor,WiFi断开
                instState = ST_OFFLINE;
            } else {
                if (chargeFlag) {
                    instState = ST_CHARGE;
                }
            }
        break;

        case ST_WORK:               //视频模式
            if (LIGHT_NULL == lightState) {
                LIGHT_OFF();
                BOTH_OFF();
            }
            else
                lightOn();

            if (OFFLINE == wifiState) { //主机已经关闭Sensor,WiFi断开
                instState = ST_OFFLINE;
            } else {
                if (chargeFlag) {
                    keyHandleInfor.keyCmd = CMD_PHOTO;      //发送关闭视频键值
                    instState = ST_CHARGE;
                }
            }

        break;

        case ST_CHARGE:                 //充电WiFi在线状态,光充电WiFi未在线不进入该状态
            LIGHT_OFF();
            BOTH_OFF();
            if (OFFLINE == wifiState) { //主机已经关闭Sensor,WiFi断开
                instState = ST_OFFLINE;
            } else {
                if (!chargeFlag) {
                    keyHandleInfor.keyCmd = CMD_VIDEO;      //发送视频键值
                    instState = ST_IDLE;
                }
            }
        break;

        case ST_STOP:               //进入低功耗
            sys_enterstop();
            break;

        case ST_OFFLINE:            //离线低功耗待机
            LIGHT_OFF();
            BOTH_OFF();
            if (ONLINE == wifiState) {
                if (chargeFlag) {
                    instState = ST_CHARGE;
                } else {
                    keyHandleInfor.keyCmd = CMD_VIDEO;      //发送视频键值
                    instState = ST_IDLE;
                }
            }
        break;

        case ST_WAKEUP:             //确认唤醒
            if (wakeupFlag) {
                SoftReset();
            } else if (keyHandleInfor.keyValue == KEY_NULL){
                led_off();          //先关显示
                instState = ST_STOP;
            }
        break;

        case ST_CHARGING:           //关机充电
            charge_vol();
        break;

        default:
        break;
    }
}

/**************************************************************
*  Function Name         :   void get_cell_state(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   电池状态测试
***************************************************************/
void get_cell_state(void)
{
    INT16U rrt;
    INT8U  temp;

    cellState.chargeState = CHARGE_STOP;
    if (!CHRG_STA1())
        cellState.chargeState &= 0xfe;
    if (!CHRG_STA2())
        cellState.chargeState &= 0xfd;
    cellState.voltage = cell_vol_read();
    rrt =  cell_rrt_read();                 //最高位报警位
    cellState.remainTime = rrt & 0x7fff;
    cellState.alert = rrt >> 15;
    temp = cell_soc_read();
    if (temp != cellState.remainCharge) {
        cellState.remainCharge = temp;
        send_status();    //电量变化发送
    }
    //<3.40v >3.45v 关机
    if (cellState.voltage < BATTERY_LOWEST) {
        threebuzzeorange();
        led_off();          //先关显示
        instState = ST_STOP;
	}
}

/**************************************************************
*  Function Name         :   void led_manage(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   状态指示灯刷新
***************************************************************/
void led_manage(void)
{
    //红色指示灯
    if(chargeFlag) { //充电
        if ((CHARGE_DONE == cellState.chargeState)||(cellState.remainCharge > 98)) {
            redLedDisp.ledType = LED_OFF;        //充满--红色灭
        } else {
            redLedDisp.ledType = LED_FLASH1000;  //充电中--红色秒闪
        }
    } else {
        if (selfInfor.fault) {
            redLedDisp.ledType = LED_ON;         //自检异常--红色常量
        } else if (cellState.remainCharge == 0) {     //电量低--红色100mS闪烁
            redLedDisp.ledType = LED_FLASH100;
        } else {
            redLedDisp.ledType = LED_OFF;        //正常--红色灭
        }
    }
    //绿色指示灯
    if (selfInfor.fault) {
        greenLedDisp.ledType = LED_OFF;             //自检异常--绿色灭
    } else {
        if (instState == ST_CHARGING) {
            if ((CHARGE_DONE == cellState.chargeState)||(cellState.remainCharge > 98))
                greenLedDisp.ledType = LED_ON;      //充满绿灯亮
            else
                greenLedDisp.ledType = LED_OFF;     //关机充电不亮
        } else if(actionState == ST_PRINTING) {
            greenLedDisp.ledType = LED_FLASH300;    //打印300mS闪烁
        } else if(actionState == ST_CAPTURING) {
            greenLedDisp.ledType = LED_FLASH100;    //抓拍100mS闪烁
        } else if(actionState == ST_RECORDING) {
            greenLedDisp.ledType = LED_FLASH100;    //录像100mS闪烁
        } else if(wifiState == OFFLINE) {
            greenLedDisp.ledType = LED_FLASH1000;   //WiFi未连接1S闪烁
        } else {
            greenLedDisp.ledType = LED_ON;
        }
    }
}

/**************************************************************
*  Function Name         :   void power_monitor(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   检测系统电源电压
***************************************************************/
void power_monitor(void)
{
    selfInfor.fault = 0x00;
    selfInfor.hisi3v2 = getadcvalue(HISI_3V2);
    if ((selfInfor.hisi3v2 < 2048u)||(selfInfor.hisi3v2 > 2255))        //小于3V,大于3.3V
        selfInfor.fault |= 0x01;
    selfInfor.hisi1v8 = getadcvalue(HISI_1V8);
    if ((selfInfor.hisi1v8 < 2185)||(selfInfor.hisi1v8 > 2730))         //小于1.6V,大于2.0V
        selfInfor.fault |= 0x02;
    selfInfor.hisi1v1 = getadcvalue(HISI_1V1);
    if ((selfInfor.hisi1v1 < 1228u)||(selfInfor.hisi1v1 > 1638))        //小于0.9V,大于1.2V
        selfInfor.fault |= 0x04;
    selfInfor.sensor3v2 = getadcvalue(SENSOR_3V2);
    if ((selfInfor.sensor3v2 < 2048u)||(selfInfor.sensor3v2 > 2255))    //小于3V,大于3.3V
        selfInfor.fault |= 0x08;
    selfInfor.sensor1v8 = getadcvalue(SENSOR_1V8);
    if ((selfInfor.sensor1v8 < 2185u)||(selfInfor.sensor1v8 > 2730))    //小于1.6V,大于2.0V
        selfInfor.fault |= 0x10;
}

/**************************************************************
*  Function Name         :   void duck_detect()
*  Param                 :   void
*  Return Param          :   void
*  Description           :   检测鸭嘴器的状态变化并计数保存到Flash
//OFF<----REPLACED<--work+1--ON---->OFF
                               <----OFF
***************************************************************/
void duck_detect(void)
{
    INT16U duck;

    duck = BUTTON_DUCK();           //0--on
    switch (duckState)
    {
        case DUCK_ON:
        if (duck){                  //off
            delay_ms(20);           //去抖动
            if (duck == BUTTON_DUCK())
                duckState = DUCK_OFF;
        } else {
            if (instState == ST_WORK) {
                duckCnt++;
                duckState = DUCK_REPLACED;
            }
        }
        break;

        case DUCK_OFF:
        if (!duck){
            delay_ms(20);           //去抖动
            if (duck == BUTTON_DUCK())
                duckState = DUCK_ON;     //OFF-->ON
        }
        break;

        case DUCK_REPLACED:
        if (duck){                  //off
            delay_ms(20);           //去抖动
            if (duck == BUTTON_DUCK())
                duckState = DUCK_OFF;
        }
        break;

        default:
        break;
    }
}

