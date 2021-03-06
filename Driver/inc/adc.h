/***************************************************************
*  FileName              :    adc.h
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :

*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    ad����
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#ifndef _ADC_H
#define _ADC_H

#ifdef ADC_GLOBALS
#define ADC_EXT
#else
#define ADC_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "lib_type.h"
#include "stm32f0xx.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/
#define ADCCONV_NUM                 64                  //ADCת������

#define HISI_3V2                    ADC_Channel_7
#define HISI_1V8                    ADC_Channel_13
#define HISI_1V1				    ADC_Channel_15
#define SENSOR_3V2                  ADC_Channel_14
#define SENSOR_1V8                  ADC_Channel_14


#define ADC_CHNL_NONE				 (ADC1->CHSELR &= (uint32_t)0x00000000)




/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

UINT16 getadcvalue(UINT32 ADC_Channel);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
