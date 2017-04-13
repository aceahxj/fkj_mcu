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
*  Description           :    ϵͳ���ƺ���
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
*  Description           :   ϵͳ�ϵ���
***************************************************************/
void PowerOn_Test(void)
{
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_BKP_VALUE) {
        RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_BKP_VALUE);
		instState = ST_OFFLINE;//instState = ST_STOP;;	//ϵͳ���縴λ׼������Stop
	} else {
		instState = ST_OFFLINE;  //���Ź���λ��ת������״̬
	}
}

/**************************************************************
*  Function Name         :   void Charge_Test(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ϵͳ�ϵ�����
***************************************************************/
void Charge_Test(void)
{
	if (USB_IN()) {				                    //���������
        delay_ms(100);
        if (USB_IN()) {                             //ȷ�ϲ���USB
            #if 0
            if (CHARGE_STOP == cellState.chargeState) {
                //��ص�������û�н����磬����ػ������
                delay_ms(50);
				if (USB_IN()) {                     //�ٴ�ȷ�ϲ���USB
					CHARGE_OFF();
					RTC_WakeUp_Configer(10);        //��ʱ10�뻽��
					instState = ST_STOP;
				}
            }
            #endif
            chargeFlag = 1;
        }
	} else {
        delay_ms(100);
        if (!USB_IN()) {                            //ȷ�ϰγ�USB
            chargeFlag = 0;
        }
	}
}



/**************************************************************
*  Function Name         :   void Reset_Init(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ��λ��ʼ��
***************************************************************/
void Reset_Init(void)
{
    //��ʼ��LED
    LED_RED_OFF();
    LED_GREEN_OFF();
    //״ָ̬ʾ��
    redLedDisp.ledType = LED_OFF;
    redLedDisp.state = LED_OFF;
    redLedDisp.timeCount = 0;
    greenLedDisp.ledType = LED_OFF;
    greenLedDisp.state = LED_OFF;
    greenLedDisp.timeCount = 0;
    //�ع��գ�Ĭ�ϰ׹�
    LIGHT_OFF();
    BOTH_OFF();
    lightState = LIGHT_WHITE;
    wifiState = OFFLINE;
    duckState = DUCK_ON;
    //MB_POWER_OFF();
    GC65_POWER_OFF();
    GC65_PWRKEY_OFF();
    //SENSOR_POWER_OFF();
    TIM_Cmd(TIM16,ENABLE);      //100mS��ʱ����
}

static void cell_vol_test(void)
{
    cellState.voltage = cell_vol_read();
    if (cellState.voltage < BATTERY_LOWEST) {
        led_off();          //�ȹ���ʾ
        instState = ST_STOP;
	} else {
	    instState = ST_OFFLINE;
	}
}

/**************************************************************
*  Function Name         :   void reset_system(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ϵͳ��ʼ��
***************************************************************/
void reset_system(void)
{
	mydelayms(10);
    RCC_Configer();
	//wwdg_init(0x7F, 0x50, WWDG_Prescaler_8);
    delay_init(8);             //ϵͳ�ڲ�ʱ��8MHz,δ��Ƶ
    GPIO_Configuration();
    CHARGE_ON();
    MB_POWER_ON();             //���������Դ���ȴ���������
    SENSOR_POWER_ON();         //����Sensor��Դ��׼��������Ƶģʽ
	//wwdg_init(0x7F, 0x50, WWDG_Prescaler_8);
    ADC_Configer();
    Key_Configer();
    Uart1_Configer();
    cw_alert_config(1);         //��������0%����
    cw_mode_set(CW_WAKEUP);     //���ѵ�����
    cell_vol_test();            //��ص�ѹ����
    Timer_Configer();
    Rtc_Configer();

    onelongbuzze();             //��������
	//ResetMode_Test();
	Reset_Init();
    PowerOn_Test();    //����Ƿ���������ػ��ǿ��Ź���λ
    Charge_Test();
}

/**************************************************************
*  Function Name         :   void key_open_system(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ����main����ϵͳ
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
*  Description           :   ��ʱ����Ԥ���
***************************************************************/
void pre_charge_init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    //��ֹMain�������ж�
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStruct);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	//USB�����ѹ���ڿ������
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
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;          //��IOģ��
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    cw_mode_set(CW_WAKEUP);               //���ѵ�����
    TIM_Cmd(TIM16,ENABLE);                //100mS��ʱ����
}

