/*
 * LED_interface.h
 *
 *  Created on: Oct 10, 2022
 *      Author: Al-Hasan Ameer
 */

#ifndef LED_INTERFACE_H_
#define LED_INTERFACE_H_


/*LED Connection Type defines*/
#define LED_SINK       0
#define LED_SOURCE     1

/*LED Structure*/
typedef struct
{
	u8 port;
	u8 pin;
	u8 type;
}LED_t;

/*
 * by this function we can Turn ON the LED I want.
 * Parameter: Copy_u8LED - object from LED_t structure-
 */
u8 LED_u8LEDOn(LED_t Copy_u8LED);

/*
 * by this function we can Turn OFF the LED I want.
 * Parameter: Copy_u8LED - object from LED_t structure-
 */
u8 LED_u8LEDOff(LED_t Copy_u8LED);

/*
 * by this function we can Toggle the LED I want.
 * Parameter: Copy_u8LED - object from LED_t structure-
 */
u8 LED_u8Toggle(LED_t Copy_u8LED, u32 Copy_u32Delay_ms);


#endif /* LED_INTERFACE_H_ */
