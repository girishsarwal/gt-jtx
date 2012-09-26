/*
 * gt-jtx.cpp
 *
 * Created: 20-09-2012 15:08:33
 *  Author: Girish Sarwal
 *	 Email: g.sarwal@gmail.com
 *
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
static int channel = SYNC;
ISR(TIMER1_COMPA_vect){
	TIMSK &= ~(1<<OCIE1A);
	
	if(channel >= MAX_CHANNEL){
		channel = -1;
	}
	
	if((PINB & 0x02) == 0x02)		//If the actual pin is high, we need to set OCR1A to the complementary delay
		OCR1A = ppm[++channel];		
	else
		OCR1A = 2000 - ppm[channel];	

	TIMSK |= (1<<OCIE1A);
}

void setup(){
	DDRB = 0x02;							/** make PB1 as out pin **/
	
	PORTB = 0x02;                    /** make PB1 as high **/
	TCNT1 = 0;
	
	TCCR1B |= (1<<CS10) ;				/** F_CPU - 1Mhz Timebase - Outer Timer**/
	TCCR1B	|=	(1<<WGM12);				/** CTC Mode**/
	OCR1A = ppm[SYNC];	
	TCCR1A |= (1<<COM1A0);				/** hardware ctc **/
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
