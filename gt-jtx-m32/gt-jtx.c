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

#define DEBUG

#include <avr/io.h>

#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <math.h>
#include "ks0108.h"
#include "arial_10.h"


/** channel mapping to named items **/

#define AIL		0
#define ELE		1
#define THR		2
#define RUD		3
#define CH5 	4
#define CH6		5
#define CH7		6
#define CH8		7
#define SYNC	8

#define MAX_CHANNEL CH8
#define NUM_CHANNELS MAX_CHANNEL + 1

#define HOME				SYNC
#define SETTINGS1   		SYNC + 1
#define SETTINGS2   		SYNC + 2
#define SAVE_SETTINGS 	SYNC + 3
#define CALIBRATION 		SYNC + 4



#define TRIM_UPPER_END 200
#define TRIM_LOWER_END 0
#define TRIM_CENTER 100

#define SIG_TRIM_AIL_PLUS 		0
#define SIG_TRIM_AIL_MINUS 	1
#define SIG_TRIM_ELE_PLUS 		2
#define SIG_TRIM_ELE_MINUS 	3
#define SIG_TRIM_THR_PLUS 		4
#define SIG_TRIM_THR_MINUS 	5
#define SIG_TRIM_RUD_PLUS 		6
#define SIG_TRIM_RUD_MINUS 	7

#define PORT_ANALOG	PORTA
#define DDR_ANALOG	DDRA
#define PIN_ANALOG	PINA

#define PORT_TRIM		PORTB
#define DDR_TRIM		DDRB
#define PIN_TRIM		PINB

#define PORT_PPM		PORTD
#define DDR_PPM		DDRD
#define PIN_PPM		PIND

#define PORT_MENU		PORTD
#define DDR_MENU		DDRD
#define PIN_MENU		PIND

#define PORT_SPEAK	PORTD
#define DDR_SPEAK		DDRD


#define SIG_PPM		5
#define SIG_SPEAK		6
#define SIG_MENU_KEY 7

#define TRIM_TRACK_WIDTH 2
#define CHANNEL_TRACK_WIDTH 7


#define MAGIC_NUMBER 29543              		
/**DO NOT CHANGE THIS **/

/** EEProm Definitions **/
uint16_t 	EEMEM _eepromOk = MAGIC_NUMBER;			 	
uint8_t 		EEMEM _setupState = 3;					/** Default Setup Status: use to check first time setups **/
uint16_t 	EEMEM _minSignalWidth = 700;
uint16_t 	EEMEM _maxSignalWidth = 1700;
uint16_t 	EEMEM _interChannelWidth = 300;
uint16_t 	EEMEM _frameWidth = 22500;
uint8_t 		EEMEM _trims[NUM_CHANNELS] = {100, 100, 100, 100, 100, 100, 100, 100};
uint8_t 		EEMEM _reverse = 0x00;
uint16_t		EEMEM _calibrationUpper[NUM_CHANNELS] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
uint16_t		EEMEM _calibrationLower[NUM_CHANNELS] = {0, 0, 0, 0, 0, 0, 0, 0};

uint16_t 	EEPROM_OK;
uint8_t 		SETUP_STATE;
uint16_t 	MAX_SIGNAL_WIDTH;
uint16_t 	MIN_SIGNAL_WIDTH;
uint16_t 	INTER_CHANNEL_WIDTH;
uint16_t 	FRAME_WIDTH;
uint8_t		trims[NUM_CHANNELS];				/** max channels for trims. Although atm only first 4 are useful,
															trims will always range from 0 to 200 points **/
uint8_t		reverse;									/**reverse settings - 0 means not reversed, 1 means channel is reversed 0bXXX00010**/
int8_t 		percent[NUM_CHANNELS];
uint16_t 	calibrationUpper[NUM_CHANNELS];		
uint16_t 	calibrationLower[NUM_CHANNELS];

uint16_t 	SIGNAL_TRAVERSAL;
uint16_t 	MID_SIGNAL_WIDTH;
uint16_t 	SYNC_SIGNAL_WIDTH;
int16_t		ppm[SYNC + 1];					/** SYNC is the last channel in a zero based index accomodate +1 for the sync channel **/

char* 		pszBuffer = 0;
uint8_t		currentScreen = 0;

