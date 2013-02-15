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

#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <math.h>
#include "lcd.h"

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
#define SETTINGS1   		MAX_CHANNEL + 1
#define SETTINGS2   		MAX_CHANNEL + 2
#define SAVE_SETTINGS 	MAX_CHANNEL + 3
#define CALIBRATION 		MAX_CHANNEL + 4



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

/** EEProm Definitions **/
uint16_t 	EEMEM _eepromOk = MAGIC_NUMBER;			 	
uint8_t 		EEMEM _setupState = 0;					/** Default Setup Status: use to check first time setups **/
uint16_t 	EEMEM _minSignalWidth = 700;
uint16_t 	EEMEM _maxSignalWidth = 1700;
uint16_t 	EEMEM _interChannelWidth = 300;
uint16_t 	EEMEM _frameWidth = 22500;
uint8_t 		EEMEM _trims[MAX_CHANNEL + 1] = {0, 100, 100, 100, 100, 100};
uint8_t 		EEMEM _reverse = 0x00;
uint16_t		EEMEM _calibrationUpper[MAX_ANALOG_CHANNEL];
uint16_t		EEMEM _calibrationLower[MAX_ANALOG_CHANNEL];

uint16_t 	EEPROM_OK;
uint8_t 		SETUP_STATE;
uint16_t 	MAX_SIGNAL_WIDTH;
uint16_t 	MIN_SIGNAL_WIDTH;
uint16_t 	INTER_CHANNEL_WIDTH;
uint16_t 	FRAME_WIDTH;
uint8_t		trims[MAX_CHANNEL + 1];				/** max channels for trims. Although atm only first 4 are useful,
															trims will always range from 0 to 200 points **/
uint8_t		reverse;									/**reverse settings - 0 means not reversed, 1 means channel is reversed 0bXXX00010**/
int8_t 		percent[MAX_CHANNEL + 1];
uint16_t 	calibrationUpper[MAX_ANALOG_CHANNEL];		/** we only need calibration for 1-4 on the sticks **/
uint16_t 	calibrationLower[MAX_ANALOG_CHANNEL];

uint16_t 	SIGNAL_TRAVERSAL;
uint16_t 	MID_SIGNAL_WIDTH;
uint16_t 	SYNC_SIGNAL_WIDTH;
int16_t		ppm[MAX_CHANNEL + 1];	/** accomodate +1 for the sync channel **/

char* 		pszBuffer = 0;
uint8_t		currentScreen = 0;

/** Character strings **/
const char* _strTitle = "#    gt-jtx    #";
const char* _strVersion = "#   m8-ver-1   #";
const char* _strPosVal = "+%d";
const char* _strNegVal = "%d";
const char* _strEepromError = "EEPROM MISSING!";
const char* _strLoadDefEeprom = "Load Def. EEPROM";
const char* _strHome1 = "A:     E:     FL";
const char* _strHome2 = "T:     R:       ";
const char* _strAil = "AIL";
const char* _strEle = "ELE";
const char* _strThr = "THR";
const char* _strRud = "RUD";
const char* _strFlaps = "FLAPS";
const char* _strMs ="ms";
const char* _strTrim = "Trim";
const char* _strState = "St";
const char* _strReverse = "R";
const char* _strYes = "Y";
const char* _strNo = "N";
const char* _strGlobalSettings1Title = "Sig. Width (ms)";
const char* _strGlobalSettings2Title = "IntCh. Frm. (us)";
const char* _strSaveSettings = "SAVE SETTINGS?";
const char* _strMenuToSave = "  Menu to save  ";
const char* _strMoveAetrMinMax = "Mov AETR Min&Max";
const char* _strSeparator = " ";
const char* _strMin = "Min";
const char* _strMax = "Max";

/** Function Declaratons **/
void reset();
void setupHardware();
void getAnalogChannelValue(uint8_t);
void getDigitalChannelValue(uint8_t);
uint16_t readAnalog(uint8_t);
uint16_t micros_to_ticks(uint16_t);
void loadModelSettings();
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
void checkNavigation();
void saveReverse();

