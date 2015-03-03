/*
 * spi.h
 *
 * Created: 3/3/2015 4:55:06 PM
 *  Author: gsarwal
 *  Purpose: Used to communicate with SPI Peripherals
 *			Will be used with the Nokia LCD display for the gt-jtx
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include "gt-jtx-defines.h"

#define SPIR	DDRB		/** ATMEGA 16/32 specific**/
#define MOSI	PB5			/** ATMEGA 16/32 specific**/
#define MISO	PB6			/** ATMEGA 16/32 specific**/
#define SCK		PB7				/** ATMEGA 16/32 specific**/

void spi_initialize();
unsigned char spi_xchg(unsigned char data);


#endif /* SPI_H_ */