/** Character strings **/
char* _pilotName = "Girish Sarwal";
const char* _strTitle = "gt-jtx";
const char* _strVersion = "m32";
const char* _strPosVal = "+%d";
const char* _strNegVal = "%d";
const char* _strEepromError = "EEPROM MISSING!";
const char* _strLoadDefEeprom = "Load Def. EEPROM";
const char* _strHome1 = "A:     E:     FL";
const char* _strHome2 = "T:     R:       ";
const char* _strAil = "Aileron Settings (CH1)";
const char* _strEle = "Elevator Settings (CH2)";
const char* _strThr = "Throttle Settings (CH3)";
const char* _strRud = "Rudder Settings (CH4)";
const char* _strCh5 = "CH5 Settings";
const char* _strCh6 = "CH6 Settings";
const char* _strCh7 = "CH7 Settings";
const char* _strCh8 = "CH8 Settings";
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
const char* _strCalibrationTitle = "CALIBRATION";
const char* _strMoveSticksPots = "Move all sticks and pots to limit";
const char* _strFlipSwitches =   "and flip all switches off and on.";
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

void loadPreCalibration();
void loadCaibration();
void calibrateChannel(uint8_t channel);
void saveCalibration();

void loadGlobalSettings();
void saveGlobalSettings();

void checkNavigation();
void saveReverse();

#define STARTADC ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE);

uint16_t _dirty = 0xFFFF;

static int channel = SYNC;

int main(){
	pszBuffer = malloc(5 * sizeof(char));

	setupHardware();

	/** TODO:Splash Screen **/	
//	ks0108GotoXY(5, 10);
//	ks0108Puts(_strTitle);
//	ks0108GotoXY(70, 10);
//	ks0108Puts(_strVersion);
//	_delay_ms(2000);

//	ks0108DrawCirc(64, 32, 30, BLACK);
	/** check EEPROM Sanity **/
	EEPROM_OK = eeprom_read_word(&_eepromOk);								/** read eeprom status **/
	
	if(EEPROM_OK != 29543){
		while(1){
		/**TODO: EEPROM ERROR MESSAGE **/
		}
	}

	loadGlobalSettings();
	loadParameters();
   loadModelSettings();	
	
   SETUP_STATE = eeprom_read_byte(&_setupState);                  /** read setup status **/
	if(!((SETUP_STATE & 0x01 == 0x01)               /** check for Upper Calibration Limits **/
		|| ((SETUP_STATE & 0x02) ==0x02)) ) {			/** check for Lower Calibration Limits **/
		loadPreCalibration();								/** Load Default Calibration Settings **/
		currentScreen = CALIBRATION;							/** something is not set,
																		we will start with the calibration screen **/
	}
	else{
		loadCalibration();
		currentScreen = HOME;
	}
	
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
		
		getAnalogChannelValue(CH5);
      getAnalogChannelValue(CH6);
      getAnalogChannelValue(CH7);
      getAnalogChannelValue(CH8);

		processKeyInputs();
		processDisplay();
	}
};


