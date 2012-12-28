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
#include "lcd.h"

void reset();
void setup();

uint16_t map(uint16_t);
int16_t mapPercent(uint16_t, uint8_t);
uint16_t readAnalog(uint8_t);
uint16_t ms_to_ticks(uint16_t);
uint16_t micros_to_ticks(uint16_t);

void applyTrimTransforms();
void applyEndPointTransforms();
uint8_t getKeyPressed(uint8_t key);
void processDigitalInputs();
void processDisplay();

#define START_FROM_SYNC_CHANNEL_WIDTH
//#define START_FROM_FIRST_CHANNEL_WIDTH
//#define START_FROM_INTER_CHANNEL_WIDTH

#define USE_LCD_DISPLAY

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

#define MAX_CHANNEL AUX1

#define TRIM_AIL_PLUS 0x01
#define TRIM_AIL_MINUS 0x02
#define TRIM_ELE_PLUS 0x04
#define TRIM_ELE_MINUS 0x08
#define TRIM_THR_PLUS 0x10
#define TRIM_THR_MINUS 0x20
#define TRIM_RUD_PLUS 0x40
#define TRIM_RUD_MINUS 0x80

#define TRIM_COMBO_MENU (TRIM_AIL_PLUS | TRIM_AIL_MINUS | TRIM_ELE_PLUS | TRIM_ELE_MINUS)
#define HOME	SYNC

#define MIN_SIGNAL_WIDTH 700  	/**800 microsecond = 0.8 ms **/
#define MID_SIGNAL_WIDTH 1300  	/** 1300 microsecond = 1.3 ms **/
#define MAX_SIGNAL_WIDTH 1700		/** 1800 microsecond = 1.8 ms **/
#define TRIM_UPPER_END 200
#define TRIM_LOWER_END 0
#define TRIM_CENTER 100
#define INIT_SIGNAL_WIDTH MID_SIGNAL_WIDTH
#define INTER_CHANNEL_WIDTH 300 	/**300 microsecond **/
#define SIGNAL_TRAVERSAL MAX_SIGNAL_WIDTH - MIN_SIGNAL_WIDTH
#define FRAME_WIDTH	22500			/**22500 microsecond = 22.5 ms **/
#define SYNC_SIGNAL_WIDTH (FRAME_WIDTH - (MAX_CHANNEL * (MID_SIGNAL_WIDTH + INTER_CHANNEL_WIDTH))) /**whatever is left **/

#define ADC_LOWER 0
#define ADC_UPPER 1023

#define STARTADC ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE);

int16_t ppm[MAX_CHANNEL + 1];	/** accomodate +1 for the sync channel **/
uint8_t trims[MAX_CHANNEL + 1];		/** max channels for trims. Although atm only first 4 are useful, trims will always range from -100 to +100 points **/
int8_t percent[MAX_CHANNEL + 1];

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
		/**read pot inputs first of all
		TODO: Do this on a timer and ISR**/
		ppm[AIL] = map(readAnalog(AIL)) + trims[AIL];
		ppm[ELE] = map(readAnalog(ELE)) + trims[ELE];
		ppm[THR] = map(readAnalog(THR)) + trims[THR];
		ppm[RUD] = map(readAnalog(RUD)) + trims[RUD];		
		ppm[FLAPS] = map(readAnalog(FLAPS)) + trims[FLAPS];
      ppm[GEAR] = map(readAnalog(GEAR)) + trims[GEAR];

		percent[AIL] = mapPercent(ppm[AIL], trims[AIL]);
		percent[ELE] = mapPercent(ppm[ELE], trims[ELE]);
		percent[THR] = mapPercent(ppm[THR], trims[THR]);
		percent[RUD] = mapPercent(ppm[RUD], trims[RUD]);
		percent[FLAPS] = mapPercent(ppm[FLAPS], trims[FLAPS]);
		percent[GEAR] = mapPercent(ppm[GEAR], trims[GEAR]);

		processDigitalInputs();
		processDisplay();
	}
};

