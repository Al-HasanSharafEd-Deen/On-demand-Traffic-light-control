/*
 * APP_program.c
 *
 *  Created on: 12 Oct 2022
 *      Author: alhasan
 */

/*LIB includes */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
/*MCAL includes */
#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/PORT/PORT_interface.h"
#include "../MCAL/EXTI/EXTI_interface.h"
#include "../MCAL/GIE/GIE_interface.h"
#include "../MCAL/TIMER0/TIMER_interface.h"
/*HAL includes */
#include "../HAL/LED/LED_interface.h"
#include "../HAL/SWITCH/SW_interface.h"

#include "APP_interface.h"

/*Car LEDs objects of LED structure*/
LED_t carRedLED    = {DIO_u8PORTA, DIO_u8PIN0,LED_SOURCE};
LED_t carGreenLED  = {DIO_u8PORTA, DIO_u8PIN1,LED_SOURCE};
LED_t carYellowLED = {DIO_u8PORTA, DIO_u8PIN2,LED_SOURCE};

/*Pedestrian LEDs objects of LED structure*/
LED_t pedesRedLED    = {DIO_u8PORTC, DIO_u8PIN0,LED_SOURCE};
LED_t pedesGreenLED  = {DIO_u8PORTC, DIO_u8PIN1,LED_SOURCE};
LED_t pedesYellowLED = {DIO_u8PORTC, DIO_u8PIN2,LED_SOURCE};

/*push button structure*/
SW_Type interrupt_SW = {DIO_u8PORTD, DIO_u8PIN2,SW_Int_PULL_UP};

/*car LED State*/
LED_STATE_t carRed_LED    = LED_IDLE;
LED_STATE_t carGreen_LED  = LED_IDLE;
LED_STATE_t carYellow_LED = LED_IDLE;

/*pedestrian LED State*/
LED_STATE_t pedesRed_LED    = LED_IDLE;
LED_STATE_t pedesGreen_LED  = LED_IDLE;
LED_STATE_t pedesYellow_LED = LED_IDLE;

/*initial mode state*/
MODE_t modeState = NORMAL_MODE;

/*all Initials I need */
void APP_voidInit(void)
{
	PORT_voidInit();
	TIMER_voidTimer0Init();
	EXTI_u8IntSetSenseControl(INT_FALLING_EDGE, EXTI_INT0);
	EXTI_u8IntGISRState(GISR_ENEBLE, EXTI_INT0);
	EXTI_voidInt0SetCallBack(&INT0_ISR);
	GIE_voidEnable();
	SW_voidInit(interrupt_SW);
}

/*All application logic*/
u8 APP_voidStart(void)
{
	/*return error state*/
	u8 Local_u8ErrorState=0;
	/*check Mode state, normal or Pedestrian*/
	switch(modeState)
	{
	/*if mode is normal*/
	case NORMAL_MODE:
		/*turn on Car Red LED for 5SEC*/
		LED_u8LEDOn(carRedLED);
		carRed_LED = LED_ACTIVE ;
		TIMER_voidTimer0Delay(5000);
		/*turn off Car Red LED*/
		LED_u8LEDOff(carRedLED);
		carRed_LED = LED_IDLE ;

		/*turn on Car Green LED for 5SEC*/
		LED_u8LEDOn(carGreenLED);
		carGreen_LED = LED_ACTIVE ;
		TIMER_voidTimer0Delay(5000);
		/*turn off Car Green LED*/
		LED_u8LEDOff(carGreenLED);
		carGreen_LED = LED_IDLE ;

		/*blanking Car yellow LED for 5sec*/
		for(u8 counter=0; counter<5;counter++)
		{
			LED_u8LEDOn(carYellowLED);
			carYellow_LED = LED_ACTIVE ;
			TIMER_voidTimer0Delay(500);
			LED_u8LEDOff(carYellowLED);
			TIMER_voidTimer0Delay(500);
		}
		carYellow_LED = LED_IDLE ;
		break;
		/*if mode is pedestrian call the pedestrian function*/
	case PEDESTRIAN_MODE: APP_voidPedestrianMode(); break;
	default : Local_u8ErrorState=1; break;
	}

	/*return to normal mode*/
	modeState = NORMAL_MODE ;
	APP_voidStart();

	/*return error state*/
	return Local_u8ErrorState;
}


