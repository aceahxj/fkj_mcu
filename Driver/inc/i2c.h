/***************************************************************
*  FileName              :  i2c.h
*  Copyright             :  e-LinkCare Medical Inc.
*  ModuleName            :
*
*  CPU                   :  STM32F103V8T6
*  RTOS                  :
*
*  CreateData            :  2016-04-26
*  Author/Corporation    :

*
*  Description           :  IOÄ£ÄâI2C
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

#ifndef I2C_H
#define I2C_H

#ifdef      I2C_GLOBAL
#define     I2C_H_EXT
#else
#define     I2C_H_EXT    extern
#endif

#include "stm32f0xx.h"
#include  "lib_type.h"

#define SCL_HIGH()           GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define SCL_LOW()            GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define SDA_HIGH()           GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define SDA_LOW()            GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define SDA_IN()             GPIOB->IDR  & GPIO_Pin_7
#define OP_SUCCESS           1
#define OP_FAIL              0


UINT8 I2C_Start(void);
void I2C_ReStart(void);
void I2C_Stop(void);
INT8U I2C_CheckAck(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void I2C_SendByte(INT8U data);
INT8U I2C_RecvByte(void);

#endif

