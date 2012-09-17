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

void setup();

int main(void)
{
	setup();
    while(1)
    {
        
    }
};

void setup()
{
	
	/* set PORTA pin 1 to out **/
	DDRB = 0b00000001;
	
	/** setup basic timer **/
	TCNT1 = 0;								/** Reset the timer counter **/
	TCCR1A |=0;
	TCCR1B |= (1<<WGM12);					/** mode 4 - CTC **/
	TCCR1B |= (1<<CS11) | (1<<CS10);		/** F_CPU/8**/
	OCR1A = 2;							/** 40000 * 0.0000005s = 20 ms **/
	TIMSK |= (1<<OCIE1A);					/** Enable Interrupt in mask **/
	
	sei();
};	

void ppmout()
{
	/** send sync **/
	/** send pulses **/
	PORTB = ~(PORTB);
};


ISR(TIMER1_COMPA_vect)
{	
	cli();
	//TIMSK&= ~(1<<OCIE1A);
	ppmout();		/** throw ppm signals out on pin PPMOUT **/
}