/**************************************************************
*  Function Name         :   void stop_with_red_on(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   Stopʱ�����
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
*  Description           :   ���Ԥ����ѹ����
***************************************************************/
void charge_vol(void)
{
    cellState.chargeState = CHARGE_STOP;
    if (!CHRG_STA1())
        cellState.chargeState &= 0xfe;
    if (!CHRG_STA2())
        cellState.chargeState &= 0xfd;
    cellState.remainTime = cell_rrt_read()&0x7fff;  //���λ����λ
    cellState.voltage = cell_vol_read();
    #if 0
    wakeupFlag = 0;
    if (cellState.voltage < PRE_CHARGE_V1) {
        RTC_WakeUp_Configer(59);   //��ʱ59�뻽��
	} else if (cellState.voltage < PRE_CHARGE_V2) {
        RTC_WakeUp_Configer(30);   //��ʱ30�뻽��
	} else if (cellState.voltage < PRE_CHARGE_V3) {
        RTC_WakeUp_Configer(20);   //��ʱ20�뻽��
	} else if (cellState.voltage < PRE_CHARGE_V4) {
        RTC_WakeUp_Configer(10);   //��ʱ10�뻽��
	} else if (cellState.voltage < PRE_CHARGE_V5) {
        RTC_WakeUp_Configer(5);   //��ʱ5�뻽��
	} else {
	    wakeupFlag = 1;
	}
    #endif
}

/**************************************************************
*  Function Name         :   void lightOn(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ����Դ
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
*  Description           :   ����ͣ��ǰ�ȹ�LED��ʾ
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
*  Description           :   ϵͳ����stopģʽ
***************************************************************/
void sys_enterstop(void)
{
	GC65_POWER_OFF();
	TIM_Cmd(TIM1,DISABLE);          //�رն�ʱ��
	TIM_Cmd(TIM3,DISABLE);
    TIM_Cmd(TIM16,DISABLE);
    TIM_Cmd(TIM17,DISABLE);
	USART_Cmd(USART1,DISABLE);      //�رմ���
	USART_Cmd(USART2,DISABLE);
    ADC_Cmd(ADC1,DISABLE);          //�ر�adc
    MB_POWER_OFF();
    SENSOR_POWER_OFF();
    cw_mode_set(CW_SLEEP);          //����������
	Lowpower_Configer();            //����stopǰ��������IO��
	Wakeup_Configer();              //���û���
	if (1 == chargeFlag) {
        pre_charge_init();
        instState = ST_CHARGING;    //�ػ����
	} else {
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    	WWDG_DeInit();
        PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//����͹���ģʽ
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
        if (waitReply.timeCount > 30) {           //��ʱ3��
            selfInfor.errorCode = HISI_COMM_ERR;
            waitReply.replyFlag = FALSE;
        }
    }
    if (waitAction.replyFlag) {
        if((waitAction.cmd == PHOTO_CTRL)&&(waitAction.timeCount > 30)) {     //���ճ�ʱ3��
            actionState = ST_NO_ACTION;
            waitAction.replyFlag = FALSE;
        }
        if((waitAction.cmd == PRINT_CTRL)&&(waitAction.timeCount > 300)) {    //���ճ�ʱ30��
            actionState = ST_NO_ACTION;
            waitAction.replyFlag = FALSE;
        }
    }
}

