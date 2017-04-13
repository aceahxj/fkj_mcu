/***************************************************************
*  FileName              :  cw2015_i2c.h
*  Copyright             :  e-LinkCare Medical Inc.
*  ModuleName            :
*
*  CPU                   :  STM8L152K4
*  RTOS                  :

*  CreateData            :  2016-11-15
*  Author/Corporation    :
*
*  Description           :
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

#ifndef _cw2015_I2C_H
#define _cw2015_I2C_H

#ifdef  cw2015_I2C_GLOBAL
#define cw2015_I2C_EXT
#else
#define cw2015_I2C_EXT extern
#endif

#include  "bsp.h"
#include  "delay.h"
#include  "lib_type.h"
#include  "i2c.h"


//CW2015µÿ÷∑
#define EE_WR_ADDR              0xc4
#define EE_RD_ADDR              0xc5
#define CW_VOL_REGH             0x02
#define CW_VOL_REGL             0x03
#define CW_SOC_REGH             0x04
#define CW_SOC_REGL             0x05
#define CW_RRT_REGH             0x06
#define CW_RRT_REGL             0x07
#define CW_ALERT_REG            0x08
#define CW_MODE_REG             0x0a

#define CW_SLEEP                0xc0
#define CW_WAKEUP               0x00

void I2C_ByteWrite(INT8U addr, INT8U data);
INT8U I2C_ByteRead(INT8U addr);
INT16U cell_vol_read(void);
INT8U cell_soc_read(void);
INT16U cell_rrt_read(void);
void cw_alert_config(INT8U alert_percent);
void cw_mode_set(INT8U mode);

#endif