#define STARTADC ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE);

static int channel = SYNC;

int main(){
	pszBuffer = malloc(5 * sizeof(char));
	

	/** start routines **/
	loadGlobalSettings();
	setupHardware();


/** Splash Screen **/
   lcd_puts(_strTitle);
	lcd_gotoxy(0, 1);
	lcd_puts(_strVersion);
//	_delay_ms(1000);	
	lcd_clrscr();
	
	
	/** check EEPROM Sanity **/
	if(EEPROM_OK != 29543){
		while(1){
			lcd_puts(_strEepromError);
			lcd_gotoxy(0, 1);
			lcd_puts(_strLoadDefEeprom);
		}
	}

	loadDefaultCalibration();								/** Load Default Calibration Settings **/

	if(!((SETUP_STATE & 0x01 == 0x01)               /** check for Upper Calibration Limits **/
		|| ((SETUP_STATE & 0x02) ==0x02)) ) {			/** check for Lower Calibration Limits **/
			
		currentScreen = CALIBRATION;							/** something is not set,
																		we will start with the calibration screen **/
	}
	else{
		loadCalibration();
		currentScreen = HOME;
	}
	
	loadParameters();
	loadModelSettings();	
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
void saveReverse(){
	eeprom_write_byte(&_reverse, reverse);
}
void checkNavigation(){
	if(getKeyPressed(PIND, TRIM_AIL_PLUS)) { _delay_ms(250); currentScreen++; }
	if(getKeyPressed(PIND, TRIM_AIL_MINUS)) { _delay_ms(250); currentScreen--; }
}
void loadModelSettings(){
	/** read the trims **/
	eeprom_read_block(&trims, _trims, sizeof(char) * 4);
	/** read the reverse settings **/
	reverse = eeprom_read_byte(&_reverse);
	
}
void processKeyInputs(){
	/** keys behave differently on each screen **/
	switch(currentScreen){
		case HOME:
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); currentScreen = AIL; return;}	/** menu pressed for 250 ms**/
			if(getKeyPressed(PIND, TRIM_AIL_PLUS)) incTrim(AIL);
			if(getKeyPressed(PIND, TRIM_AIL_MINUS)) decTrim(AIL);
			if(getKeyPressed(PIND, TRIM_ELE_PLUS)) incTrim(ELE);
			if(getKeyPressed(PIND, TRIM_ELE_MINUS)) decTrim(ELE);
			if(getKeyPressed(PIND, TRIM_THR_PLUS)) incTrim(THR);
			if(getKeyPressed(PIND, TRIM_THR_MINUS)) decTrim(THR);
			if(getKeyPressed(PIND, TRIM_RUD_PLUS)) incTrim(RUD);
			if(getKeyPressed(PIND, TRIM_RUD_MINUS)) decTrim(RUD);
			break;
		case AIL...MAX_CHANNEL:
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); currentScreen = HOME; return;}	/** menu pressed for 250 ms**/
			checkNavigation();
			if(getKeyPressed(PIND, TRIM_ELE_PLUS)) { incTrim(currentScreen); }
			if(getKeyPressed(PIND, TRIM_ELE_MINUS)) { decTrim(currentScreen); }
			if(getKeyPressed(PIND, TRIM_THR_PLUS)) { reverse |= (1<<(currentScreen - 1));	}
			if(getKeyPressed(PIND, TRIM_THR_MINUS)) {	reverse &= (~(1 << (currentScreen - 1)));	}
			break;
		case SETTINGS1:
			checkNavigation();
			if(getKeyPressed(PIND, TRIM_ELE_PLUS)) { MIN_SIGNAL_WIDTH--; }
			if(getKeyPressed(PIND, TRIM_ELE_MINUS)) { MIN_SIGNAL_WIDTH++; }
			if(getKeyPressed(PIND, TRIM_THR_PLUS)) { MAX_SIGNAL_WIDTH++; }
			if(getKeyPressed(PIND, TRIM_THR_MINUS)) { MAX_SIGNAL_WIDTH--; }
			break;
		case SETTINGS2:
			checkNavigation();
			if(getKeyPressed(PIND, TRIM_ELE_PLUS)) { INTER_CHANNEL_WIDTH--; }
			if(getKeyPressed(PIND, TRIM_ELE_MINUS)) { INTER_CHANNEL_WIDTH++; }
			if(getKeyPressed(PIND, TRIM_THR_PLUS)) { FRAME_WIDTH++; }
			if(getKeyPressed(PIND, TRIM_THR_MINUS)) { FRAME_WIDTH--; }
			
			break;
		case SAVE_SETTINGS:
			checkNavigation();
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); saveGlobalSettings(); saveReverse(); currentScreen = HOME; return;}   /** menu pressed for 250 ms**/
			break;
		case CALIBRATION:
			checkNavigation();
			if(getKeyPressed(PINC, 0x01)) {_delay_ms(250); saveCalibration(); currentScreen = HOME; return;}   /** menu pressed for 250 ms**/
			calibrateChannel(AIL);
			calibrateChannel(ELE);
			calibrateChannel(THR);
			calibrateChannel(RUD);
			break;
	}
}
void showAnalogChannelSettings(){
	lcd_puts(_strSeparator);lcd_puts(_strTrim);lcd_puts(_strSeparator);lcd_puts(_strSeparator);lcd_puts(_strReverse);lcd_puts(_strSeparator);lcd_puts(_strMs);
	sprintf(pszBuffer, percent[currentScreen] >= 0?_strPosVal:_strNegVal, percent[currentScreen]);lcd_gotoxy(0, 1);lcd_puts(pszBuffer);
	sprintf(pszBuffer, _strNegVal, trims[currentScreen - 1]);lcd_gotoxy(5, 1); lcd_puts(pszBuffer);
	uint8_t mask = (1 << (currentScreen - 1));
	lcd_gotoxy(10, 1); lcd_puts((reverse & mask) == mask ? _strYes: _strNo);
	sprintf(pszBuffer, _strNegVal, ppm[currentScreen]);lcd_gotoxy(12, 1); lcd_puts(pszBuffer);		
}
void showDigitalChannelSettings(){
	lcd_puts(_strSeparator);lcd_puts(_strState);lcd_puts(_strSeparator);lcd_puts(_strSeparator);lcd_puts(_strReverse);lcd_puts(_strSeparator);lcd_puts(_strMs);
	sprintf(pszBuffer, percent[currentScreen] >= 0?_strPosVal:_strNegVal, percent[currentScreen]);lcd_gotoxy(0, 1);lcd_puts(pszBuffer);
	lcd_gotoxy(6, 1); lcd_puts(percent[currentScreen] >= 0? _strYes: _strNo);
	uint8_t mask = (1 << (currentScreen - 1));
	lcd_gotoxy(10, 1); lcd_puts((reverse & mask)== mask ? _strYes: _strNo);
	sprintf(pszBuffer, _strNegVal, ppm[currentScreen]);lcd_gotoxy(12, 1); lcd_puts(pszBuffer);
}
void processDisplay(){
#ifdef USE_LCD_DISPLAY
	lcd_clrscr();
	lcd_home();
	switch(currentScreen){
		case HOME:
			lcd_puts(_strHome1);
			lcd_gotoxy(0,1);
			lcd_puts(_strHome2);
			sprintf(pszBuffer, percent[AIL] >= 0 ?_strPosVal:_strNegVal, percent[AIL]); lcd_gotoxy(2, 0); lcd_puts(pszBuffer);
			sprintf(pszBuffer, percent[ELE] >= 0 ?_strPosVal:_strNegVal, percent[ELE]); lcd_gotoxy(9, 0); lcd_puts(pszBuffer);
			sprintf(pszBuffer, percent[THR] >= 0 ?_strPosVal:_strNegVal, percent[THR]); lcd_gotoxy(2, 1); lcd_puts(pszBuffer);
			sprintf(pszBuffer, percent[RUD] >= 0 ?_strPosVal:_strNegVal, percent[RUD]); lcd_gotoxy(9, 1); lcd_puts(pszBuffer);			
			lcd_gotoxy(14, 1);
			lcd_puts(percent[FLAPS] > 0? _strYes:_strNo);			
			_delay_ms(100);			
			return;
		case AIL:			
			lcd_puts(_strAil);showAnalogChannelSettings();
			break;
		case ELE:
			lcd_puts(_strEle);showAnalogChannelSettings();			
			break;
		case THR:
			lcd_puts(_strThr);showAnalogChannelSettings();			
			break;		
		case RUD:
			lcd_puts(_strRud);showAnalogChannelSettings();			
			break;		
		case FLAPS:
			lcd_puts(_strFlaps);showDigitalChannelSettings();						
			break;		
		case SETTINGS1:
			lcd_puts(_strGlobalSettings1Title);
			lcd_gotoxy(0, 1);
			lcd_puts(_strMin);
			sprintf(pszBuffer, _strNegVal, MIN_SIGNAL_WIDTH + TRIM_CENTER);lcd_puts(pszBuffer);
			lcd_gotoxy(8, 1);
			lcd_puts(_strMax);
			sprintf(pszBuffer, _strNegVal, MAX_SIGNAL_WIDTH + TRIM_CENTER);lcd_puts(pszBuffer);
			break;
		case SETTINGS2:
			lcd_puts(_strGlobalSettings2Title);
			lcd_gotoxy(0, 1);		
			sprintf(pszBuffer, _strNegVal, INTER_CHANNEL_WIDTH);lcd_puts(pszBuffer);
			lcd_gotoxy(8, 1);
			sprintf(pszBuffer, _strNegVal, FRAME_WIDTH);lcd_puts(pszBuffer);
			break;
		case SAVE_SETTINGS:
			lcd_puts(_strSaveSettings);
			lcd_gotoxy(0, 1);
			lcd_puts(_strMenuToSave);
			return;
		case CALIBRATION:
			lcd_puts(_strMoveAetrMinMax);
			lcd_gotoxy(0, 1);
			lcd_puts(_strMenuToSave);
			return;
     	default:
			currentScreen = AIL;	/**Reset to home screen, if we don't know where we are **/
			return;
			}
#endif
	_delay_ms(100);			
}