/**************************************************************
*  Function Name         :   void systemtask(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ϵͳ������
***************************************************************/
void system_task(void)
{
    if ((instState != ST_CHARGING)&&(instState != ST_STOP)) {
        //��������
        key_task();
        //�������ݴ���
        checkuartpacket();
        get_cell_state();
        //�ȴ�HiSiӦ��
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
*  Description           :   ����ռ�ձ�
***************************************************************/
void LED_pulse(INT8U upDown)
{
    if (DOWN == upDown) {
        ledPulse--;                          //CCRռ�ձ�
    } else {
        ledPulse++;                          //CCRռ�ձ�
    }
    if ((ledPulse > 4)&&(ledPulse < 17)) {   //5~16
        TIM3->CCR2 = ledPulse;
    } else {
        //�ָ������Χ��
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
*  Description           :   ��λ���оƬ
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
*  Description           :   �����������ڷ��ͼ�ֵ��HiSi
***************************************************************/
static void key_task(void)
{
    if (keyHandleInfor.keyType != NONE_KEY) {
        if ((ONLINE == wifiState)||(KEY_MAIN == keyHandleInfor.keyValue)) {
            switch (keyHandleInfor.keyValue)
            {
                //����
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
                        wakeupFlag = 1;             //����
                    } else {
                        if (ONLINE == wifiState)
                            keyHandleInfor.keyCmd = CMD_SHUT_DOWN;
                        else
                            twoshortbuzze();
                            led_off();          //�ȹ���ʾ
                            instState = ST_STOP;
                    }
                }
                break;

                //��Ƶ����Ƭ�Ŵ�
        		case KEY_ZOOM_OUT:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if ((ST_WORK == instState)||(ST_IDLE == instState)) {
                        keyHandleInfor.keyCmd = CMD_ZOOM_OUT;
                    }
                } else if (keyHandleInfor.keyType == LONG_KEY) {
                    if (ST_IDLE == instState) {
                        keyHandleInfor.keyCmd = CMD_VIDEO;      //�л�����Ƶģʽ
                    } else if (ST_WORK == instState) {
                        keyHandleInfor.keyCmd = CMD_PHOTO;      //�л�����Ƭģʽ
                    }
                }
        		break;

                //��Ƶ����Ƭ��С
                case  KEY_ZOOM_IN:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {
                    if ((ST_WORK == instState)||(ST_IDLE == instState)) {
                        keyHandleInfor.keyCmd = CMD_ZOOM_IN;
                    }
                } else if (keyHandleInfor.keyType == LONG_KEY) {

                }
                break;

                //��Ƭ�Ϸ�
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

                //��Ƭ�·�
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

                //�����л�LED����
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

                //��ӡ
                case  KEY_PRINT:
                if (keyHandleInfor.keyType == SIGNLE_KEY) {


                } else if (keyHandleInfor.keyType == LONG_KEY) {
                    if ((ST_IDLE == instState)&&(actionState == ST_NO_ACTION)){
                        keyHandleInfor.keyCmd = CMD_PRINT;
                    }
                }
        	    break;

                //¼��
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
        //�����ֵ
        keyHandleInfor.keyValue = KEY_NULL;
        keyHandleInfor.keyType = NONE_KEY;
    }
    if (keyHandleInfor.keyCmd != CMD_NULL) {
        send_key_value();   //��������
        //�������
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
        case ST_IDLE:               //��Ƭģʽ
            LIGHT_OFF();
            BOTH_OFF();
            if (OFFLINE == wifiState) { //�����Ѿ��ر�Sensor,WiFi�Ͽ�
                instState = ST_OFFLINE;
            } else {
                if (chargeFlag) {
                    instState = ST_CHARGE;
                }
            }
        break;

        case ST_WORK:               //��Ƶģʽ
            if (LIGHT_NULL == lightState) {
                LIGHT_OFF();
                BOTH_OFF();
            }
            else
                lightOn();

            if (OFFLINE == wifiState) { //�����Ѿ��ر�Sensor,WiFi�Ͽ�
                instState = ST_OFFLINE;
            } else {
                if (chargeFlag) {
                    keyHandleInfor.keyCmd = CMD_PHOTO;      //���͹ر���Ƶ��ֵ
                    instState = ST_CHARGE;
                }
            }

        break;

        case ST_CHARGE:                 //���WiFi����״̬,����WiFiδ���߲������״̬
            LIGHT_OFF();
            BOTH_OFF();
            if (OFFLINE == wifiState) { //�����Ѿ��ر�Sensor,WiFi�Ͽ�
                instState = ST_OFFLINE;
            } else {
                if (!chargeFlag) {
                    keyHandleInfor.keyCmd = CMD_VIDEO;      //������Ƶ��ֵ
                    instState = ST_IDLE;
                }
            }
        break;

        case ST_STOP:               //����͹���
            sys_enterstop();
            break;

        case ST_OFFLINE:            //���ߵ͹��Ĵ���
            LIGHT_OFF();
            BOTH_OFF();
            if (ONLINE == wifiState) {
                if (chargeFlag) {
                    instState = ST_CHARGE;
                } else {
                    keyHandleInfor.keyCmd = CMD_VIDEO;      //������Ƶ��ֵ
                    instState = ST_IDLE;
                }
            }
        break;

        case ST_WAKEUP:             //ȷ�ϻ���
            if (wakeupFlag) {
                SoftReset();
            } else if (keyHandleInfor.keyValue == KEY_NULL){
                led_off();          //�ȹ���ʾ
                instState = ST_STOP;
            }
        break;

        case ST_CHARGING:           //�ػ����
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
*  Description           :   ���״̬����
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
    rrt =  cell_rrt_read();                 //���λ����λ
    cellState.remainTime = rrt & 0x7fff;
    cellState.alert = rrt >> 15;
    temp = cell_soc_read();
    if (temp != cellState.remainCharge) {
        cellState.remainCharge = temp;
        send_status();    //�����仯����
    }
    //<3.40v >3.45v �ػ�
    if (cellState.voltage < BATTERY_LOWEST) {
        threebuzzeorange();
        led_off();          //�ȹ���ʾ
        instState = ST_STOP;
	}
}

/**************************************************************
*  Function Name         :   void led_manage(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ״ָ̬ʾ��ˢ��
***************************************************************/
void led_manage(void)
{
    //��ɫָʾ��
    if(chargeFlag) { //���
        if ((CHARGE_DONE == cellState.chargeState)||(cellState.remainCharge > 98)) {
            redLedDisp.ledType = LED_OFF;        //����--��ɫ��
        } else {
            redLedDisp.ledType = LED_FLASH1000;  //�����--��ɫ����
        }
    } else {
        if (selfInfor.fault) {
            redLedDisp.ledType = LED_ON;         //�Լ��쳣--��ɫ����
        } else if (cellState.remainCharge == 0) {     //������--��ɫ100mS��˸
            redLedDisp.ledType = LED_FLASH100;
        } else {
            redLedDisp.ledType = LED_OFF;        //����--��ɫ��
        }
    }
    //��ɫָʾ��
    if (selfInfor.fault) {
        greenLedDisp.ledType = LED_OFF;             //�Լ��쳣--��ɫ��
    } else {
        if (instState == ST_CHARGING) {
            if ((CHARGE_DONE == cellState.chargeState)||(cellState.remainCharge > 98))
                greenLedDisp.ledType = LED_ON;      //�����̵���
            else
                greenLedDisp.ledType = LED_OFF;     //�ػ���粻��
        } else if(actionState == ST_PRINTING) {
            greenLedDisp.ledType = LED_FLASH300;    //��ӡ300mS��˸
        } else if(actionState == ST_CAPTURING) {
            greenLedDisp.ledType = LED_FLASH100;    //ץ��100mS��˸
        } else if(actionState == ST_RECORDING) {
            greenLedDisp.ledType = LED_FLASH100;    //¼��100mS��˸
        } else if(wifiState == OFFLINE) {
            greenLedDisp.ledType = LED_FLASH1000;   //WiFiδ����1S��˸
        } else {
            greenLedDisp.ledType = LED_ON;
        }
    }
}

/**************************************************************
*  Function Name         :   void power_monitor(void)
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ���ϵͳ��Դ��ѹ
***************************************************************/
void power_monitor(void)
{
    selfInfor.fault = 0x00;
    selfInfor.hisi3v2 = getadcvalue(HISI_3V2);
    if ((selfInfor.hisi3v2 < 2048u)||(selfInfor.hisi3v2 > 2255))        //С��3V,����3.3V
        selfInfor.fault |= 0x01;
    selfInfor.hisi1v8 = getadcvalue(HISI_1V8);
    if ((selfInfor.hisi1v8 < 2185)||(selfInfor.hisi1v8 > 2730))         //С��1.6V,����2.0V
        selfInfor.fault |= 0x02;
    selfInfor.hisi1v1 = getadcvalue(HISI_1V1);
    if ((selfInfor.hisi1v1 < 1228u)||(selfInfor.hisi1v1 > 1638))        //С��0.9V,����1.2V
        selfInfor.fault |= 0x04;
    selfInfor.sensor3v2 = getadcvalue(SENSOR_3V2);
    if ((selfInfor.sensor3v2 < 2048u)||(selfInfor.sensor3v2 > 2255))    //С��3V,����3.3V
        selfInfor.fault |= 0x08;
    selfInfor.sensor1v8 = getadcvalue(SENSOR_1V8);
    if ((selfInfor.sensor1v8 < 2185u)||(selfInfor.sensor1v8 > 2730))    //С��1.6V,����2.0V
        selfInfor.fault |= 0x10;
}

/**************************************************************
*  Function Name         :   void duck_detect()
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ���Ѽ������״̬�仯���������浽Flash
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
            delay_ms(20);           //ȥ����
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
            delay_ms(20);           //ȥ����
            if (duck == BUTTON_DUCK())
                duckState = DUCK_ON;     //OFF-->ON
        }
        break;

        case DUCK_REPLACED:
        if (duck){                  //off
            delay_ms(20);           //ȥ����
            if (duck == BUTTON_DUCK())
                duckState = DUCK_OFF;
        }
        break;

        default:
        break;
    }
}