void processDigitalInputs()
{
		if((~PIND & TRIM_COMBO_MENU) == TRIM_COMBO_MENU)				
		{
			if(trims[SYNC] == 0)
				trims[SYNC]++;		/** just enter the first screen **/
			else
				trims[SYNC] = 0;
		}
		
		/** inputs are based on the screen we're on **/
		switch(trims[SYNC])
		{
		case HOME:
			incTrim(TRIM_AIL_PLUS, AIL);
			incTrim(TRIM_ELE_PLUS, ELE);
			incTrim(TRIM_THR_PLUS, THR);
			incTrim(TRIM_RUD_PLUS, RUD);
			decTrim(TRIM_AIL_MINUS, AIL);
			decTrim(TRIM_ELE_MINUS, ELE);
			decTrim(TRIM_THR_MINUS, THR);
			decTrim(TRIM_RUD_MINUS, RUD);
			return;
		case AIL:
			break;
		case ELE:
			break;
		case THR:
			break;		
		case RUD:
			break;		
		case FLAPS:
			break;		
		case GEAR:
			break;		
		case MIX:
			break;
		case AUX1:
			break;
		default:
			trims[SYNC] = AIL;	/**Reset to first setting screen if we dont know where we are **/
			return;
		}
		if(getKeyPressed(TRIM_AIL_PLUS)
			&& trims[SYNC] < MAX_CHANNEL)		
		{
			trims[SYNC]++;
		}
		if(getKeyPressed(TRIM_AIL_MINUS)
			&& trims[SYNC] > CHANNEL_1)		
		{
			trims[SYNC]--;
		}
}

uint8_t getKeyPressed(uint8_t key)
{
	if(((~PIND) & key) == key)	
	{
		_delay_ms(1);	/** get rid of keybounce **/
		return (((~PIND) & key) == key);
	}	
	return 0x00;
}
void incTrim(uint8_t trimkey, uint8_t channel)
{
	if(getKeyPressed(trimkey))
	{
		if(++trims[channel] > TRIM_UPPER_END)
			trims[channel] = TRIM_UPPER_END;
	}
}
void decTrim(uint8_t trimkey, uint8_t channel)
{
	if(getKeyPressed(trimkey))
	{
		if(trims[channel] > 0 && --trims[channel] <= TRIM_LOWER_END)
			trims[channel] = TRIM_LOWER_END;
	}	
}
void processDisplay()
{
   	#ifdef USE_LCD_DISPLAY
   		lcd_clrscr();
			lcd_home();
			switch(trims[SYNC])
			{
			case HOME:
				sprintf(szBuffer, "A:%d E:%d", percent[AIL], percent[ELE]);
				lcd_puts(szBuffer);
				lcd_gotoxy(0,1);
				sprintf(szBuffer, "T:%d R:%d", percent[THR], percent[RUD]);
				lcd_puts(szBuffer);
				return;
			case AIL:
				sprintf(szBuffer, "AIL  Tr    ms");
				break;
			case ELE:
				sprintf(szBuffer, "ELE  Tr    ms");				
				break;
			case THR:
				sprintf(szBuffer, "THR 	Tr    ms");				
				break;		
			case RUD:
				sprintf(szBuffer, "RUD  Tr    ms");				
				break;		
			case FLAPS:
				sprintf(szBuffer, "FLAPS  Tr    ms");				
				break;		
			case GEAR:
				sprintf(szBuffer, "GEAR  Tr    ms");				
				break;		
			case MIX:
				sprintf(szBuffer, "MIX  Tr    ms");				
				break;		
			case AUX1:
				sprintf(szBuffer, "AUX1  Tr    ms");				
				break;		
      	default:
				trims[SYNC] = 0;	/**Reset to home screen, if we don't know where we are **/
				return;
			}
			lcd_puts(szBuffer);
			lcd_gotoxy(0,1);
			sprintf(szBuffer, "%d %d %d", percent[trims[SYNC]], trims[trims[SYNC]], ppm[trims[SYNC]]);
			lcd_puts(szBuffer);
		#endif
}

int16_t mapPercent(uint16_t value, uint8_t trim){
	/** traversal range is always between -500 and 500 and the percent value, 200 points window for trims
	 	mapping needs to be between -100 and 100, so this is a shortcut
	**/
	int16_t midVal =(MIN_SIGNAL_WIDTH + trim + MAX_SIGNAL_WIDTH + trim)/2.0;
	int16_t m =(value - (midVal));
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
#ifdef USE_LCD_DISPLAY
	/** init LCD Display **/
	lcd_init(LCD_DISP_ON);
	/** set output **/
#endif	
	DDRD = 0x00;						/** Port D has the trim switches, make them digital input **/
	PORTD = 0xFF;

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
	
	/** set all trims to zero **/
	/** TODO: read trims from eeprom **/
	trims[AIL] = 2;
	trims[ELE] = TRIM_CENTER;
	trims[THR] = TRIM_CENTER;
	trims[RUD] = TRIM_CENTER;
	trims[FLAPS] = TRIM_CENTER;
	trims[GEAR] = TRIM_CENTER;
	trims[MIX] = TRIM_CENTER;
	trims[AUX1] = TRIM_CENTER;
	
	
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




