/*Pedestrian Mode logic*/
u8 APP_voidPedestrianMode(void)
{
	/*return error state*/
	u8 Local_u8ErrorState=0;
	/*turn off all LEDs*/
	LED_u8LEDOff(carRedLED);
	LED_u8LEDOff(carYellowLED);
	LED_u8LEDOff(carGreenLED);
	LED_u8LEDOff(pedesRedLED);
	LED_u8LEDOff(pedesYellowLED);
	LED_u8LEDOff(pedesGreenLED);


	if(carRed_LED == LED_ACTIVE)
	{
		/*turn on pedestrian Green and Car Red LEDs for 5 SEC*/
		LED_u8LEDOn(pedesGreenLED);
		LED_u8LEDOn(carRedLED);
		TIMER_voidTimer0Delay(5000);
		LED_u8LEDOff(pedesGreenLED);
		LED_u8LEDOff(carRedLED);
	}
	/*check if Car Green or Yellow LEDs Active*/
	else if((carGreen_LED == LED_ACTIVE) || (carYellow_LED == LED_ACTIVE))
	{
		/*blinking for 5 SEC to start pedestrian mode*/
		for(u8 counter=0; counter<5;counter++)
		{
			LED_u8LEDOn(pedesYellowLED);
			LED_u8LEDOn(carYellowLED);
			TIMER_voidTimer0Delay(500);
			LED_u8LEDOff(pedesYellowLED);
			LED_u8LEDOff(carYellowLED);
			TIMER_voidTimer0Delay(500);
		}
		/*turn off yellows after blanking*/
		LED_u8LEDOff(pedesYellowLED);
		LED_u8LEDOff(carYellowLED);

		/*turn on Pedestrian Green LED to Pedestrian passing*/
		LED_u8LEDOn(pedesGreenLED);
		LED_u8LEDOn(carRedLED);
		TIMER_voidTimer0Delay(5000);
		LED_u8LEDOff(pedesGreenLED);
		LED_u8LEDOff(carRedLED);

	}else
	{
		Local_u8ErrorState=1;
	}
	/*call this function to start turn off pedestrian mode*/
	APP_voidFinish();

	/*return error state*/
	return Local_u8ErrorState;
}

/*end of logic*/
u8 APP_voidFinish(void)
{
	/*return error state*/
	u8 Local_u8ErrorState=0;

	/*blink yellows to stop pedestrian mode*/
	for(u8 counter=0; counter<5;counter++)
	{
		LED_u8LEDOn(pedesYellowLED);
		LED_u8LEDOn(carYellowLED);
		TIMER_voidTimer0Delay(500);
		LED_u8LEDOff(pedesYellowLED);
		LED_u8LEDOff(carYellowLED);
		TIMER_voidTimer0Delay(500);
	}
	/*stop planking*/
	LED_u8LEDOff(pedesYellowLED);
	LED_u8LEDOff(carYellowLED);

	/*ready to back to normal mode*/
	LED_u8LEDOff(pedesGreenLED);
	LED_u8LEDOn(pedesRedLED);
	LED_u8LEDOn(carGreenLED);
	TIMER_voidTimer0Delay(5000);

	/*turn off all LEDs*/
	LED_u8LEDOff(carRedLED);
	LED_u8LEDOff(carYellowLED);
	LED_u8LEDOff(carGreenLED);
	LED_u8LEDOff(pedesRedLED);
	LED_u8LEDOff(pedesYellowLED);
	LED_u8LEDOff(pedesGreenLED);

	/*back to normal mode*/
	modeState = NORMAL_MODE ;
	APP_voidStart();

	/*return error state*/
	return Local_u8ErrorState;
}

/*ISR function*/
void INT0_ISR(void)
{
	modeState = PEDESTRIAN_MODE;
	APP_voidStart();
}
