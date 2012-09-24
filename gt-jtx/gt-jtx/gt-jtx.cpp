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
#define FLAPS 5
#define GEAR 6
#define MIX 7
#define AUX 8
#define MAX_CHANNEL AUX

uint16_t ppm[MAX_CHANNEL];


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
	TIMSK &= ~(1<<OCIE1A);
	
	if(channel >= MAX_CHANNEL){
		channel = -1;
	}
	if((PORTC & 0x01) == 0x01)		//If port is high, we need to set OCR1A to the complementary delay
		OCR1A = 2000 - ppm[channel];	
	else
		OCR1A = ppm[++channel];		//if port is low, we need to goto the next channel
   PORTC ^= (1<<0);		/** todo: change to hardware pin **/
	TIMSK |= (1<<OCIE1A);
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
	ppm[SYNC] = 1000;
	ppm[AIL] = 1500;
	ppm[ELE] = 1500;
	ppm[THR] = 500;
	ppm[RUD] = 500;
	ppm[FLAPS] = 500;
	ppm[GEAR] = 500;
	ppm[MIX] = 500;
	ppm[AUX] = 500;
}