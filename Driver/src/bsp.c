/***************************************************************
*  FileName              :    bsp.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    Ӳ����صײ���������
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#define    BSP_GLOBAL

#include "bsp.h"
#include "delay.h"
#include "flash.h"
#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_flash.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_wwdg.h"

UINT8 WWDG_CNT = 0x7F;

//#define WWDG_ENABLE 1

/**************************************************************
*  Function Name         :   RCC_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   rcc��ʼ������HSI
***************************************************************/
void RCC_Configer(void)
{
/*
    ErrorStatus HSEStartupStatus;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartupStatus = RCC_WaitForHSEStartUp();
	if(SUCCESS == HSEStartupStatus)												//HSE
	{
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLKConfig(RCC_HCLK_Div1);
		FLASH_SetLatency(FLASH_Latency_1);
		FLASH_PrefetchBufferCmd(ENABLE);
		RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_6);    					//6 x 8 = 48M
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(0x08 != RCC_GetSYSCLKSource());
	} else {																	//HSI
		SoftReset();															//system reset
	}
	RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
	RCC_USARTCLKConfig(RCC_USART1CLK_SYSCLK);
*/
    RCC_DeInit();
    /* enable HSI */
    RCC_HSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(ENABLE);
    /* set Flash Latency, 0-24M: FLASH_Latency_0, 24M < sysclk <48M: FLASH_Latency_1*/
    FLASH_SetLatency(FLASH_Latency_1);

    /* Configures the AHB clock (HCLK)  */
    RCC_HCLKConfig(RCC_CFGR_HPRE_DIV1);

    /* Configures the APB clock (PCLK) */
    RCC_PCLKConfig(RCC_CFGR_PPRE_DIV1);

    /* Configures PLL source, (8M/2) * 12 = 48M */
    //RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_CFGR_PLLMULL12);
    //RCC_PLLCmd(ENABLE);
    //while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Configures system clock source */
    //RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    /* Wait till PLL is used as system clock source */
    //while ((uint32_t)RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL);

	RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
	RCC_USARTCLKConfig(RCC_USART1CLK_SYSCLK);
}



/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | \
						  RCC_AHBPeriph_GPIOC | \
						  RCC_AHBPeriph_GPIOF, ENABLE);

    //����IN    KEY_MAIN--PA0,KEY_ZOOM_OUT--PA1,BUTTON_DUCK--PA15,USB_IN--PA11
    //GC65_NETLIGHT--PA12
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_15 | GPIO_Pin_12 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    //����IN    KEY_DOWN--PC13,KEY_UP--PC14,KEY_ZOOM_IN--PC15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&GPIO_InitStruct);

    //����IN    KEY_VIDEO--PB8,KEY_PRINT--PB9
    //CHRG_STA1--PB14,CHRG_STA2--PB13
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    //����IN   (SENSOR_PWDN--PF0) ,KEY_LED--PF7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOF,&GPIO_InitStruct);

    //��ʼ��LED
    LED_RED_OFF();
    LED_GREEN_OFF();
	//OUT   LED_RED--PB1,LED_GREEN--PB2
	//OUT   MB_POWER--PB12,CHARGE--PB10,SENSOR_POWER--PB11,LED_WHITE--PB4,LED_BLUE--PB3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | \
	                           GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    //OUT   GC65_POWER--PB15
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    //OUT   GC65_PWRKEY--PF6
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;             //OC OUTPUT
    GPIO_Init(GPIOF,&GPIO_InitStruct);

	//adc�ɼ���HISI3V2--PA6,HISI1V8--PA7,HISI1V1--PB0,VI3V2--PA4,VI1V8--PA5
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//adc�ɼ���HISI1V1--PB0
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

    //UART1 �ܽ����ã�����δ�ϵ���Ҫ����
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    //USART2 �ܽ�����,2Gδ�ϵ���Ҫ����
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    //LED_PWM TIM3_CH2
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_1);

    //BELL_PWM TIM1_CH1
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_2);

    //CW2015 I2C1
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;          //��IOģ��
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/**************************************************************
*  Function Name         :   adcinit
*  Param                 :   void
*  Return Param          :   void
*  Description           :   adc��ʼ������
***************************************************************/
void ADC_Configer(void)
{
	ADC_InitTypeDef  ADC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	ADC_StructInit(&ADC_InitStruct);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	//ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //�ر�����ת��ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                      //�ر�����ת��ģʽ
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1,&ADC_InitStruct);

	ADC_GetCalibrationFactor(ADC1);	                                      //ADCУ׼

	//ADC_StopOfConversion(ADC1);
	ADC_Cmd(ADC1,ENABLE);
}

