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
*  Description           :  EEPROM��д����
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#define   cw2015_I2C_GLOBAL

#include  "cw2015_i2c.h"

/*************************************************************
* ��    �ƣ�I2C_ByteWrite
* ��    �ܣ�I2Cд��CW2015���ֽ�
* ��ڲ�����addr д�����ݵĵ�ַ
*           data д������
* ���ڲ�������
* ˵    ����
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
* ��    �ƣ�I2C_ByteRead
* ��    �ܣ�I2C��ȡCW2015���ֽ�
* ��ڲ�������ȡ��ַ
* ���ڲ�������ȡ��һ���ֽ�
* ˵    ����ÿ��AD��0.305mV
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
* ��    �ƣ�cell_vol_read
* ��    �ܣ���ȡ��ص�ѹ
* ��ڲ�������
* ���ڲ�������ѹֵ
* ˵    ����LSB 305uV
**************************************************************/
INT16U cell_vol_read(void)
{
    INT16U ret;
    ret = I2C_ByteRead(CW_VOL_REGH)<<8;
    ret |= I2C_ByteRead(CW_VOL_REGL);
    return ret;
}

/*************************************************************
* ��    �ƣ�cell_soc_read
* ��    �ܣ���ȡ���ʣ�����
* ��ڲ�������
* ���ڲ�����ʣ�����
* ˵    ����ֻ�����ֽ�%,���ֽ�1/256%
**************************************************************/
INT8U cell_soc_read(void)
{
    INT8U ret;
    ret = I2C_ByteRead(CW_SOC_REGH);
    //ret |= I2C_ByteRead(CW_SOC_REGL);
    return ret;
}

/*************************************************************
* ��    �ƣ�cell_rrt_read
* ��    �ܣ���ȡ���ʣ��ʱ��
* ��ڲ�������
* ���ڲ�����12λʣ��ʱ�䣬����
* ˵    �������λ�Ǳ���λ
**************************************************************/
INT16U cell_rrt_read(void)
{
    INT16U ret;
    ret = I2C_ByteRead(CW_RRT_REGH)<<8;
    ret |= I2C_ByteRead(CW_RRT_REGL);
    return ret;
}

/*************************************************************
* ��    �ƣ�cw_alert_config
* ��    �ܣ����ñ�����ֵ
* ��ڲ�������ֵ
* ���ڲ�����
* ˵    ������5λ����1%~31%��Ĭ��3%
**************************************************************/
void cw_alert_config(INT8U alert_percent)
{
    I2C_ByteWrite(CW_ALERT_REG,alert_percent<<3);
}

/*************************************************************
* ��    �ƣ�cw_mode_set
* ��    �ܣ����ù���ģʽ
* ��ڲ�����ģʽ sleep(d7d6),qstart(d5d4),por(d3d2d1d0)
* ���ڲ�����
* ˵    ����11-sleep,00-wakeup
**************************************************************/
void cw_mode_set(INT8U mode)
{
    I2C_ByteWrite(CW_MODE_REG,mode<<3);
}


