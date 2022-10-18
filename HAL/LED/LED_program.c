/*
 * LED_program.c
 *
 *  Created on: Oct 10, 2022
 *      Author: Al-Hasan Ameer
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "../../MCAL/DIO/DIO_interface.h"
#include "LED_interface.h"
#include "../../MCAL/TIMER0/TIMER_interface.h"
#include "LED_private.h"
#include "LED_config.h"

/*
 * by this function we can Turn ON the LED I want.
 * Parameter: Copy_u8LED - object from LED_t structure-
 */
u8 LED_u8LEDOn(LED_t Copy_u8LED)
{
	u8 Local_u8ErrorState=0;
	switch(Copy_u8LED.type)
	{
	case LED_SOURCE: DIO_u8SetPinValue(Copy_u8LED.port, Copy_u8LED.pin, DIO_u8PIN_HIGH);break;
	case LED_SINK:   DIO_u8SetPinValue(Copy_u8LED.port, Copy_u8LED.pin, DIO_u8PIN_LOW); break;
	default: Local_u8ErrorState=1; break;
	}
	return Local_u8ErrorState;
}

/*
 * by this function we can Turn OFF the LED I want.
 * Parameter: Copy_u8LED - object from LED_t structure-
 */
u8 LED_u8LEDOff(LED_t Copy_u8LED)
{
	u8 Local_u8ErrorState=0;
	switch(Copy_u8LED.type)
	{
	case LED_SOURCE: DIO_u8SetPinValue(Copy_u8LED.port, Copy_u8LED.pin, DIO_u8PIN_LOW); break;
	case LED_SINK:   DIO_u8SetPinValue(Copy_u8LED.port, Copy_u8LED.pin, DIO_u8PIN_HIGH);break;
	default: Local_u8ErrorState=1; break;
	}
	return Local_u8ErrorState;
}

/*
 * by this function we can Toggle the LED I want.
 * Parameter: Copy_u8LED - object from LED_t structure-, Copy_u32Delay_ms-delay time-
 */
u8 LED_u8Toggle(LED_t Copy_u8LED, u32 Copy_u32Delay_ms)
{
	u8 Local_u8ErrorState=0;
	u8 Local_u8PinState;
	DIO_u8GetPinValue(Copy_u8LED.port,Copy_u8LED.pin,&Local_u8PinState);

	switch(Copy_u8LED.type)
	{
	case LED_SOURCE:
		LED_u8LEDOn(Copy_u8LED);
		TIMER_voidTimer0Delay(Copy_u32Delay_ms);
		LED_u8LEDOff(Copy_u8LED);
		TIMER_voidTimer0Delay(Copy_u32Delay_ms);
		break;
	case LED_SINK:
		LED_u8LEDOff(Copy_u8LED);
		TIMER_voidTimer0Delay(Copy_u32Delay_ms);
		LED_u8LEDOn(Copy_u8LED);
		TIMER_voidTimer0Delay(Copy_u32Delay_ms);
		break;
	default: Local_u8ErrorState=1;          break;
	}
	return Local_u8ErrorState;
}

