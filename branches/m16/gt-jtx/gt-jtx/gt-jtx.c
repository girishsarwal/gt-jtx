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
#include <avr/eeprom.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

void reset();
void setupHardware();


void getAnalogChannelValue(uint8_t);
void getDigitalChannelValue(uint8_t);

uint16_t readAnalog(uint8_t);
uint16_t micros_to_ticks(uint16_t);

void incTrim(uint8_t channel);
void decTrim(uint8_t channel);

uint8_t getKeyPressed(uint8_t port, uint8_t key);
void processKeyInputs();
void processDisplay();
void loadParameters();
void calibrateChannel(uint8_t channel);
void loadGlobalSettings();
void saveGlobalSettings();
void saveCalibration();

#define START_FROM_SYNC_CHANNEL_WIDTH
//#define START_FROM_FIRST_CHANNEL_WIDTH
//#define START_FROM_INTER_CHANNEL_WIDTH

#define USE_LCD_DISPLAY

/** channel mapping to named items **/
#define SYNC	0
#define AIL		1
#define ELE		2
#define THR		3
#define RUD		4
#define FLAPS 	5

#define MAX_CHANNEL FLAPS
#define MAX_ANALOG_CHANNEL RUD
/** Screens -
0 - home screen,
1 - AUX1 - channel settings
MAX_CHANNEL + 1 - Global Settings
MAX_CHANNEL + 2 - Calibration
**/
#define HOME	0
#define SETTINGS    MAX_CHANNEL + 1
#define CALIBRATION MAX_CHANNEL + 2


#define TRIM_UPPER_END 200
#define TRIM_LOWER_END 0
#define TRIM_CENTER 100

#define TRIM_AIL_PLUS 0x01
#define TRIM_AIL_MINUS 0x02
#define TRIM_ELE_PLUS 0x04
#define TRIM_ELE_MINUS 0x08
#define TRIM_THR_PLUS 0x10
#define TRIM_THR_MINUS 0x20
#define TRIM_RUD_PLUS 0x40
#define TRIM_RUD_MINUS 0x80

#define MAGIC_NUMBER 29543              		
/**DO NOT CHANGE THIS **/


uint16_t EEMEM _eepromOk = MAGIC_NUMBER;			 	
uint8_t 	EEMEM _setupState = 0;					/** Default Setup Status: use to check first time setups **/
uint16_t EEMEM _minSignalWidth = 700;
uint16_t EEMEM _maxSignalWidth = 1700;
uint16_t EEMEM _interChannelWidth = 300;
uint16_t EEMEM _frameWidth = 22500;
uint8_t 	EEMEM _trims[MAX_CHANNEL + 1];
uint16_t	EEMEM _calibrationUpper[MAX_ANALOG_CHANNEL];
uint16_t	EEMEM _calibrationLower[MAX_ANALOG_CHANNEL];

uint16_t EEPROM_OK;
uint8_t SETUP_STATE;
uint16_t MAX_SIGNAL_WIDTH;
uint16_t MIN_SIGNAL_WIDTH;
uint16_t INTER_CHANNEL_WIDTH;
uint16_t FRAME_WIDTH;
uint16_t SIGNAL_TRAVERSAL;


uint16_t MID_SIGNAL_WIDTH;
uint16_t SYNC_SIGNAL_WIDTH;

#define STARTADC ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE);

int16_t ppm[MAX_CHANNEL + 1];	/** accomodate +1 for the sync channel **/
uint8_t trims[MAX_ANALOG_CHANNEL + 1];		/** max channels for trims. Although atm only first 4 are useful, trims will always range from -100 to +100 points **/
int8_t percent[MAX_ANALOG_CHANNEL + 1];
uint16_t calibrationUpper[MAX_ANALOG_CHANNEL];		/** we only need calibration for 1-4 on the sticks **/
uint16_t calibrationLower[MAX_ANALOG_CHANNEL];

char szBuffer[16];

static int channel = SYNC;

