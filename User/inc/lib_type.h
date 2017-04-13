/***************************************************************
*  FileName              :    lib_type.h
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    数据类型定义
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/

#ifndef  _LIB_TYPE_H
#define  _LIB_TYPE_H


#ifndef  NULL
#define  NULL   (void *)0
#endif

//位定义
#define   Bit0    (1 << 0)
#define   Bit1    (1 << 1)
#define   Bit2    (1 << 2)
#define   Bit3    (1 << 3)
#define   Bit4    (1 << 4)
#define   Bit5    (1 << 5)
#define   Bit6    (1 << 6)
#define   Bit7    (1 << 7)
#define   Bit8    (1 << 8)
#define   Bit9    (1 << 9)
#define   Bit10   (1 << 10)
#define   Bit11   (1 << 11)
#define   Bit12   (1 << 12)
#define   Bit13   (1 << 13)
#define   Bit14   (1 << 14)
#define   Bit15   (1 << 15)
#define   Bit16   (1 << 16)
#define   Bit17   (1 << 17)
#define   Bit18   (1 << 18)
#define   Bit19   (1 << 19)
#define   Bit20   (1 << 20)
#define   Bit21   (1 << 21)
#define   Bit22   (1 << 22)
#define   Bit23   (1 << 23)
#define   Bit24   (1 << 24)
#define   Bit25   (1 << 25)
#define   Bit26   (1 << 26)
#define   Bit27   (1 << 27)
#define   Bit28   (1 << 28)
#define   Bit29   (1 << 29)
#define   Bit30   (1 << 30)
#define   Bit31   (1 << 31)


typedef  int              BOOLEAN;
typedef  unsigned char    INT8U;
typedef  signed char      INT8S;
typedef  unsigned short   INT16U;
typedef  signed short     INT16S;
typedef  unsigned int     INT32U;
typedef  signed int       INT32S;
typedef  float            FP32;
typedef  float            FP64;

#define     TRUE    1
#define     FALSE   0

#define  UINT8    INT8U
#define  SINT8    INT8S
#define  UINT16   INT16U
#define  SINT16   INT16S
#define  UINT32   INT32U
#define  SINT32   INT32S

typedef struct
{
	INT8U  b0    :1;
	INT8U  b1    :1;
	INT8U  b2    :1;
	INT8U  b3    :1;
	INT8U  b4    :1;
	INT8U  b5    :1;
	INT8U  b6    :1;
	INT8U  b7    :1;
}BITS8;


//8位共用体
typedef union
{
	INT8U  ubyte;
	BITS8  bits;
}UNION8;


//16位共用体
typedef union
{
	INT16U  ushort;
	INT8U   ubyte[2];
}UNION16;


//32位共用体
typedef union
{
	INT32U  uword;
	INT8U   ubyte[4];
}UNION32;

#endif
