/***************************************************************
*  FileName              :  cw2015_i2c.c
*  Copyright             :  e-LinkCare Medical Inc.
*  ModuleName            :
*
*  CPU                   :  STM8L152K4
*  RTOS                  :
*
*  CreateData            :  2016-11-15
*  Author/Corporation    :
*
*  Description           :  EEPROM读写操作
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#define   cw2015_I2C_GLOBAL

#include  "cw2015_i2c.h"

/*************************************************************
* 名    称：I2C_ByteWrite
* 功    能：I2C写入CW2015单字节
* 入口参数：addr 写入数据的地址
*           data 写入数据
* 出口参数：无
* 说    明：
**************************************************************/
void I2C_ByteWrite(INT8U addr, INT8U data)
{
    I2C_Start();
    I2C_SendByte(EE_WR_ADDR);
    I2C_CheckAck();
    I2C_SendByte(addr);
    I2C_CheckAck();
    I2C_SendByte(data);
    I2C_CheckAck();
    I2C_Stop();
}

/*************************************************************
* 名    称：I2C_ByteRead
* 功    能：I2C读取CW2015单字节
* 入口参数：读取地址
* 出口参数：读取的一个字节
* 说    明：每个AD字0.305mV
**************************************************************/
INT8U I2C_ByteRead(INT8U addr)
{
    INT8U data;

    I2C_Start();
    I2C_SendByte(EE_WR_ADDR);
    I2C_CheckAck();

    I2C_SendByte(addr);
    I2C_CheckAck();

    I2C_Start();
    I2C_SendByte(EE_RD_ADDR);
    I2C_CheckAck();
    data = I2C_RecvByte();
    I2C_NoAck();
    I2C_Stop();
    return data;
}

/*************************************************************
* 名    称：cell_vol_read
* 功    能：读取电池电压
* 入口参数：无
* 出口参数：电压值
* 说    明：LSB 305uV
**************************************************************/
INT16U cell_vol_read(void)
{
    INT16U ret;
    ret = I2C_ByteRead(CW_VOL_REGH)<<8;
    ret |= I2C_ByteRead(CW_VOL_REGL);
    return ret;
}

/*************************************************************
* 名    称：cell_soc_read
* 功    能：读取电池剩余电量
* 入口参数：无
* 出口参数：剩余电量
* 说    明：只读高字节%,低字节1/256%
**************************************************************/
INT8U cell_soc_read(void)
{
    INT8U ret;
    ret = I2C_ByteRead(CW_SOC_REGH);
    //ret |= I2C_ByteRead(CW_SOC_REGL);
    return ret;
}

/*************************************************************
* 名    称：cell_rrt_read
* 功    能：读取电池剩余时间
* 入口参数：无
* 出口参数：12位剩余时间，分钟
* 说    明：最高位是报警位
**************************************************************/
INT16U cell_rrt_read(void)
{
    INT16U ret;
    ret = I2C_ByteRead(CW_RRT_REGH)<<8;
    ret |= I2C_ByteRead(CW_RRT_REGL);
    return ret;
}

/*************************************************************
* 名    称：cw_alert_config
* 功    能：设置报警阈值
* 入口参数：阈值
* 出口参数：
* 说    明：高5位设置1%~31%，默认3%
**************************************************************/
void cw_alert_config(INT8U alert_percent)
{
    I2C_ByteWrite(CW_ALERT_REG,alert_percent<<3);
}

/*************************************************************
* 名    称：cw_mode_set
* 功    能：设置工作模式
* 入口参数：模式 sleep(d7d6),qstart(d5d4),por(d3d2d1d0)
* 出口参数：
* 说    明：11-sleep,00-wakeup
**************************************************************/
void cw_mode_set(INT8U mode)
{
    I2C_ByteWrite(CW_MODE_REG,mode<<3);
}


