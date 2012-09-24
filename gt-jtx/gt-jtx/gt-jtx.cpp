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


void setup();
void reset();
void applyTrimTransforms();
void applyEndPointTransforms();

#define SYNC 0
#define AIL	1
#define ELE	2
#define THR	3
#define RUD	4
#define MAX_CHANNEL RUD

uint16_t ppm[(MAX_CHANNEL + 1) * 2 ];


int main(){
	reset();
	
	setup();
	
	/**Enable global interrupts **/
	sei();

	/** start our timer **/
	TIMSK |= (1<<OCIE1A);
	while(1){
		/**update output display **/
	}
}
uint8_t i = 1;
static int channel = SYNC;
ISR(TIMER1_COMPA_vect){
	PORTC ^= (1<<0);
	//if(channel %2 == 0){
		//PORTC = 0x00;
	//}
	OCR1A = ppm[channel++];
	if(channel > MAX_CHANNEL * 2){
		channel = SYNC;
	}
}

void setup(){
	DDRC = 0x01;
	PORTC = 0x01;
	TCNT1 = 0;
	
	TCCR1B |= (1<<CS10) ;				/** F_CPU - 1Mhz Timebase - Outer Timer**/
	TCCR1B	|=	(1<<WGM12);				/** CTC Mode**/
	OCR1A = ppm[SYNC];						
}

void reset(){
	/** set values to match servo spec **/
	ppm[0] = 1000;
	ppm[1] = 2000 - ppm[0];
	
	ppm[2] = 1500;
	ppm[3] = 2000 -ppm[2];
	
	ppm[4] = 1500;
	ppm[5] = 2000- ppm[4];
	
	ppm[6] = 500;
	ppm[7] = 2000 - ppm[6];
	
	ppm[8] = 500;
	ppm[9] = 2000 - ppm[8];
	
}