uint8_t getKeyPressed(uint8_t port, uint8_t key){
	if(((~port) & (1 << key)) == (1 << key))	
	{
		_delay_ms(1);	/** get rid of keybounce **/
		return (((~port) & (1 << key)) == (1 << key));
	}	
	return 0x00;
}
void calibrateChannel(uint8_t ch)
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
void saveReverse(){
	eeprom_write_byte(&_reverse, reverse);
}
void checkNavigation(){
	if(getKeyPressed(PIN_TRIM, SIG_TRIM_AIL_PLUS)) { _delay_ms(250); currentScreen++; }
	if(getKeyPressed(PIN_TRIM, SIG_TRIM_AIL_MINUS)) { _delay_ms(250); currentScreen--; }
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
			if(getKeyPressed(PIN_MENU, SIG_MENU_KEY)) {_delay_ms(250); currentScreen = AIL; return;}	/** menu pressed for 250 ms**/
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_AIL_PLUS)) incTrim(AIL);
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_AIL_MINUS)) decTrim(AIL);
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_PLUS)) incTrim(ELE);
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_MINUS)) decTrim(ELE);
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_PLUS)) incTrim(THR);
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_MINUS)) decTrim(THR);
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_RUD_PLUS)) incTrim(RUD);
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_RUD_MINUS)) decTrim(RUD);
			break;
		case AIL...MAX_CHANNEL:
			if(getKeyPressed(PIN_MENU, SIG_MENU_KEY)) {_delay_ms(250); currentScreen = HOME; return;}	/** menu pressed for 250 ms**/
			checkNavigation();
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_PLUS)) { incTrim(currentScreen); }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_MINUS)) { decTrim(currentScreen); }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_PLUS)) { reverse |= (1<<(currentScreen - 1));	}
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_MINUS)) {	reverse &= (~(1 << (currentScreen - 1)));	}
			break;
		case SETTINGS1:
			checkNavigation();
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_PLUS)) { MIN_SIGNAL_WIDTH--; }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_MINUS)) { MIN_SIGNAL_WIDTH++; }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_PLUS)) { MAX_SIGNAL_WIDTH++; }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_MINUS)) { MAX_SIGNAL_WIDTH--; }
			break;
		case SETTINGS2:
			checkNavigation();
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_PLUS)) { INTER_CHANNEL_WIDTH--; }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_ELE_MINUS)) { INTER_CHANNEL_WIDTH++; }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_PLUS)) { FRAME_WIDTH++; }
			if(getKeyPressed(PIN_TRIM, SIG_TRIM_THR_MINUS)) { FRAME_WIDTH--; }
			
			break;
		case SAVE_SETTINGS:
			checkNavigation();
			if(getKeyPressed(PIN_MENU, SIG_MENU_KEY)) {_delay_ms(250); saveGlobalSettings(); saveReverse(); currentScreen = HOME; return;}   /** menu pressed for 250 ms**/
			break;
		case CALIBRATION:
			checkNavigation();
			if(getKeyPressed(PIN_MENU, SIG_MENU_KEY)) {_delay_ms(250); saveCalibration(); currentScreen = HOME; return;}   /** menu pressed for 250 ms**/
			calibrateChannel(AIL);
			calibrateChannel(ELE);
			calibrateChannel(THR);
			calibrateChannel(RUD);
			calibrateChannel(CH5);
			calibrateChannel(CH6);
			calibrateChannel(CH7);
			calibrateChannel(CH8);
			break;
	}
}
void showAnalogChannelSettings(){
	//lcd_puts(_strSeparator);lcd_puts(_strTrim);lcd_puts(_strSeparator);lcd_puts(_strSeparator);lcd_puts(_strReverse);lcd_puts(_strSeparator);lcd_puts(_strMs);
	sprintf(pszBuffer, percent[currentScreen] >= 0?_strPosVal:_strNegVal, percent[currentScreen]);//lcd_gotoxy(0, 1);lcd_puts(pszBuffer);
	sprintf(pszBuffer, _strNegVal, trims[currentScreen - 1]);//lcd_gotoxy(5, 1); lcd_puts(pszBuffer);
	uint8_t mask = (1 << (currentScreen - 1));
	//lcd_gotoxy(10, 1); lcd_puts((reverse & mask) == mask ? _strYes: _strNo);
	sprintf(pszBuffer, _strNegVal, ppm[currentScreen]);//lcd_gotoxy(12, 1); lcd_puts(pszBuffer);		
}
void showDigitalChannelSettings(){
	/**TODO: SHOW DIGITAL CHANNEL SETTING **/
	//lcd_puts(_strSeparator);lcd_puts(_strState);lcd_puts(_strSeparator);lcd_puts(_strSeparator);lcd_puts(_strReverse);lcd_puts(_strSeparator);lcd_puts(_strMs);
	sprintf(pszBuffer, percent[currentScreen] >= 0?_strPosVal:_strNegVal, percent[currentScreen]);//lcd_gotoxy(0, 1);lcd_puts(pszBuffer);
	//lcd_gotoxy(6, 1); lcd_puts(percent[currentScreen] >= 0? _strYes: _strNo);
	uint8_t mask = (1 << (currentScreen - 1));
	//lcd_gotoxy(10, 1); lcd_puts((reverse & mask)== mask ? _strYes: _strNo);
	sprintf(pszBuffer, _strNegVal, ppm[currentScreen]);//;lcd_gotoxy(12, 1); lcd_puts(pszBuffer);
}

uint8_t displayChannelBar(uint8_t ch, uint8_t x){
	volatile uint8_t barValue = ((percent[ch] + 100) * 0.15); //0.15 = 30/200
	ks0108DrawRect(x, 19, TRIM_TRACK_WIDTH, 30, BLACK);
	ks0108FillRect(x + TRIM_TRACK_WIDTH + 1, 19 + barValue, CHANNEL_TRACK_WIDTH, 30 - barValue, BLACK);
}