/**************************************************************
*  Function Name         :   rtc_confige
*  Param                 :   UINT8 flag, DATASTRUCT * datastruct
*  Return Param          :   void
*  Description           :   rtc��ʼ������,LSI
***************************************************************/
void Rtc_Configer(void)
{
	RTC_InitTypeDef RTC_InitStruct;

	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_BKP_VALUE) {	//ϵͳ������

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		PWR_BackupAccessCmd(ENABLE);

		RCC_LSICmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);                  //����RTC
		RTC_WaitForSynchro();                   //�ȴ�RTCʱ����APB1ʱ��ͬ��,���ܶ�д�Ĵ���
    	RTC_InitStruct.RTC_AsynchPrediv = 0x63;
	    RTC_InitStruct.RTC_SynchPrediv = 0x18F;

    	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;

		if(RTC_Init(&RTC_InitStruct) == ERROR)
		{
		}
	}
}
/**************************************************************
*  Function Name         :   RTC_WakeUp_Configer
*  Param                 :   INT8U sec
*  Return Param          :   void
*  Description           :   rtc��ʱ��������
***************************************************************/
void RTC_WakeUp_Configer(INT8U sec)
{
    RTC_AlarmTypeDef  RTC_AlarmStructure;
    RTC_TimeTypeDef   RTC_TimeStructure;

    /* Disable the Alarm A */
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

    /* Get the current time */
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

    /* Set the alarm to current time + 3s */
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_TimeStructure.RTC_H12;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = RTC_TimeStructure.RTC_Hours;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = (RTC_TimeStructure.RTC_Seconds + sec) % 60;
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = 31;
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

    /* Enable RTC Alarm A Interrupt: this Interrupt will wake-up the system from
    STANDBY mode (RTC Alarm IT not enabled in NVIC) */
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);

    /* Enable the Alarm A */
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    /* Clear RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);
}

/**************************************************************
*  Function Name         :   Timer_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   timer��ʱ����ʼ������
***************************************************************/
void Timer_Configer(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef  TIM_OCInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;

    //TIM14,TIM15����
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //23.8kHz��LED��������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);         //4kHz��BUZZER
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);		//100ms��ͨ�ö�ʱ��Ӧ���ѯ
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17,ENABLE);        //20ms��������

    TIM_DeInit(TIM3);
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM16);
	TIM_DeInit(TIM17);

    //BUZZER 4KHz
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1;         //4M��Ƶ2��Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 999;                   //ARR����4k
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	//TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 500;                          //CCRռ�ձ�
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);                        //CH1
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1,ENABLE);

    //LED�������ȣ�23.8kHz
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 20; //500/21=23.8kHz
    TIM_TimeBaseInitStruct.TIM_Prescaler = 16; //500K=8M/16
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	//TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 12;                          //CCRռ�ձ�
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);

    /*
    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0; //���ȼ�Ϊ0
    NVIC_Init(&NVIC_InitStruct);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	//ʹ���ж�֮ǰ�����־
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    */


	//100ms��ʱ��,���ڼ�ʱ
	TIM_TimeBaseInitStruct.TIM_Prescaler = 39;     // 200K=8M/40
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInitStruct.TIM_Period = 19999;     // 100ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM16,&TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM16,DISABLE);

    //��ʱ�ж����ȼ�3
	NVIC_InitStruct.NVIC_IRQChannel = TIM16_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	TIM_ClearITPendingBit(TIM16,TIM_IT_Update);
    TIM_ITConfig(TIM16,TIM_IT_Update,ENABLE);


	//20ms��ʱ��,���ڰ���
	TIM_TimeBaseInitStruct.TIM_Prescaler = 39;    // 200K=8M/40
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInitStruct.TIM_Period = 3999;     // 20ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM17,&TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM17,DISABLE);

    //�����ж����ȼ�1
	NVIC_InitStruct.NVIC_IRQChannel = TIM17_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	TIM_ClearITPendingBit(TIM17,TIM_IT_Update);
    TIM_ITConfig(TIM17,TIM_IT_Update,ENABLE);
}

