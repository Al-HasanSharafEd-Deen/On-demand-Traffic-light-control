/*
 * TIMER_program.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Al-Hasan Ameer
 */


/*include Lib Files*/
#include <math.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/*include important files*/
#include "TIMER_interface.h"
#include "TIMER_register.h"
#include "TIMER_config.h"
#include "TIMER_private.h"

void TIMER_voidTimer0Init(void)
{
	/*Normal mode to TIMER0*/
	CLRBIT(TCCR0,TCCR0_WGM00);
	CLRBIT(TCCR0,TCCR0_WGM01);
}

void TIMER_voidTimer0Delay(u32 Copy_u32Delay_ms)
{
	CLRBIT(TCCR0, TCCR0_CS00);
	CLRBIT(TCCR0, TCCR0_CS01);
	SETBIT(TCCR0, TCCR0_CS02);

	f64 Local_f64TickTime= 256.0/10000.0;
	f64 Local_f64DelayMax= Local_f64TickTime *(1<<8);
	u32 Local_u32TimerInitValue;
	u32 Local_u32NumOV;
	u32 Local_u32OverflowCounter = 0;

	if(Copy_u32Delay_ms < Local_f64DelayMax)
	{
		Local_u32TimerInitValue = (Local_f64DelayMax - Copy_u32Delay_ms)/Local_f64TickTime;
		/*Set initial timer value*/
		TCNT0 = Local_u32TimerInitValue;
		/*Busy waiting*/
		while(GETBIT(TIFR,TIFR_TOV0) == 0);
		/*Clear the flag*/
		SETBIT(TIFR,TIFR_TOV0);

	}
	else if(Copy_u32Delay_ms == Local_f64DelayMax)
	{
		/*Set initial timer value*/
		TCNT0 = 0x00;
		/*Busy waiting*/
		while(GETBIT(TIFR,TIFR_TOV0) == 0);
		/*Clear the flag*/
		SETBIT(TIFR,TIFR_TOV0);
	}
	else
	{
		Local_u32NumOV = ceil(Copy_u32Delay_ms/Local_f64DelayMax);
		Local_u32TimerInitValue =(1<<8) - ((Copy_u32Delay_ms/Local_f64TickTime) / Local_u32NumOV);
		/*Set initial timer value*/
		TCNT0 = Local_u32TimerInitValue;

		while(Local_u32OverflowCounter < Local_u32NumOV)
		{
			/*Busy waiting*/
			while(GETBIT(TIFR,TIFR_TOV0) == 0);
			/*Clear the flag*/
			SETBIT(TIFR,TIFR_TOV0);

			Local_u32OverflowCounter++;
		}

	}

	/*Timer Stop*/
	TCCR0 = 0x00;
}