void processDisplay(){
	ks0108ClearScreen();
	switch(currentScreen){
		case HOME:
			/** Draw The basic UI **/
			ks0108GotoXY(0, 10);
//			ks0108Puts("  A        E        T        R        5        6        7        8");
			
         displayChannelBar(AIL, 1);
			displayChannelBar(ELE, 15);	
			displayChannelBar(THR, 29);	
			displayChannelBar(RUD, 43);	
			displayChannelBar(CH5, 57);	
			displayChannelBar(CH6, 71);	
			displayChannelBar(CH7, 85);	
			displayChannelBar(CH8, 99);									

			
//			ks0108FillRect(77, 3, 2, 57, BLACK);
//			ks0108FillRect(80, 3, 2, 57, BLACK);

//			ks0108DrawRect(54, 11, 10, 52, BLACK);
//			ks0108DrawRect(66, 10, 10, 52, BLACK);
//			ks0108DrawRect(78, 10, 10, 52, BLACK);
//			ks0108DrawRect(90, 10, 10, 52, BLACK);
//			ks0108DrawRect(102, 10, 10, 52, BLACK);
//			ks0108DrawHoriLine(16, 35, 50, BLACK);
//			ks0108DrawVertLine(41, 18, 8, BLACK);
			sprintf(pszBuffer, percent[AIL] >= 0 ?_strPosVal:_strNegVal, percent[AIL]); //lcd_gotoxy(2, 0); lcd_puts(pszBuffer);
			sprintf(pszBuffer, percent[ELE] >= 0 ?_strPosVal:_strNegVal, percent[ELE]); //lcd_gotoxy(9, 0); lcd_puts(pszBuffer);
			sprintf(pszBuffer, percent[THR] >= 0 ?_strPosVal:_strNegVal, percent[THR]); //lcd_gotoxy(2, 1); lcd_puts(pszBuffer);
			sprintf(pszBuffer, percent[RUD] >= 0 ?_strPosVal:_strNegVal, percent[RUD]); //lcd_gotoxy(9, 1); lcd_puts(pszBuffer);			
			//lcd_gotoxy(14, 1);
			//lcd_puts(percent[FLAPS] > 0? _strYes:_strNo);			
			_delay_ms(100);			
			return;
		case AIL:			
			ks0108Puts(_strAil);
			//lcd_puts(_strAil);showAnalogChannelSettings();
			break;
		case ELE:
			//lcd_puts(_strEle);showAnalogChannelSettings();			
			break;
		case THR:
			//lcd_puts(_strThr);showAnalogChannelSettings();			
			break;		
		case RUD:
			//lcd_puts(_strRud);showAnalogChannelSettings();			
			break;		
		case CH5:
			//lcd_puts(_strFlaps);showDigitalChannelSettings();						
			break;
		case CH6:
			break;
		case CH7:
			break;
		case CH8:
			break;
		case SETTINGS1:
			//lcd_puts(_strGlobalSettings1Title);
			//lcd_gotoxy(0, 1);
			//lcd_puts(_strMin);
			sprintf(pszBuffer, _strNegVal, MIN_SIGNAL_WIDTH + TRIM_CENTER);//lcd_puts(pszBuffer);
			//lcd_gotoxy(8, 1);
			//lcd_puts(_strMax);
			sprintf(pszBuffer, _strNegVal, MAX_SIGNAL_WIDTH + TRIM_CENTER);//lcd_puts(pszBuffer);
			break;
		case SETTINGS2:
			//lcd_puts(_strGlobalSettings2Title);
			//lcd_gotoxy(0, 1);		
			sprintf(pszBuffer, _strNegVal, INTER_CHANNEL_WIDTH);//lcd_puts(pszBuffer);
			//lcd_gotoxy(8, 1);
			sprintf(pszBuffer, _strNegVal, FRAME_WIDTH);//lcd_puts(pszBuffer);
			break;
		case SAVE_SETTINGS:
			//lcd_puts(_strSaveSettings);
			//lcd_gotoxy(0, 1);
			//lcd_puts(_strMenuToSave);
			return;
		case CALIBRATION:
			ks0108GotoXY(0, 0);
			ks0108Puts(_strCalibrationTitle);
			ks0108GotoXY(0, 20);
			ks0108Puts(_strMoveSticksPots);
			ks0108GotoXY(0, 30);
			ks0108Puts(_strFlipSwitches);
			ks0108GotoXY (0, 40);
			ks0108Puts(_strMenuToSave);
//			lcd_puts(_strMoveAetrMinMax);
//			lcd_gotoxy(0, 1);
//			lcd_puts(_strMenuToSave);
			return;
     	default:
			currentScreen = AIL;	/**Reset to home screen, if we don't know where we are **/
			return;
			}
	_delay_ms(100);			
}