/**************************************************************
*  Function Name         :   Key_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ������ʼ��
***************************************************************/
void Key_Configer(void)
{
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

    EXTI_DeInit();

    //KEY_MAIN EXTI_Line0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line0);
    EXTI_ClearITPendingBit(EXTI_Line0);

	//KEY_ZOOM_OUT EXTI_Line1
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource1);
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line1);
    EXTI_ClearITPendingBit(EXTI_Line1);

    //KEY_ZOOM_IN EXTI_Line15
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource15);
	EXTI_InitStruct.EXTI_Line = EXTI_Line15;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line15);
    EXTI_ClearITPendingBit(EXTI_Line15);

	//KEY_UP EXTI_Line14
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource14);
	EXTI_InitStruct.EXTI_Line = EXTI_Line14;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line14);
    EXTI_ClearITPendingBit(EXTI_Line14);

    //KEY_DOWN EXTI_Line13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line13);
    EXTI_ClearITPendingBit(EXTI_Line13);

    //KEY_LED EXTI_Line7
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource7);
    EXTI_InitStruct.EXTI_Line = EXTI_Line7;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line7);
    EXTI_ClearITPendingBit(EXTI_Line7);

    //KEY_PRINT EXTI_Line9
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource9);
    EXTI_InitStruct.EXTI_Line = EXTI_Line9;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line9);
    EXTI_ClearITPendingBit(EXTI_Line9);

    //KEY_LED EXTI_Line8
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource8);
    EXTI_InitStruct.EXTI_Line = EXTI_Line8;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line8);
    EXTI_ClearITPendingBit(EXTI_Line8);

    //USB_IN EXTI_Line11
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource11);
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line11);
    EXTI_ClearITPendingBit(EXTI_Line11);

    //�ж����ȼ�2
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
    //�ж����ȼ�2
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/**************************************************************
*  Function Name         :   Wakeup_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   �����ж�����
***************************************************************/
void Wakeup_Configer(void)
{
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    EXTI_DeInit();

    //KEY_MAIN EXTI_Line0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line0);
    EXTI_ClearITPendingBit(EXTI_Line0);

    //USB_IN EXTI_Line11
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource11);
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line11);
    EXTI_ClearITPendingBit(EXTI_Line11);

    //�ж����ȼ�2
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}



/***************************************************************
*  Function Name         :   GCopenpin_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   2Gģ�������жϼ��
***************************************************************/
void GCopenpin_Configer(volatile UINT8 mode)
{
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;

	EXTI_ClearITPendingBit(EXTI_Line12);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource12);

	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //�����ػ���
	if (mode)
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	else
		EXTI_InitStruct.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStruct);
    EXTI_ClearFlag(EXTI_Line12);
    EXTI_ClearITPendingBit(EXTI_Line12);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 3;        //USB����ж����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

/**************************************************************
*  Function Name         :   Uart1gpio_Normal
*  Param                 :   void
*  Return Param          :   void
*  Description           :   uart��ʼ������
***************************************************************/
void Uart1gpio_Normal(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**************************************************************
*  Function Name         :   Uart1_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   uart��ʼ������
***************************************************************/
void Uart1_Configer(void)
{
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);

    USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStruct);
    //����ͨ���ж����ȼ�0
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;               //uartͨ���ж����ȼ�Ӧ��Ϊ���
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);

	USART_OverrunDetectionConfig(USART1, USART_OVRDetection_Disable);

	USART_Cmd(USART1,ENABLE);
}


/**************************************************************
*  Function Name         :   Uart2gpio_Normal
*  Param                 :   void
*  Return Param          :   void
*  Description           :   uart��ʼ������
***************************************************************/
void Uart2gpio_Normal(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
}


/**************************************************************
*  Function Name         :   Uart_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   uart��ʼ������
***************************************************************/
void Uart2_Configer(void)
{
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);

    USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0;               //uartͨ���ж����ȼ�Ӧ��Ϊ���
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);

	USART_OverrunDetectionConfig(USART2, USART_OVRDetection_Disable);

	USART_Cmd(USART2,DISABLE);
}


/***************************************************************
*  Function Name         :   ResetMode_Test
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ���ϵͳ��λ
***************************************************************/
void ResetMode_Test(void)
{
	if (RCC->CSR & 0x10000000)	//software reset
	{
		reset_flag = 1;
	}
	RCC->CSR |= RCC_CSR_RMVF;
}

