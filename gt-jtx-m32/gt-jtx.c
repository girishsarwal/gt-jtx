/*
 * gt-jtx.cpp
 *
 * Created: 20-09-2012 15:08:33
 *  Author: Girish Sarwal
 *	 Email: g.sarwal@gmail.com
 *
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "glcd/glcd.h"
#include "tx.h"

int16_t		ppm[SYNC + 1];					/** SYNC is the last channel in a zero based index accomodate +1 for the sync channel **/

/** Function Declaratons **/
uint16_t readAnalog(uint8_t);
uint16_t readChannelValue(uint8_t);

uint16_t micros_to_ticks(uint16_t);
uint8_t getKeyPressed(uint8_t port, uint8_t key);

void setupHardware();

void incTrim(uint8_t channel);
void decTrim(uint8_t channel);


void calibrateChannel(uint8_t channel);

ISR(SPI_STC_vect);

#define STARTADC ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE);

uint16_t _dirty = 0xFFFF;

static int channel = SYNC;



struct TX EEMEM MAINTX;
struct TX maintx;

void setup(){
	
	/* setup the hardware **/
	setupHardware();
	
	/* check transmitter status **/

	tx_load_from_eeprom(&maintx, &MAINTX);
	if(maintx.eeprom_state != MAGIC_NUMBER){
		tx_load_defaults(&maintx);
	}
	if((maintx.setup_state && UPPER_CALIBRATION_MISSING) == UPPER_CALIBRATION_MISSING){
		tx_load_default_upper_calibration(&maintx);
	}
	if((maintx.setup_state && LOWER_CALIBRATION_MISSING) == LOWER_CALIBRATION_MISSING){
		tx_load_default_lower_calibration(&maintx);
	}
	tx_calculate_parameters(&maintx);
	tx_save_to_eeprom(&maintx, &MAINTX);
	
}

int main(){
	setup();
	/**Enable global interrupts **/
	sei();
	/** start our timer **/
	TIMSK |= (1<<OCIE1A);
	/** start the adc **/
	ADCSRA |= (1<<ADSC);

	uint8_t idx = -1;
	while(1){
		idx = -1;
		while(++idx <= MAX_CHANNEL){
			ppm[idx] = readChannelValue(idx);
		}
		ppm[SYNC] = maintx.sync_signal_width;
	}
}


uint8_t getKeyPressed(uint8_t port, uint8_t key){
	if(((~port) & (1 << key)) == (1 << key))	
	{
		_delay_ms(1);	/** get rid of keybounce **/
		return (((~port) & (1 << key)) == (1 << key));
	}	
	return 0x00;
}

/**void calibrateChannel(uint8_t ch)
{
	uint16_t calibration = readAnalog(ch);		
	if(calibrationUpper[ch] < calibration)
		calibrationUpper[ch] = calibration;
	if(calibrationLower[ch] > calibration)
		calibrationLower[ch] = calibration;
}

void incTrim(uint8_t ch){
	if(++trims[ch] > TRIM_UPPER_END){
		trims[ch] = TRIM_UPPER_END;
	}
	eeprom_write_byte(&_trims[ch], trims[ch]);
}

void decTrim(uint8_t ch){
	if(trims[ch] > 0 && --trims[ch]<= TRIM_LOWER_END){
		trims[ch] = TRIM_LOWER_END;
	}
	eeprom_write_byte(&_trims[ch], trims[ch]);
}
void processKeyInputs(){
	//if(getKeyPressed(PIN_MENU, SIG_MENU_KEY)) {_delay_ms(250); currentScreen = AIL; return;}	menu pressed for 250 ms
	//if(getKeyPressed(PIN_TRIM, SIG_TRIM_AIL_PLUS)) incTrim(AIL);
	//if(getKeyPressed(PIN_TRIM, SIG_TRIM_AIL_MINUS)) decTrim(AIL);
	//if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_PLUS)) incTrim(ELE);

}**/

