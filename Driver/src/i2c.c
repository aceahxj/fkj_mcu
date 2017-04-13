/***************************************************************
*  FileName              :  i2c.c
*  Copyright             :  e-LinkCare Medical Inc.
*  ModuleName            :
*
*  CPU                   :  STM32F103V8T6
*  RTOS                  :
*
*  CreateData            :  2016-04-26
*  Author/Corporation    :
*
*  Description           :  IO模拟I2C
*
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#define    I2C_GLOBAL

#include "i2c.h"
#include "delay.h"

/*! 延时2us*/
void I2C_Delay(void)
{
    delay_us(2);
}

/*! 发送I2C起始条件*/
UINT8 I2C_Start(void)
{
    SDA_HIGH();
    SCL_HIGH();
    I2C_Delay();
    if (!SDA_IN())
        return OP_FAIL;
    SDA_LOW();
    I2C_Delay();
    if (SDA_IN())
        return OP_FAIL;
    SCL_LOW();
    I2C_Delay();
    return OP_SUCCESS;
}

/*! 发送I2C停止条件*/
void I2C_Stop(void)
{
    SCL_LOW();
    I2C_Delay();
    SDA_LOW();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SDA_HIGH();
    I2C_Delay();
}

/*! 发送I2C 一个字节*/
void I2C_SendByte(INT8U data)
{
    volatile INT8U i=0;
    for(i=0; i<8; i++)
    {
        SCL_LOW();
        I2C_Delay();
        if(data & 0x80){
            SDA_HIGH();
        } else {
            SDA_LOW();
        }
        data <<= 1;
        I2C_Delay();
        SCL_HIGH();
        I2C_Delay();
    }
    SCL_LOW();
}

/*! 发送一字节后读取 Ack*/
INT8U I2C_CheckAck(void)
{
    INT8U  ack;
    SCL_LOW();
    I2C_Delay();
    SDA_HIGH();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    if(SDA_IN())
        ack = 1;
    else
        ack = 0;
    SCL_LOW();
    return ack;
}

/*! 接收一字节 */
INT8U I2C_RecvByte(void)
{
    INT8U data=0;
    volatile INT8U i;

    SDA_HIGH();
    for(i=0; i<8; i++)
    {
        data <<= 1;
        SCL_LOW();
        I2C_Delay();
        SCL_HIGH();
        I2C_Delay();
        if(SDA_IN()){
            data |= 0x01;
        }
        SCL_LOW();
    }
    return data;
}

/*! 接收一字节后应答Ack */
void I2C_Ack(void)
{
    SCL_LOW();
    I2C_Delay();
    SDA_LOW();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
    I2C_Delay();
}

/*! 接收最后一字节后回复NoAck*/
void I2C_NoAck(void)
{
    SCL_LOW();
    I2C_Delay();
    SDA_HIGH();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
    I2C_Delay();
}