void getAnalogChannelValue(uint8_t ch){
	/** read up the canonical channel value**/
	uint16_t value = readAnalog(ch);
	float calibratedChannelRange = calibrationUpper[ch - 1] - calibrationLower[ch - 1];
	float stickMoveRatio = (value - calibrationLower[ch - 1])/calibratedChannelRange;
	
	/**adjust for reverse **/
	uint8_t mask = (1<< (ch -1));
	if ((reverse & mask) == mask)	 stickMoveRatio = 1 - stickMoveRatio;
	
	ppm[ch] = MIN_SIGNAL_WIDTH + ((stickMoveRatio * (float)SIGNAL_TRAVERSAL)) + trims[ch];

	percent[ch] = (200 * stickMoveRatio) - 100;
}

void getDigitalChannelValue(uint8_t ch){
	/** read up the canonical channel value**/
	uint16_t value = readAnalog(ch);
	/** we need to do a xor here to swap condition for reverse and signal length **/
	uint8_t mask = (1 << (ch -1));
	
	uint8_t xoredSignal = (value < 512) ^ ((reverse & mask) == mask);
	
	ppm[ch] = (xoredSignal ? MIN_SIGNAL_WIDTH : MAX_SIGNAL_WIDTH) + TRIM_CENTER;
	percent[ch] = xoredSignal ? -100 : 100;
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