/**************************************************************
*  Function Name         :   WWDG_NVIC_Init
*  Param                 :   void
*  Return Param          :   void
*  Description           :   ���Ź��ж�����
***************************************************************/
void WWDG_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**************************************************************
* ��ʼ�����ڿ��Ź�
* tr    : T[6:0], ������ֵ
* wr    : W[6:0], ����ֵ
* fprer : ��Ƶϵ��(WDGTB), �����2λ��Ч
* Fwwdg = PCLK1 / (4096*2^fprer)
***************************************************************/
void wwdg_init(UINT8 tr, UINT8 wr, UINT32 fprer)
{
    #ifdef WWDG_ENABLE
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);    // WWDGʱ��ʹ��

    WWDG_CNT = tr & WWDG_CNT;   // ��ʼ��WWDG_CNT

    /* ���Ź�����=(48MHz(PCLK1)/4096)/8 = 1464Hz */
    /* Ҳ����˵1���Ӽ�������1464�� */
    /* ���� T=1/1464 s = 0.68ms */
    WWDG_SetPrescaler(fprer);   // ����IWDGԤ��Ƶֵ

    /* ����ֵ��wr , ����˵����������0x7F, �ڼ���wr֮ǰι���˾���ι����--�Ḵλ */
    /* wrֵһ��Ҫ����0x40, ���򴰿�ֵ�Ͳ�������, ��Ϊ�������� 0x40����0x3F��������Ź���λ */
    WWDG_SetWindowValue(wr);    // ���ô���ֵ

    /* ���Ź�ʹ��, ����ʼ����ʱ��Ϊ0x7F ����������0x3Fʱ(T6λ����), �����һ����λ*/
    WWDG_Enable(WWDG_CNT);      // ʹ�ܿ��Ź�, ���� counter

    WWDG_ClearFlag();           // �����ǰ�����жϱ�־λ

    //WWDG_NVIC_Init();           // ��ʼ�����ڿ��Ź� NVIC

    /* ʹ��EW interrupt  ����ڼ���������0x40��ʱ�����һ��"��ǰ"�ж�,
     * ��ȻҲ����������ж���Ͻ�ι��, ʡ�ù�ҧ��(��λ) */
    //WWDG_EnableIT();            // �������ڿ��Ź��ж�
    #endif
}


/**************************************************************
*  Function Name         :   feedwwdg
*  Param                 :   void
*  Return Param          :   void
*  Description           :   �жϴ���ֵ�ͼ�����ֵ��ִ��ι��
***************************************************************/
void feedwwdg(void)
{
    #ifdef WWDG_ENABLE
    volatile UINT8 tr = 0;
    volatile UINT8 wr = 0;

    wr = WWDG->CFR & 0X7F;      // ����ֵ
    tr = WWDG->CR & 0X7F;       // ������ֵ

    //CLI();
    /* ע��tr��wr�ıȽϣ�ȷ��ι��ʱ�� */
    if(tr<wr)   // ������ֵtr����С�ڴ���ֵwrʱ����ι������֮ǰι����̫�磬��������Ź���λ
    {
        WWDG_SetCounter(WWDG_CNT);
    }
    //SEI();
    #endif
}


/**************************************************************
*  Function Name         :   Lowpower_Configer
*  Param                 :   void
*  Return Param          :   void
*  Description           :   stopģʽ��gpio״̬����
***************************************************************/
void Lowpower_Configer(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB |  \
                          RCC_AHBPeriph_GPIOC |  \
                          RCC_AHBPeriph_GPIOF, ENABLE);
    /*****************PULL UP************************/
	//����IN    KEY_MAIN--PA0,KEY_ZOOM_OUT--PA1,BUTTON_DUCK--PA15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

	//LED_R--PB1 LED_G--PB2 CW2015 I2C --PB6--PB7 KEY_VIDEO--PB8,KEY_PRINT--PB9
    //CHRG_STA1--PB14,CHRG_STA2--PB13
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7 |  \
	                           GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

	//����IN    KEY_DOWN--PC13,KEY_UP--PC14,KEY_ZOOM_IN--PC15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC,&GPIO_InitStruct);

	//����IN    KEY_LED--PF7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF,&GPIO_InitStruct);

    /*****************PULL DOWN************************/
	//other OUT PA
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
	                           GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | \
	                           GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//other OUT PB
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
	                           GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

	//other PF
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOF,&GPIO_InitStruct);
}