void getAnalogChannelValue(uint8_t ch){
	/** read up the canonical channel value**/
	uint16_t value = readAnalog(ch);
	float calibratedChannelRange = calibrationUpper[ch] - calibrationLower[ch];
	float stickMoveRatio = (value - calibrationLower[ch])/calibratedChannelRange;
	
	/**adjust for reverse **/
	uint8_t mask = (1 << ch);
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
	TIMSK &= ~(1 << OCIE1A);
	if((PIN_PPM & (1 << SIG_PPM)) == (1 << SIG_PPM)){		//If the actual pin is high, we need to set OCR1A to the complementary delay
		if(++channel > (NUM_CHANNELS)){
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
	MIN_SIGNAL_WIDTH = eeprom_read_word(&_minSignalWidth);			/** read signal lengths **/
	MAX_SIGNAL_WIDTH = eeprom_read_word(&_maxSignalWidth);
	INTER_CHANNEL_WIDTH = eeprom_read_word(&_interChannelWidth);	
	FRAME_WIDTH = eeprom_read_word(&_frameWidth);
};


void loadParameters(){
	MID_SIGNAL_WIDTH = ((MAX_SIGNAL_WIDTH + MIN_SIGNAL_WIDTH)/2) + TRIM_CENTER;
	SYNC_SIGNAL_WIDTH = (FRAME_WIDTH - (NUM_CHANNELS * (MID_SIGNAL_WIDTH + INTER_CHANNEL_WIDTH))); /**whatever is left **/
	SIGNAL_TRAVERSAL = MAX_SIGNAL_WIDTH - MIN_SIGNAL_WIDTH;
};

void saveGlobalSettings(){
	eeprom_write_word(&_minSignalWidth, MIN_SIGNAL_WIDTH);
	eeprom_write_word(&_maxSignalWidth, MAX_SIGNAL_WIDTH);
	eeprom_write_word(&_interChannelWidth, INTER_CHANNEL_WIDTH);
	eeprom_write_word(&_frameWidth, FRAME_WIDTH);
};
void loadPreCalibration(){
	/** load the default settings **/
	calibrationUpper[AIL] = 0;
	calibrationUpper[ELE] = 0;
	calibrationUpper[THR] = 0;
	calibrationUpper[RUD] = 0;
	calibrationUpper[CH5] = 0;
	calibrationUpper[CH6] = 0;
	calibrationUpper[CH7] = 0;
	calibrationUpper[CH8] = 0;
	

	calibrationLower[AIL] = 1024;
	calibrationLower[ELE] = 1024;
	calibrationLower[THR] = 1024;
	calibrationLower[RUD] = 1024;
	calibrationLower[CH5] = 1024;
	calibrationLower[CH6] = 1024;
	calibrationLower[CH7] = 1024;
	calibrationLower[CH8] = 1024;
}

void loadCalibration(){
	eeprom_read_block((void*)&calibrationUpper, (const void*)_calibrationUpper, 2 * (NUM_CHANNELS));	
	eeprom_read_block((void*)&calibrationLower, (const void*)_calibrationLower, 2 * (NUM_CHANNELS));	
};

void saveCalibration(){
	eeprom_write_block((void*)&calibrationUpper, (const void*)_calibrationUpper, 2 * (NUM_CHANNELS));
	eeprom_write_block((void*)&calibrationLower, (const void*)_calibrationLower, 2 * (NUM_CHANNELS));
	SETUP_STATE |= (1<<0) | (1<<1);
	eeprom_write_byte(&_setupState, SETUP_STATE);		/**write back byte to the calibration**/
};

void setupHardware(){
	/** setup lcd **/
	ks0108Init(0);
	ks0108SelectFont(Arial_10, ks0108ReadFontData, BLACK);
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
 	
 	
 	/**PORTC is LCD_DATA, let the lcd library take care of it **/
 	
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

void reset(){
	/** set values to match servo spec **/
	ppm[SYNC] = SYNC_SIGNAL_WIDTH;
	ppm[AIL] = MID_SIGNAL_WIDTH;
	ppm[ELE] = MID_SIGNAL_WIDTH;
	ppm[THR] = MID_SIGNAL_WIDTH;
	ppm[RUD] = MID_SIGNAL_WIDTH;
	ppm[CH5] = MID_SIGNAL_WIDTH;
	ppm[CH6] = MID_SIGNAL_WIDTH;
	ppm[CH7] = MID_SIGNAL_WIDTH;
	ppm[CH8] = MID_SIGNAL_WIDTH;

	/** reset to the first channel **/
	channel = SYNC;
	OCR1A = micros_to_ticks(ppm[SYNC]);
};

















