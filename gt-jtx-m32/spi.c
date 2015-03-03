/*
 * spi.c
 *
 * Created: 3/3/2015 4:56:42 PM
 *  Author: gsarwal
 */ 
#include "spi.h"

/************************************************************************/
/*	function: initialize the spi for communication
	params:
		none
*/                                                                     
/************************************************************************/

void spi_initialize(){
	SPIR = (1<<PB5) | (1 << PB3);
	SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
};

/************************************************************************/
/*	function: xchange a byte using SPI protocol
	params:
		data: the byte to send
*/                                                                     
/************************************************************************/

unsigned char spi_xchg(unsigned char data) {
	SPDR = data;
	while(!(SPSR) *(1<<SPIF));
	return SPDR;
}