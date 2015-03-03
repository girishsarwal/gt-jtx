/*
 * usart.c
 *
 * Created: 3/3/2015 3:58:46 PM
 *  Author: gsarwal
 */ 
#include "usart.h"

/************************************************************************/
/*	function: initialize the uart for communciation with artrix
	params:
		none
*/                                                                     
/************************************************************************/

void usart_initialize(){
	UBRRH = (BAUDRATE >> 8);
	UBRRL = BAUDRATE;
	UCSRB = (1<<TXEN) | (1<<RXEN);
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
};

/************************************************************************/
/*	function: send a byte via the usart
	params:
		data: the byte to send
*/                                                                     
/************************************************************************/

void usart_send(unsigned char data) {
	while(!(UCSRA && (1<<UDRE)));
	UDR = data;
};

/************************************************************************/
/*	function: receive a byte of data from the usart
	params:
		none	
*/                                                                     
/************************************************************************/

unsigned char usart_recv() {
	while(!(UCSRA && (1<<RXC)));
	return UDR;
};