/***************************************************************
*  FileName              :    buzzerpwm.c
*  Copyright             :
*  ModuleName            :
*
*  CPU                   :
*  RTOS                  :

*  CreateData            :
*  Author/Corporation    :
*
*  Description           :    ·äÃùÆ÷PWM
*
*---------------------Revision History------------------------
*  No    Version    Date    Revised By    Item    Description
*
*
***************************************************************/
#include "buzzerpwm.h"

/**************************************************************
*  Function Name		 :	 oneshortbuzze
*  Param				 :	 void
*  Return Param 		 :	 void
*  Description		 :	 ·äÃùÆ÷Ò»Éù¶ÌÒô
***************************************************************/
void oneshortbuzze(void)
{
    BUZZER_ON();
    delay_ms(20);
	BUZZER_OFF();
}

/**************************************************************
*  Function Name		 :	 threelongbuzze
*  Param				 :	 void
*  Return Param 		 :	 void
*  Description		 :	 ·äÃùÆ÷ÈýÉù³¤Òô--¶¨Ê±¹Ø»ú
***************************************************************/
void threelongbuzze(void)
{
	UINT8 idx;

	for(idx = 3;idx > 0; idx--)
	{
       BUZZER_ON();
	   delay_ms(100);
	   BUZZER_OFF();
	   delay_ms(100);
	}
}

/**************************************************************
*  Function Name		 :	 twoshortbuzze
*  Param				 :	 void
*  Return Param 		 :	 void
*  Description		 :	 ·äÃùÆ÷Á½Éù¶ÌÒô--¿ªÊ¼ºôÆøÌáÊ¾
***************************************************************/
void twoshortbuzze(void)
{
	UINT8 idx;

	for(idx = 2;idx > 0; idx--)
	{
       BUZZER_ON();
	   delay_ms(120);
	   BUZZER_OFF();
	   delay_ms(120);
	}
}

/**************************************************************
*  Function Name		 :	 onelongbuzze
*  Param				 :	 void
*  Return Param 		 :	 void
*  Description		 :	 ·äÃùÆ÷Ò»Éù³¤Òô--²âÊÔÍê³É
***************************************************************/
void onelongbuzze(void)
{
    BUZZER_ON();
    delay_ms(800);
	BUZZER_OFF();
}

/**************************************************************
*  Function Name		 :	 threebuzzeorange
*  Param				 :	 void
*  Return Param 		 :	 void
*  Description		 :	 ·äÃùÆ÷µÎµÎ-µÎµÎ-µÎµÎÈýÉù¶ÌÒô--µç³ØµçÁ¿
***************************************************************/
void threebuzzeorange(void)
{
	UINT8 idx;

	for(idx = 3;idx > 0;idx--)
	{
       BUZZER_ON();
	   delay_ms(50);
	   BUZZER_OFF();
	   delay_ms(150);
	   BUZZER_ON();
	   delay_ms(50);
	   BUZZER_OFF();
	   delay_ms(500);
	}
}

