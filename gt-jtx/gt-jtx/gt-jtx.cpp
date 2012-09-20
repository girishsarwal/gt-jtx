/*
 * gt_jtx.cpp
 *
 * Created: 13-09-2012 18:09:41
 *  Author: girish
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000	//16 Mhz

#define PPMOUT 2

int main(void)
{
	
	/* set PORTA pin 1 to out **/
	DDRB = 0b00000001;
	
	/** setup outer timer 
	*	counts per 10 ms to give a timebase for frame generation
	*/
	
	TCNT0 = 0;								/** Reset the timer counter **/
	TCCR0 |= (1<<CS02) || (1 << CS00); 		/** F_CPU/1024 **/
	OCR0 = 156;								/** 156 * 0.00064s = 10 ms **/
	
	
	TCNT1 = 0;								/** Reset the timer counter **/
	TCCR1A |=0;
	TCCR1B |= (1<<WGM12);					/** mode 4 - CTC **/
	TCCR1B |= (1<<CS11);					/** F_CPU/8**/
	OCR1A = 2000;							/** 2000 * 0.0000005s = 1 ms **/
	
	
	sei();
	
	TIMSK |= (1<<OCIE0);					/** Enable Interrupt in mask so our 'outer' timer begins**/

    while(1)
    {
        PORTB = 0x00;
		/** updates to user interface etc from data values**/
    }
};




ISR(TIMER0_COMP_vect)
{

	cli();
	TIMSK &= ~(1<<OCIE0);	/** stop reentrance **/
	TIMSK |= (1<<OCIE1A);	/** enable our 'inner' timer **/
	TIMSK |= (1<<OCIE0);	/** enable back 'outer' timer **/
	sei();
}


ISR(TIMER1_COMPA_vect)
{	
	PORTB = 0x01;
	cli();
	TIMSK&= ~(1<<OCIE1A);	/** stop reentrancy **/
	/** single instruction **/
	TIMSK |= (1<<OCIE1A);	/** enable back 'inner' timer **/
	sei();
}
