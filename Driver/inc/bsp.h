/***************************************************************
*  FileName              :    bsp.h
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    Ӳ����صײ�ͷ�ļ�
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

#ifndef  _BSP_H
#define  _BSP_H

#ifdef     BSP_GLOBAL
#define    BSP_EXT
#else
#define    BSP_EXT    extern
#endif


#include "lib_type.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

#define		RTC_BKP_VALUE  0xAA5555AA

//��������
#define KEY_MAIN()              (GPIOA->IDR  & GPIO_Pin_0)      //PA0
#define KEY_ZOOM_OUT()          (GPIOA->IDR  & GPIO_Pin_1)      //PA1
#define KEY_ZOOM_IN()           (GPIOC->IDR  & GPIO_Pin_15)     //PC15
#define KEY_UP()                (GPIOC->IDR  & GPIO_Pin_14)     //PC14
#define KEY_DOWN()              (GPIOC->IDR  & GPIO_Pin_13)     //PC13
#define KEY_LED()               (GPIOF->IDR  & GPIO_Pin_7)      //PF7
#define KEY_PRINT()             (GPIOB->IDR  & GPIO_Pin_9)      //PB9
#define KEY_VIDEO()             (GPIOB->IDR  & GPIO_Pin_8)      //PB8
#define BUTTON_DUCK()           (GPIOA->IDR  & GPIO_Pin_15)     //PA15����ʱ��ѯ�������ٶȲ���

#define GC65_NETLIGHT()         (GPIOA->IDR  & GPIO_Pin_12)     //PA12��2Gģ�鿪���ɹ�Netlight�ź�
#define CHRG_STA1()             (GPIOB->IDR  & GPIO_Pin_14)     //PB14
#define CHRG_STA2()             (GPIOB->IDR  & GPIO_Pin_13)     //PB13
#define USB_IN()               (GPIOA->IDR  & GPIO_Pin_11)      //PA11��1--��磬0--δ��
#define SENSOR_EN()             (GPIOF->IDR  & GPIO_Pin_0)      //PF0��0--�رգ�1--����

//�����Դ
#define MB_POWER_OFF()              GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET)
#define MB_POWER_ON()               GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET)

//��翪��
#define CHARGE_ON()		  	        GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET)
#define CHARGE_OFF()		  	    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET)

//2G��Դ����,�ϵ絽PWRKEY������С��30mS
#define GC65_POWER_OFF()            GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET)
#define GC65_POWER_ON()             GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET)
#define GC65_PWRKEY_ON()            GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_RESET)         //OC���
#define GC65_PWRKEY_OFF()           GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_SET)

//Sensor��Դ����
#define SENSOR_POWER_OFF()          GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET)
#define SENSOR_POWER_ON()           GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET)

//LED_RED״̬��ʾ
#define LED_RED_ON()                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET)
#define LED_RED_OFF()               GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET)

//LED_GREEN״̬��ʾ
#define LED_GREEN_ON()              GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_RESET)
#define LED_GREEN_OFF()             GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_SET)

//BUZZER���� TIM1_CH1
#define BUZZER_ON()                 (TIM1->CCER |= TIM_CCER_CC1E)
#define BUZZER_OFF()                (TIM1->CCER &= ~TIM_CCER_CC1E)

//����LED���ȿ��� TIM3_CH2
#define LIGHT_ON()                  (TIM3->CCER |= TIM_CCER_CC2E)
#define LIGHT_OFF()                 (TIM3->CCER &= ~TIM_CCER_CC2E)

//LED��ɫ����
#define WHITE_ON()                  do {GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);\
                                        GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);} while(0)
#define BLUE_ON()                   do {GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);\
                                        GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);} while(0)
#define BOTH_ON()                   do {GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);\
                                        GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);} while(0)
#define BOTH_OFF()                  do {GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);\
                                        GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);} while(0)

#define DOWN        0
#define UP          1


#define CLI() __set_PRIMASK(1)              //�ر������ж�
#define SEI() __set_PRIMASK(0)              //�������ж�



#ifdef     BSP_GLOBAL
BSP_EXT  UINT8    reset_flag = 0;         //��λ��ʽ0-�临λ,1-���Ź��ȸ�λ
BSP_EXT  UINT8    sysenstop = 0;
#else
BSP_EXT  UINT8    reset_flag;
BSP_EXT  UINT8    sysenstop;
#endif

void RCC_Configer(void);
void Rtc_Configer(void);
void RTC_WakeUp_Configer(INT8U sec);
void Timer_Configer(void);
void Key_Configer(void);
void Wakeup_Configer(void);
void Uart1gpio_Normal(void);
void Uart1_Configer(void);
void Uart2gpio_Normal(void);
void Uart2_Configer(void);
void Lowpower_Configer(void);
void GPIO_Configuration(void);
void ADC_Configer(void);
void GCopenpin_Configer(volatile UINT8 mode);
void wwdg_init(UINT8 tr, UINT8 wr, UINT32 fprer);
void feedwwdg(void);
void ResetMode_Test(void);
void LED_pulse(INT8U upDown);



#endif