int main(){
	loadGlobalSettings();

	setupHardware();
	
   lcd_puts("#    gt-jtx    #");
	lcd_gotoxy(0, 1);
	lcd_puts("#     ver 1    #");
	_delay_ms(3000);	
	lcd_clrscr();
	
	if(EEPROM_OK != 29543){
		while(1){
			lcd_puts("EEPROM MISSING!");
			lcd_gotoxy(0, 1);
			lcd_puts("Load Def. EEPROM");
		}
	}

	loadDefaultCalibration();
	
	if(!((SETUP_STATE & 0x01 == 0x01)               /** check for Upper Calibration Limits **/
		|| ((SETUP_STATE & 0x02) ==0x02)) ) {			/** check for Lower Calibration Limits **/
			
		trims[SYNC] = CALIBRATION;							/** something is not set,
																		we will start with the calibration screen **/
	}
	else{
		loadCalibration();
		trims[SYNC] = HOME;
	}

	loadParameters();
	
	reset();

	/**Enable global interrupts **/
	sei();

	/** start our timer **/
	TIMSK |= (1<<OCIE1A);
	
	/** start the adc **/
	ADCSRA |= (1<<ADSC);

	while(1){
		/**read pot inputs first of all
		TODO: Do this on a timer and ISR**/
		getAnalogChannelValue(AIL);
		getAnalogChannelValue(ELE);
		getAnalogChannelValue(THR);
		getAnalogChannelValue(RUD);
		
		getDigitalChannelValue(FLAPS);

		processKeyInputs();
		processDisplay();
	}
};



uint8_t getKeyPressed(uint8_t port, uint8_t key){
	if(((~port) & key) == key)	
	{
		_delay_ms(1);	/** get rid of keybounce **/
		return (((~port) & key) == key);
	}	
	return 0x00;
}
void calibrateChannel(uint8_t ch)
{
	uint16_t calibration = readAnalog(ch);
	if(calibrationUpper[ch - 1] < calibration)
		calibrationUpper[ch - 1] = calibration;
	if(calibrationLower[ch - 1] > calibration)
		calibrationLower[ch - 1] = calibration;
}

void incTrim(uint8_t ch){
	if(++trims[ch] > TRIM_UPPER_END)
		trims[ch] = TRIM_UPPER_END;
}

void decTrim(uint8_t ch){
	if(trims[ch] > 0 && --trims[ch]<= TRIM_LOWER_END)
		trims[ch] = TRIM_LOWER_END;
}
void processKeyInputs(){
	/** keys behave differently on each screen **/
	switch(trims[SYNC]){
		case HOME:
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); trims[SYNC] = AIL; return;}	/** menu pressed for 250 ms**/
			if(getKeyPressed(PIND, TRIM_AIL_PLUS)) incTrim(AIL);
			if(getKeyPressed(PIND, TRIM_AIL_MINUS)) decTrim(AIL);
			if(getKeyPressed(PIND, TRIM_ELE_PLUS)) incTrim(ELE);
			if(getKeyPressed(PIND, TRIM_ELE_MINUS)) decTrim(ELE);
			if(getKeyPressed(PIND, TRIM_THR_PLUS)) incTrim(THR);
			if(getKeyPressed(PIND, TRIM_THR_MINUS)) decTrim(THR);
			if(getKeyPressed(PIND, TRIM_RUD_PLUS)) incTrim(RUD);
			if(getKeyPressed(PIND, TRIM_RUD_PLUS)) decTrim(RUD);
			break;
		case AIL...MAX_CHANNEL:
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); trims[SYNC] = HOME; return;}	/** menu pressed for 250 ms**/
			if(getKeyPressed(PIND, TRIM_AIL_PLUS)) { _delay_ms(250); trims[SYNC]++; }
			if(getKeyPressed(PIND, TRIM_AIL_MINUS)) { _delay_ms(250); trims[SYNC]--; }
			break;
		case SETTINGS:
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); saveGlobalSettings(); trims[SYNC] = HOME; return;}   /** menu pressed for 250 ms**/
			if(getKeyPressed(PIND, TRIM_AIL_PLUS)) { _delay_ms(250); trims[SYNC]++; }
			if(getKeyPressed(PIND, TRIM_AIL_MINUS)) { _delay_ms(250); trims[SYNC]--; }
			break;
		case CALIBRATION:
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); saveCalibration(); trims[SYNC] = HOME; return;}   /** menu pressed for 250 ms**/
			if(getKeyPressed(PIND, TRIM_AIL_PLUS)) { _delay_ms(250); trims[SYNC]++; }
			if(getKeyPressed(PIND, TRIM_AIL_MINUS)) { _delay_ms(250); trims[SYNC]--; }
			calibrateChannel(AIL);
			calibrateChannel(ELE);
			calibrateChannel(THR);
			calibrateChannel(RUD);
			break;
	}
}
void processDisplay(){
#ifdef USE_LCD_DISPLAY
	lcd_clrscr();
	lcd_home();
	switch(trims[SYNC]){
		case HOME:
			sprintf(szBuffer, "A:%d E:%d", percent[AIL], percent[ELE]);
			lcd_puts(szBuffer);
			lcd_gotoxy(0,1);
			sprintf(szBuffer, "T:%d R:%d", percent[THR], percent[RUD]);
			lcd_puts(szBuffer);
			return;
		case AIL:
			sprintf(szBuffer, "AIL    Tr  ms");
			break;
		case ELE:
			sprintf(szBuffer, "ELE    Tr  ms");				
			break;
		case THR:
			sprintf(szBuffer, "THR    Tr  ms");				
			break;		
		case RUD:
			sprintf(szBuffer, "RUD    Tr  ms");				
			break;		
		case FLAPS:
			sprintf(szBuffer, "FLAPS      ms");				
			break;		
		case SETTINGS:
			lcd_puts("SAVE SETTINGS?");
			lcd_gotoxy(0, 1);
			lcd_puts("  Menu to save  ");
			return;
		case CALIBRATION:
			lcd_puts("CAL-Menu to save");
			lcd_gotoxy(0, 1);
			lcd_puts("Mov AETR Min&Max");
			return;
     	default:
			trims[SYNC] = AIL;	/**Reset to home screen, if we don't know where we are **/
			return;
			}
		lcd_puts(szBuffer);
		lcd_gotoxy(0,1);
		sprintf(szBuffer, "%d   %d  %d", percent[trims[SYNC]], trims[trims[SYNC]], ppm[trims[SYNC]]);
		lcd_puts(szBuffer);
#endif
}

