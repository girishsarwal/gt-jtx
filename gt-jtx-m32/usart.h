/*
 * usart.h
 *
 * Created: 3/3/2015 3:58:18 PM
 *  Author: gsarwal
 */ 


#ifndef USART_H_
#define USART_H_
#include "gt-jtx-defines.h"
#include <avr/io.h>

#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

void usart_initialize();
void usart_send(unsigned char data);
unsigned char usart_recv();



#endif /* USART_H_ */