uint16_t readChannelValue(uint8_t ch){
	
	uint16_t value = readAnalog(ch);		/** read up the canonical channel value**/
	
	float calibratedChannelRange = maintx.calibration_upper[ch] - maintx.calibration_lower[ch];
	float stickMoveRatio = (value - maintx.calibration_lower[ch])/calibratedChannelRange;			/** calculate a ratio **/

	uint8_t mask = (1 << ch);
	
	if ((maintx.model.reverse & mask) == mask) stickMoveRatio = 1 - stickMoveRatio;				/** invert for reverse **/
	
	return maintx.min_signal_width + ((stickMoveRatio * (float)maintx.signal_traversal)) + maintx.model.trims[ch];
}

uint16_t micros_to_ticks(uint16_t value){
	return (value * ((F_CPU/1000)/1000));
};

ISR(TIMER1_COMPA_vect){
	TIMSK &= ~(1 << OCIE1A);
	if((PIN_PPM & (1 << SIG_PPM)) == (1 << SIG_PPM)){		//If the actual pin is high, we need to set OCR1A to the complementary delay
		if(++channel > (NUM_CHANNELS)){
			channel = 0;
		}
		OCR1A = micros_to_ticks(ppm[channel]);
	}		
	else{
		OCR1A = micros_to_ticks(maintx.inter_channel_width);
   }
	TIMSK |= (1<<OCIE1A);
};


void setupHardware(){
	/** setup lcd **/
	glcd_init();
	/** Setup I/O **/
	/** Analog Inputs**/
	DDR_ANALOG = 0x00;						
	PORT_ANALOG = 0x00;				/**Deactivate any pull ups **/
											/**
											 ** PC0 = AIL Input
											 ** PC1 = ELE Input
											 ** PC2 = THR Input
											 ** PC3 = RUD Input
											 ** PC4 = CH5 Input
											 ** PC5 - CH6 Input
											 ** PC6 - CH7 Input
											 ** PC7 - Ch8 Input
											**/
 	
 	/** Trim switches **/
 	DDR_TRIM = 0x00;
 	PORT_TRIM = 0xFF;					/**Activate all pull ups on Port B **/ 	
 	
 	
 	/** PORTD has D:0-D:4 for LCD_CTRL, make D5 (MENU), D6 (SPEAKER), D7 (PPM) as output pins **/
 	DDR_MENU &= ~(1 << SIG_MENU_KEY);					/** D:7 is MENU KEY = 0**/
 	PORT_MENU |= (1 << SIG_MENU_KEY);
  	DDR_SPEAK |= (1 << SIG_SPEAK);						/** D:6 is SPEAKER = 1**/
 	DDR_PPM |= (1 << SIG_PPM);								/** D:5 is PPM = 1**/
 	
 	
	TCNT1 = 0;
	
	TCCR1B |= (1<<CS10) ;									/** F_CPU - 1Mhz Timebase **/
	TCCR1B	|=	(1<<WGM12);									/** CTC Mode**/

	TCCR1A |= (1<<COM1A0);									/** hardware ctc **/
	
	ADMUX |= (1<<REFS0);				
	ADCSRA |= (1<<ADEN); 									/** enable the ADC **/
	ADCSRA|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	/**F_CPU/64 Prescalar **/
};


uint16_t readAnalog(uint8_t ch){
	ch = ch & 0x07;					/** zero out all bits except last three for limiting channel **/
	ADMUX &=0xE0;								/**reset channel selection **/
	ADMUX |=ch;									/**select the requested channel **/
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|= (1<<ADIF);
	return (ADC);
};

void reset_ppm_values(){
	/** set values to match servo spec **/
	ppm[SYNC] = maintx.sync_signal_width;
	uint8_t idx = 0;
	while(++idx <= NUM_CHANNELS){
		ppm[idx] = maintx.min_signal_width;	
	}
	
	/** reset to the first channel **/
	channel = SYNC;
	OCR1A = micros_to_ticks(ppm[SYNC]);
};