void getAnalogChannelValue(uint8_t ch){
	/** read up the canonical channel value**/
	uint16_t value = readAnalog(ch);
	float calibratedChannelRange = calibrationUpper[ch - 1] - calibrationLower[ch - 1];
	float stickMoveRatio = (value - calibrationLower[ch - 1])/calibratedChannelRange;
	
	ppm[ch] = MIN_SIGNAL_WIDTH + ((stickMoveRatio * (float)SIGNAL_TRAVERSAL)) + trims[ch];

	percent[ch] = (200 * stickMoveRatio) - 100;
}

void getDigitalChannelValue(uint8_t ch){
	/** read up the canonical channel value**/
	uint16_t value = readAnalog(ch);
	if(value < 512){
		ppm[ch] = MAX_SIGNAL_WIDTH + TRIM_CENTER;
		percent[ch] = 100;
	}
	else{
		ppm[ch] = MIN_SIGNAL_WIDTH + TRIM_CENTER;
		percent[ch] = -100;
	}
}


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

void loadGlobalSettings(){
	EEPROM_OK = eeprom_read_word(&_eepromOk);								/** read eeprom status **/
	SETUP_STATE = eeprom_read_byte(&_setupState);                  /** read setup status **/
	MIN_SIGNAL_WIDTH = eeprom_read_word(&_minSignalWidth);			/** read signal lengths **/
	MAX_SIGNAL_WIDTH = eeprom_read_word(&_maxSignalWidth);
	INTER_CHANNEL_WIDTH = eeprom_read_word(&_interChannelWidth);	
	FRAME_WIDTH = eeprom_read_word(&_frameWidth);
};

void loadModelSettings(){
	/** load trim settings **/
};

void loadParameters(){
	MID_SIGNAL_WIDTH = ((MAX_SIGNAL_WIDTH + MIN_SIGNAL_WIDTH)/2) + TRIM_CENTER;
	SYNC_SIGNAL_WIDTH = (FRAME_WIDTH - (MAX_CHANNEL * (MID_SIGNAL_WIDTH + INTER_CHANNEL_WIDTH))); /**whatever is left **/
	SIGNAL_TRAVERSAL = MAX_SIGNAL_WIDTH - MIN_SIGNAL_WIDTH;
};

