/*
 * HelloChip.c
 *
 * Created: 20-09-2012 15:08:33
 *  Author: girish
 */ 
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main(){
	DDRC = 0x01;
	PORTC = 0x00;
	TCNT1 = 0;
	
	TCCR1B |= (1<<CS10) ;				/** F_CPU - 1Mhz Timebase**/
	TCCR1B	|=	(1<<WGM12);				/** Mode 4 **/
	OCR1A = 22000;						/** 1/(1000000 ) * 22000= 22 ms **/
	
	sei();
	TIMSK |= (1<<OCIE1A);
	while(1){
		
		
	}
}

ISR(TIMER1_COMPA_vect){
	/**Ailerons **/
	PORTC = 0x00;
	_delay_us(300);
	PORTC= 0x01;
	_delay_us(700);
	
	/**Elevator**/
	PORTC = 0x00;
	_delay_us(300);
	PORTC= 0x01;
	_delay_us(700);
	
	/** Throttle **/
	PORTC = 0x00;
	_delay_us(300);
	PORTC= 0x01;
	_delay_us(700);
	
	/** Rudder **/
	PORTC = 0x00;
	_delay_us(300);
	PORTC= 0x01;
	_delay_us(700);
	
	/**sync pulse **/
	PORTC = 0x00;
	_delay_us(1000);
	PORTC = 0x01;

	TCNT1 = 0;
}
