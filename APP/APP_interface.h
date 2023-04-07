/*
 * APP_interface.h
 *
 *  Created on: Apr 6, 2023
 *      Author: Al-Hasan Ameer
 */

#ifndef APP_APP_INTERFACE_H_
#define APP_APP_INTERFACE_H_

/*LEDs States Enum*/
typedef enum{
	LED_IDLE ,LED_ACTIVE
}LED_STATE_t;

/*mode state Enum*/
typedef enum{
	NORMAL_MODE, PEDESTRIAN_MODE
}MODE_t;

/*all Initials I need */
void APP_voidInit(void);

/*All application logic*/
u8 APP_u8Start(void);

/*Pedestrian Mode logic*/
u8 APP_u8PedestrianMode(void);

/*end of logic*/
u8 APP_u8Finish(void);

/*INT0_ISR function Prototype*/
void INT0_ISR(void);

#endif /* APP_APP_INTERFACE_H_ */