void saveGlobalSettings(){
	eeprom_write_word(&_minSignalWidth, MIN_SIGNAL_WIDTH);
	eeprom_write_word(&_maxSignalWidth, MAX_SIGNAL_WIDTH);
	eeprom_write_word(&_interChannelWidth, INTER_CHANNEL_WIDTH);
	eeprom_write_word(&_frameWidth, FRAME_WIDTH);
};
void loadDefaultCalibration(){
	/** load the default settings **/
	calibrationUpper[AIL - 1] = 0;
	calibrationUpper[ELE - 1] = 0;
	calibrationUpper[THR - 1] = 0;
	calibrationUpper[RUD - 1] = 0;
	
	calibrationLower[AIL - 1] = 1024;
	calibrationLower[ELE - 1] = 1024;
	calibrationLower[THR - 1] = 1024;
	calibrationLower[RUD - 1] = 1024;
}
void loadCalibration(){
	eeprom_read_block((void*)&calibrationUpper, (const void*)_calibrationUpper, 2 * MAX_ANALOG_CHANNEL);	
	eeprom_read_block((void*)&calibrationLower, (const void*)_calibrationLower, 2 * MAX_ANALOG_CHANNEL);	
};

void saveCalibration(){
	eeprom_write_block((void*)&calibrationUpper, (const void*)_calibrationUpper, 2 * MAX_ANALOG_CHANNEL);
	eeprom_write_block((void*)&calibrationLower, (const void*)_calibrationLower, 2 * MAX_ANALOG_CHANNEL);
	SETUP_STATE |= (1<<0) | (1<<1);
	eeprom_write_byte(&_setupState, SETUP_STATE);		/**write back byte to the calibration**/
};

void setupHardware(){

#ifdef USE_LCD_DISPLAY
	/** init LCD Display **/
	lcd_init(LCD_DISP_ON);
	/** set output **/
#endif	
	/** set inputs **/				
	DDRC = 0x00;						/** PORTC in the input for ADC control on ATmega8**/
											/** PC0 = Menu key
											 ** PC1 = AIL Input
											 ** PC2 = ELE Input
											 ** PC3 = THR Input
											 ** PC4 = RUD Input
											 ** PC5 = FLAP Input **/
 	PORTC = 0x01;           		/** Activate Pull Up resistor on MENU KEY ONLY 0b00000001 **/

	DDRD = 0x00;						/** Port D has the trim switches, make them digital input **/
											/** PD0 = AIL+
											 ** PD1 = AIL-
											 ** PD2 = ELE+
											 ** PD3 = ELE-
											 ** PD4 = THR+
											 ** PD5 = THR-
											 ** PD6 = RUD+
											 ** PD7 = RUD- **/
	PORTD = 0xFF; 						/** Activate all pull up resistors on port D 0b11111111 */

	DDRB = 0xFF;						/** make PB1 as out pin **/
	PORTB = 0x20;						/** make PB1 as low **/
	TCNT1 = 0;
	
	TCCR1B |= (1<<CS10) ;				/** F_CPU - 1Mhz Timebase **/
	TCCR1B	|=	(1<<WGM12);				/** CTC Mode**/

	TCCR1A |= (1<<COM1A0);				/** hardware ctc **/
	
	ADMUX |= (1<<REFS0);				
	ADCSRA |= (1<<ADEN); 				/** enable the ADC **/
	ADCSRA|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	/**F_CPU/64 Prescalar **/
	
};


uint16_t readAnalog(uint8_t ch){
	ch = ch & 0x07;					/** zero out all bits except last three for limiting channel **/
	ADMUX &=0xE0;						/**reset channel selection **/
	ADMUX |=ch;							/**select the requested channel **/
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|= (1<<ADIF);
	return (ADC);
};

void reset(){
	/** set values to match servo spec **/
	ppm[SYNC] = SYNC_SIGNAL_WIDTH;
	ppm[AIL] = MID_SIGNAL_WIDTH;
	ppm[ELE] = MID_SIGNAL_WIDTH;
	ppm[THR] = MID_SIGNAL_WIDTH;
	ppm[RUD] = MID_SIGNAL_WIDTH;
	ppm[FLAPS] = MID_SIGNAL_WIDTH;
	
	/** set all trims to zero **/
	/** TODO: read trims from eeprom **/
	
	trims[AIL] = TRIM_CENTER;
	trims[ELE] = TRIM_CENTER;
	trims[THR] = TRIM_CENTER;
	trims[RUD] = TRIM_CENTER;

	
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
