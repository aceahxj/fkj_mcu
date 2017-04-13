/***************************************************************
*  FileName              :    main.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :
*
*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    Ö÷º¯Êý
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#include "stm32f0xx.h"
#include "delay.h"
#include "bsp.h"
#include "adc.h"
#include "uart.h"
#include "interrupt.h"
#include "systemwork.h"


//#define  IAP

#ifdef IAP
#define APPLICATION_ADDRESS     ((uint32_t)0x08002000)
#if   (defined ( __CC_ARM ))
  __IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
  __no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
  __IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#elif defined ( __TASKING__ )
  __IO uint32_t VectorTable[48] __at(0x20000000);
#endif

#endif


int main(void)
{
	#ifdef IAP
	uint32_t i = 0;

	/* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/

	  /* Copy the vector table from the Flash (mapped at the base of the application
		 load address 0x08002800) to the base address of the SRAM at 0x20000000. */

	for(i = 0; i < 48; i++)
	{
	  VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}

	/* Enable the SYSCFG peripheral clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Remap SRAM at 0x00000000 */
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
    #endif
/*
* Application start here
*/
    reset_system();
	for(;;)
	{
		#if 0
        //debug here
		#else
      	system_task();
		//feedwwdg();
		#endif
	}
}
