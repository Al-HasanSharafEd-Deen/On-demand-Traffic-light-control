/*
 * main.c
 *
 *  Created on: Oct 13, 2022
 *      Author: Alhassan
 */
 
 /*LIB includes */
#include "LIB/STD_TYPES.h"

/*APP includes */
#include "APP/APP_interface.h"


int main (void)
{
	APP_voidInit();
	while(1)
	{
		APP_voidStart();
	}
	return 0;
}
