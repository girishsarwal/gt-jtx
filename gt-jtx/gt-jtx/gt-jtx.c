/*
 * gt-jtx.cpp
 *
 * Created: 20-09-2012 15:08:33
 *  Author: Girish Sarwal
 *	 Email: g.sarwal@gmail.com
 *
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <lcd.h>

void reset();
void setup();

uint16_t map(uint16_t);
int16_t mapPercent(uint16_t);
uint16_t readAnalog(uint8_t);
uint16_t ms_to_ticks(uint16_t);
uint16_t micros_to_ticks(uint16_t);

void applyTrimTransforms();
void applyEndPointTransforms();

#define START_FROM_SYNC_CHANNEL_WIDTH
//#define START_FROM_FIRST_CHANNEL_WIDTH
//#define START_FROM_INTER_CHANNEL_WIDTH

/** indexing in data array **/
#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3
#define CHANNEL_4 4
#define CHANNEL_5 5
#define CHANNEL_6 6
#define CHANNEL_7 7
#define CHANNEL_8 8
#define CHANNEL_9	9
#define CHANNEL_10 10

/** channel mapping to named items **/
#define SYNC	0
#define AIL		CHANNEL_1
#define ELE		CHANNEL_2
#define THR		CHANNEL_3
#define RUD		CHANNEL_4
#define FLAPS 	CHANNEL_5
#define GEAR 	CHANNEL_6
#define MIX 	CHANNEL_7
#define AUX1 	CHANNEL_8
#define AUX2	CHANNEL_9
#define AUX3	CHANNEL_10

#define MAX_CHANNEL AUX3

#define MIN_SIGNAL_WIDTH 1000  	/** 1000 microsecond = 1 ms **/
#define MID_SIGNAL_WIDTH 1500  	/** 1500 microsecond = 1.5 ms **/
#define MAX_SIGNAL_WIDTH 2000		/** 2000 microsecond = 2 ms **/
#define INIT_SIGNAL_WIDTH MID_SIGNAL_WIDTH
#define INTER_CHANNEL_WIDTH 200 	/**200 microsecond **/
#define SIGNAL_TRAVERSAL MAX_SIGNAL_WIDTH - MIN_SIGNAL_WIDTH
#define FRAME_WIDTH	20000			/**20000 microsecond = 20 ms **/
#define SYNC_SIGNAL_WIDTH (FRAME_WIDTH - (MAX_CHANNEL * (MID_SIGNAL_WIDTH + INTER_CHANNEL_WIDTH))) /**whatever is left **/

#define ADC_LOWER 0
#define ADC_UPPER 1023

#define STARTADC ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE);

uint16_t ppm[MAX_CHANNEL + 1];	/** accomodate +1 for the sync channel **/
char szBuffer[16];
static int channel = SYNC;

float adc_scaler = 0;

int main(){
	reset();
	
	setup();
	
	/**Enable global interrupts **/
	sei();

	/** start our timer **/
	TIMSK |= (1<<OCIE1A);
	/** start the adc **/
	ADCSRA |= (1<<ADSC);
	while(1){
		/**read inputs **/
		ppm[AIL] = map(readAnalog(AIL));
		ppm[ELE] = map(readAnalog(ELE));
		ppm[THR] = map(readAnalog(THR));
		ppm[RUD] = map(readAnalog(RUD));		
		lcd_clrscr();
		lcd_home();
		sprintf(szBuffer, "A:%d E:%d", mapPercent(ppm[AIL]), mapPercent(ppm[ELE]));
		lcd_puts(szBuffer);
		
		lcd_gotoxy(0,1);
		sprintf(szBuffer, "T:%d R:%d",  mapPercent(ppm[THR]),  mapPercent(ppm[RUD]));
      lcd_puts(szBuffer);

	}
};
int16_t mapPercent(uint16_t value){
	/** traversal range is between -500 and 500 and the percent value
	 	mapping needs to be between -100 and 100, so this is a shortcut
	**/
	int16_t m =(value - MID_SIGNAL_WIDTH);
	return m/5;
};

uint16_t map(uint16_t value){
	return MIN_SIGNAL_WIDTH + (value * adc_scaler);
};

uint16_t ms_to_ticks(uint16_t value){
	return (value * F_CPU/1000);
};
uint16_t micros_to_ticks(uint16_t value){
	return (value * ((F_CPU/1000)/1000));
};
ISR(TIMER1_COMPA_vect){
	TIMSK &= ~(1<<OCIE1A);
	if((PINB & 0x02) == 0x02){		//If the actual pin is high, we need to set OCR1A to the complementary delay
		if(++channel > MAX_CHANNEL){
			channel = 0;
		}
		OCR1A = micros_to_ticks(ppm[channel]);
	}		
	else{
		OCR1A = micros_to_ticks(INTER_CHANNEL_WIDTH);
   }
	TIMSK |= (1<<OCIE1A);
};

void setup(){
	adc_scaler = ((float)SIGNAL_TRAVERSAL) /(ADC_UPPER - ADC_LOWER);
	/** init LCD Display **/
	lcd_init(LCD_DISP_ON);
	/** set output **/
	DDRB = 0xFF;						/** make PB1 as out pin **/
	
	PORTB = 0x20;						/** make PB1 as low **/
	TCNT1 = 0;
	
	TCCR1B |= (1<<CS10) ;				/** F_CPU - 1Mhz Timebase **/
	TCCR1B	|=	(1<<WGM12);				/** CTC Mode**/

	TCCR1A |= (1<<COM1A0);				/** hardware ctc **/
	
	/** set inputs **/					/** PORTC in the input for ADC control on ATmega8**/
	PORTC = 0x00;
	ADMUX |= (1<<REFS0);				
	ADCSRA |= (1<<ADEN); 					/** enable the ADC **/
	ADCSRA|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	/**F_CPU/64 Prescalar **/
	
};
uint16_t readAnalog(uint8_t ch){
	ch = ch & 0x07;			/** zero out all bits except last three for limiting channel **/
	ADMUX &=0xE0;		/**reset channel selection **/
	ADMUX |=ch;						/**select the requested channel **/
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|= (1<<ADIF);
	return (ADC);
};
void reset(){
	/** set values to match servo spec **/
	ppm[SYNC] = SYNC_SIGNAL_WIDTH;
	ppm[AIL] = INIT_SIGNAL_WIDTH;
	ppm[ELE] = INIT_SIGNAL_WIDTH;
	ppm[THR] = INIT_SIGNAL_WIDTH;
	ppm[RUD] = INIT_SIGNAL_WIDTH;
	ppm[FLAPS] = INIT_SIGNAL_WIDTH;
	ppm[GEAR] = INIT_SIGNAL_WIDTH;
	ppm[MIX] = INIT_SIGNAL_WIDTH;
	ppm[AUX1] = INIT_SIGNAL_WIDTH;
	ppm[AUX2] = INIT_SIGNAL_WIDTH;
	ppm[AUX3] = INIT_SIGNAL_WIDTH;
	/** reset to the first channel **/
#ifdef START_FROM_FIRST_CHANNEL_WIDTH
	channel = AIL;
	OCR1A = micros_to_ticks(ppm[AIL]);
#endif
#ifdef START_FROM_SYNC_CHANNEL_WIDTH
	channel = SYNC;
	OCR1A = micros_to_ticks(ppm[SYNC]);
#endif
#ifdef START_FROM_INTER_CHANNEL_WIDTH
	channel = -1;
	OCR1A = micros_to_ticks(INTER_CHANNEL_WIDTH);
#endif
	
};












