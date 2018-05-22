/*
 * common.c
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#include "../includes/common.h"

/*********************************************** memset16 ***************************************
** used to set values in a 16 bit contigous location
**/
void memset16(uint16_t* a, uint16_t value, uint8_t size){
	volatile uint8_t index = -1;
	while(++index < size){
		a[index] = value;
	};
};
