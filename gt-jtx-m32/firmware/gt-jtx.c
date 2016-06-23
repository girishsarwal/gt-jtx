/*
 * gt-jtx.cpp
 *	
 *  Author: Girish Sarwal
 *	 Email: g.sarwal@gmail.com
 *       , girish.sarwal@gluedtomatoes.com
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

#define MAGIC_NUMBER 0xA0


#define INP1		0    //Mapped to PA7
#define INP2		1    //Mapped to PA6
#define INP3		2    //Mapped to PA5
#define INP4		3    //Mapped to PA4
#define INP5 		4    //Mapped to PA3
#define INP6		5    //Mapped to PA2
#define INP7		6    //Mapped to PA1
#define INP8		7    //Mapped to PA0

#define MAX_ANALOG_INPUTS 8

#define INP9		8     //Mapped to PD6
#define INP10		9     //Mapped to PD3
#define INP11		10    //Mapped to PD2
#define INP12		11    //Mapped to PB4
#define INP13		12    //Mapped to PB3
#define INP14		13    //Mapped to PB2
#define INP15		14    //Mapped to PB1
#define INP16		15     //Mapped to PB0

#define MAX_DIGITAL_INPUTS 8

#define NUM_PHYSICAL_INPUTS (MAX_ANALOG_INPUTS + MAX_DIGITAL_INPUTS)

#define VINP0		16
#define VINP1		17
#define VINP2		18
#define VINP3		19
#define VINP4		20
#define VINP5		21
#define VINP6		22
#define VINP7		23

#define MAX_VIRTUAL_INPUTS 8

#define CH1		0		//Mapped to A
#define CH2		1		//Mapped to E
#define CH3		2		//Mapped to T
#define CH4		3		//Mapped to R
#define CH5		4		//Mapped to 5
#define CH6		5		//Mapped to 6
#define CH7		6		//Mapped to 7
#define CH8		7		//Mapped to 8

#define NUM_INPUTS (MAX_ANALOG_INPUTS + MAX_DIGITAL_INPUTS + MAX_VIRTUAL_INPUTS)

#define NUM_OUTPUTS	8
#define SYNC NUM_OUTPUTS
#define NUM_OUTPUTS_INCLUDING_SYNC	(NUM_OUTPUTS + 1)

#define TRIM_UPPER_END 200
#define TRIM_LOWER_END 0
#define TRIM_CENTER 100

#define SIG_TRIM_CH1_PLUS 		0
#define SIG_TRIM_CH1_MINUS 	1
#define SIG_TRIM_CH2_PLUS 		2
#define SIG_TRIM_CH2_MINUS 	3
#define SIG_TRIM_CH3_PLUS 		4
#define SIG_TRIM_CH3_MINUS 	5
#define SIG_TRIM_CH4_PLUS 		6
#define SIG_TRIM_CH4_MINUS 	7

#define PORT_ANALOG	PORTA
#define DDR_ANALOG	DDRA
#define PIN_ANALOG	PINA

#define PORT_TRIM		PORTC
#define DDR_TRIM		DDRC
#define PIN_TRIM		PINC

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


#define HIBYTE(x) (x>>8)
#define LOBYTE(x) (x & 0xFF)
#define HIWORD(x) (x>>4)
#define LOWORD(x) (x & 0xF)


volatile uint16_t input_hw_controls[NUM_INPUTS];	/** stores the sensor values, pots or switches **/
volatile uint16_t *output_ppm = 0;		/** stores the final output ppm sent to the rf module **/

volatile uint8_t idx = 0;

uint8_t 	eeprom_okay;
uint8_t 	EEMEM _eeprom_eeprom_okay = 0xA0; //must bear A0 (10100000) for valid eeprom

void eeprom_mark_clean();
uint8_t eeprom_check_sanity();


void settings_new_default();
void settings_write_to_eeprom();
void settings_read_from_eeprom();
void calibration_write_to_eeprom();
void calibration_read_to_eeprom();

/*****************************************************save_eeprom_ok_status *****************************
** marks eeprom as OK
**/
void eeprom_mark_clean()
{
	eeprom_write_byte(&_eeprom_eeprom_okay, eeprom_okay);
};
/**************************************** check_eeprom_sanity *************************
** This method checks whether the eeprom is configured for use
** return false if there is a problem and eeprom needs formatting
**
**/
uint8_t eeprom_check_sanity(){
	eeprom_okay = eeprom_read_word(&_eeprom_eeprom_okay);								/** read eeprom status **/
	return (eeprom_okay == MAGIC_NUMBER);
}

/** radio settings **/
struct settings_t
{
	uint8_t setup_state;
	uint16_t min_signal_width_us;
	uint16_t max_signal_width_us;
	uint16_t inter_channel_width_us;
	uint16_t frame_width_us;
	uint8_t num_hw_input;		
	uint16_t upper_calibration[MAX_ANALOG_INPUTS];
	uint16_t lower_calibration[MAX_ANALOG_INPUTS];
};


struct settings_t g_settings = {0};
struct settings_t EEMEM _eeprom_g_settings = {252, 700, 1700, 300, 22500, 16,
																{0, 0, 0, 0, 0, 0, 0, 0},
																{1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
															};																																

void settings_new_default()
{
	g_settings.setup_state = 252;
	g_settings.min_signal_width_us = 700;
	g_settings.max_signal_width_us = 1700;
	g_settings.inter_channel_width_us = 300;
	g_settings.frame_width_us = 22500;
	g_settings.num_hw_input = 16;
	
	memset16(&g_settings.upper_calibration, 0, MAX_ANALOG_INPUTS);
	memset16(&g_settings.lower_calibration, 1024, MAX_ANALOG_INPUTS);

	settings_write_to_eeprom();
	calibration_write_to_eeprom();
};
void settings_write_to_eeprom()
{
	//eeprom_write_block((void*)&g_settings, (const void*) _eeprom_g_settings, sizeof(settings_t));
};
void settings_read_from_eeprom()
{
	eeprom_read_block((void*)&g_settings , (const void*)&_eeprom_g_settings, sizeof(struct settings_t));
};

void calibration_write_to_eeprom()
{

};

void calibration_read_to_eeprom()
{

};

/** temporary calculation variables **/
volatile uint16_t 	signal_traversal_us;
volatile uint16_t 	mid_signal_width_us;
volatile uint16_t 	sync_signal_width_us;

static uint8_t channel = 0;

/** models and mixes **/


struct mix_t
{
	uint8_t source;			/** source that transform the value, index into the input_hw_controls array **/
	uint8_t target;			/** target that will be changed, values will be first read and then modified based on mplex and rev
										 , always an index in the output_ppm array **/
	uint8_t wt_low;
	uint8_t wt_high;
	
	uint8_t expo_low;
	uint8_t expo_high;
	
	uint8_t rev_mplex;		/** 1b for rev 3b for mplex = add/mult/replace/invert **/
	uint8_t offset;			/** a final offset trim that will be applied **/
};

void mix_apply_transform(struct mix_t* mix)
{
	/** This is the meat of all mixes, every mix when applied will execute this function **/
};

#define MAX_LEN_NAME 16
#define MAX_MIXES 32




struct model
{
	struct mix_t mixes[MAX_MIXES];		/** array of mixes **/
	uint8_t trims[MAX_ANALOG_INPUTS];	/** array of trims **/
	
} model;		//This is the only representation of the model int the system. the list of models will be stored with the frontend.
				//It is the frontend's responsibility to create a to-fro mechanism for sending model data to gt-jtx over spi


void model_new_default()
{
	/** define a few mixes **/
	memset(&model.trims, 100, MAX_ANALOG_INPUTS);
};

void model_load_from_eeprom()
{
	
};
void model_save_to_eeprom()
{
	
};
void model_save_trim(uint8_t channel)
{

};

/** spi communication **/
enum OPCODE
{
	NOP = 0x00,        /** a NOP request will send back the status code to master, if no error, then NOP will be sent back **/

   SETTUP = 0x01,     /** set up trim **/
   SETTDN = 0x02,     /** set down trim **/
   GETT = 0x03,       /** get trim **/
   SETREV = 0x04,     /** set signal reverse **/
   GETREV = 0x05,
   SCUP = 0x06,
   GCUP = 0x07,
   SCDN = 0x08,
   GCDN = 0x09,
   SPPMLEN = 0x0A,
   GPPMLEN = 0x0B,
   SPPMICL = 0x0C,
   GPPMICL = 0x0D,
   SSTIM = 0x0E,
   GSTIM = 0x0F,
   GCV = 0x10,

	/** everything above 0xE* is control or error state, 32 signals can be sent back **/

   BAD_EEPROM = 0xFE,
	CALIBRATION_REQUIRED = 0xFD,
   RESET = 0xFF,
};

enum STATE
{
   WAIT_RESET  = 0x00,
	WAIT_OPCODE = 0x01,
   WAIT_HIBYTE = 0x02,
   WAIT_LOBYTE = 0x03,
};

struct spitransaction_t
{
	uint8_t opcode;
	uint8_t state;
	uint8_t data0;
	uint8_t data1;
	uint16_t result;
};


/** SPI State Management and operation variables **/
volatile struct spitransaction_t transaction;


/***************************************** Function Declarations *****************************************/
uint16_t read_analog(uint8_t);
uint16_t micros_to_ticks(uint16_t);
uint8_t get_key_pressed(uint8_t port, uint8_t key);
void process_key_inputs();

void reset();
void setup_hardware();
void read_ad_channel_value(uint8_t);
void read_switch(uint8_t ch, uint8_t port, uint8_t pin);
void calculate_signal_params();

void increment_trim(uint8_t channel);
void decrement_trim(uint8_t channel);

void calibrate_channel(uint8_t channel);

void memset16(uint16_t* array, uint16_t value, uint8_t size);


/***************************************** Interrupt Declarations *****************************************/
ISR(SPI_STC_vect);     	/**SPI byte received **/
ISR(TIMER1_COMPA_vect); /**PPM time elapsed **/


/************************************MAIN Routine *****************************************
*/

int main(){
	setup_hardware();
   /** check EEPROM Sanity **/
	if(!eeprom_check_sanity()){
		/** create default settings **/	
		settings_new_default();
		eeprom_mark_clean();
		/**give three beeps specifying defautl settings have been loaded **/
	}
	/** read back settings from eeprom **/
	settings_read_from_eeprom();

	while(1){
		if(!(g_settings.setup_state & 0x01)
		&& !(g_settings.setup_state & 0x02)){             	/** check for Upper Calibration Limits **/			/** check for Lower Calibration Limits **/
			transaction.result = NOP;
			break;
		}
		/** instruct the SPI to be sending back CALIBRATION_REQUIRED state **/
		/** do nothing**/
      transaction.result = CALIBRATION_REQUIRED;
	}
	
	calculate_signal_params();	
	reset();
	/**Enable global interrupts **/
	sei();
   		/** start our timer **/
	TIMSK |= (1<<OCIE1A);
			/** start the adc **/
	ADCSRA |= (1<<ADSC);
	while(1){
		volatile uint8_t index = -1;
		
		/** read analog inputs **/
		read_ad_channel_value(INP1);
		read_ad_channel_value(INP2);
		read_ad_channel_value(INP3);
		read_ad_channel_value(INP4);
		read_ad_channel_value(INP5);
		read_ad_channel_value(INP6);
		read_ad_channel_value(INP7);
		read_ad_channel_value(INP8);

      /** read digital inputs **/
		read_switch(INP9, PORTD, 4);
		read_switch(INP10, PORTD, 6);
		read_switch(INP11, PORTD, 7);
		read_switch(INP12, PORTB, 0);
		read_switch(INP13, PORTB, 1);
		read_switch(INP14, PORTB, 2);
		read_switch(INP15, PORTB, 3);
		read_switch(INP16, PORTB, 4);

		/** apply the mixes **/
		uint8_t idx_mixes = -1;
		while(++idx_mixes < MAX_MIXES){
				mix_apply_transform(&model.mixes[idx_mixes]);
		};
	}
};

/**************************************** get_key_pressed *****************************
** This method checks to see if a key has been pressed on a port.
** debounce is included
**/
uint8_t get_key_pressed(uint8_t port, uint8_t key){
	if(((~port) & (1 << key)) == (1 << key))	
	{
		_delay_ms(1);	/** get rid of keybounce **/
		return (((~port) & (1 << key)) == (1 << key));
	}	
	return 0x00;
};


/**************************************** calibrate_channel *****************************
** This method keeps on updating the values if the calibration settings of a channel
** call the method in a loop
**/
void calibrate_channel(uint8_t ch){
	uint16_t calibration = read_analog(ch);		
	if(g_settings.upper_calibration[ch] < calibration)
		g_settings.upper_calibration[ch] = calibration;
	if(g_settings.lower_calibration[ch] > calibration)
		g_settings.lower_calibration[ch] = calibration;
}

/**************************************** increment_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void increment_trim(uint8_t ch){
	if(++(model.trims[ch]) > TRIM_UPPER_END){
		(model.trims[ch]) = TRIM_UPPER_END;
	}
	model_save_trim(ch);
}

/**************************************** decrement_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void decrement_trim(uint8_t ch){
	if(model.trims[ch] > 0 && --model.trims[ch]<= TRIM_LOWER_END){
		model.trims[ch] = TRIM_LOWER_END;
	}
	model_save_trim( ch);
}



/**************************************** read_ad_channel_value *****************************
** This method reads up the analog value of a channel to the ppm array
** the first 8 channels can have either analog or digital values.
** the next 8 channels can only have digital values.
** analog or digital depends on whether one puts a POT ot a SPDT switch on that channel
**/
void read_ad_channel_value(uint8_t ch){
	uint16_t value = read_analog(ch);
	
	float calibratedChannelRange = g_settings.upper_calibration[ch] - g_settings.lower_calibration[ch];
	float stickMoveRatio = (value - g_settings.lower_calibration[ch])/calibratedChannelRange;
	
	uint8_t mask = (1 << ch);
	input_hw_controls[ch] = g_settings.min_signal_width_us + ((stickMoveRatio * (float)signal_traversal_us));
}

/**************************************** read_switch *****************************
** This method reads up the digital value of a channel (8-15) to the switches variable
** on these inputs, only a SPDT will work
**/
void read_switch(uint8_t ch, uint8_t port, uint8_t pin){
	
	volatile uint8_t mask = (1 << ch);
	volatile uint8_t value = get_key_pressed(port, pin);
	//volatile uint8_t value = (get_key_pressed(port, pin) ^ ((reverse & mask) == mask)) ? 1 : 0;

	output_ppm[ch] = g_settings.min_signal_width_us + (value * (float)signal_traversal_us) + model.trims[ch];
}

/**************************************** micros_to_ticks *****************************
** Converts the values from microseconds to ticks based on the processor clock
**/
uint16_t micros_to_ticks(uint16_t value){
	return (value * ((F_CPU/1000)/1000));
};

/**************************************** calculate_signal_params *****************************
** Calculates the signal limits and other values used in deciding the final ppm signal
**/
void calculate_signal_params(){
	mid_signal_width_us = ((g_settings.max_signal_width_us + g_settings.max_signal_width_us)/2) + TRIM_CENTER;
	sync_signal_width_us = (g_settings.frame_width_us - (NUM_OUTPUTS * (mid_signal_width_us + g_settings.inter_channel_width_us))); /**whatever is left **/
	signal_traversal_us = g_settings.max_signal_width_us - g_settings.min_signal_width_us;
};


/***************************************** METHOD setupHardware ****************************************
	* Sets up the hardware, sets up the ports, ppm counters, adc and spi
*/

void setup_hardware(){

	/** setup the SPI Slave **/
	/** Port B has the MISO/MOSI pins. Setup MOSI as input **/
	transaction.state = WAIT_OPCODE;
	DDRB = (1<<PB6);	/**Setup MISO as output */
	SPCR = (1<<SPE) | (1<<SPIE) | (1<<SPR0) | (1<<SPR1)| (1<<CPOL);	/** enable SPI **/
	SPDR = 0xFF;
	sei();

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
	ADCSRA|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	/**F_CPU/64 Prescalar **/
	ADCSRA |= (1<<ADEN); 									/** enable the ADC **/

};


uint16_t read_analog(uint8_t ch){
	ch = ch & 0x07;					/** zero out all bits except last three for limiting channel **/
	ADMUX &=0xE0;								/**reset channel selection **/
	ADMUX |=ch;									/**select the requested channel **/
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|= (1<<ADIF);
	return (ADC);
};

/***************************************** METHOD reset ****************************************
* resets all servos to go to center of signal
**/

void reset(){
	/** create input objects **/
	uint8_t idx = -1;
	
	idx = -1;

	/** see how many outputs **/
	output_ppm = malloc(NUM_OUTPUTS_INCLUDING_SYNC * sizeof(uint16_t));
	/** set values to match servo spec **/
	memset16(output_ppm, mid_signal_width_us, NUM_OUTPUTS_INCLUDING_SYNC);
	/**reset the servo signals **/
	output_ppm[SYNC] = sync_signal_width_us;
	/** reset to the first channel **/
	channel = CH1;
	OCR1A = micros_to_ticks(output_ppm[SYNC]);
};

/*****************************************  process_key_inputs ****************************************
	* processes Key inputs on the gt-jtx. These are trim keys only for the gt-jtx-m32
	* this needs to be called in a loop for continous polling. Call in the main loop
**/
void process_key_inputs(){
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH1_PLUS)) increment_trim(CH1);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH1_MINUS)) decrement_trim(CH1);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH2_PLUS)) increment_trim(CH2);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH2_MINUS)) decrement_trim(CH2);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH3_PLUS)) increment_trim(CH3);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH3_MINUS)) decrement_trim(CH3);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH4_PLUS)) increment_trim(CH4);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH4_MINUS)) decrement_trim(CH4);
};

/*****************************************  spi_process_get_message ****************************************
	* processess an SPI message that is required to read something. A byte is called a message
	* this will be executed when the HIBYTE (data0) is received
	* since we need atleast two more bytes to return the result
**/
void spi_process_get_message(){
 	switch (transaction.opcode)
	{
		case NOP:
			break;
      case GCV:
        	transaction.result = (output_ppm[(transaction.data0 >> 4)]);
        	break;
   	case GETT:
   		transaction.result = (model.trims[(transaction.data0 >> 4)]);
   		break;
	   case GETREV:
			//transaction.result = reverse >> (transaction.data0 >> 4);
	   	break;
		case GCUP:
			transaction.result = (g_settings.lower_calibration[(transaction.data0 >> 4) - 1]);
			break;
		case GCDN:
			transaction.result = (g_settings.lower_calibration[(transaction.data0 >> 4) - 1]);
			break;
		case GPPMLEN:
			transaction.result = g_settings.frame_width_us;
			break;
		case GPPMICL:
			transaction.result = g_settings.inter_channel_width_us;
			break;
		case GSTIM:
			transaction.result = g_settings.max_signal_width_us;
			break;
	  	case RESET:
  	     	transaction.state = WAIT_OPCODE;
  	     	SPDR = 0x00;		
     	   break;
   }
};

/*****************************************  spi_process_set_message ****************************************
	* processess an SPI message that is required to write something. A byte is called a message
	* this will be executed when the RESULT (result) is received on SPI
**/
void spi_process_set_message(){
 	switch (transaction.opcode)
	{
		case NOP:
			break;
		case SETTUP:
   		break;
   	case SETTDN:
   		break;
	   case SETREV:
	   	break;
		case SCUP:
			break;
		case SCDN:
			break;
		case SPPMLEN:
			g_settings.frame_width_us = (transaction.data0 << 8) | transaction.data1;
			settings_write_to_eeprom();
			calculate_signal_params();
			reset();
			break;
		case SPPMICL:
			break;
		case SSTIM:
			break;
     	case RESET:
  	     	transaction.state = WAIT_OPCODE;
  	     	SPDR = 0x00;		
     	   break;
   }
};

/***************************************** PPM Interrupt ********************************1*********/
ISR(TIMER1_COMPA_vect){
	TIMSK &= ~(1 << OCIE1A);
	if((PIN_PPM & (1 << SIG_PPM)) == (1 << SIG_PPM)){		//If the actual pin is high, we need to set OCR1A to the complementary delay
		OCR1A = micros_to_ticks(output_ppm[channel++]);
		if(channel > SYNC){
			channel = CH1;
		}
	}		
	else{
		OCR1A = micros_to_ticks(g_settings.inter_channel_width_us);
   }
	TIMSK |= (1<<OCIE1A);
};


/***************************************** SPI Interrupt *****************************************/
ISR(SPI_STC_vect){
	uint8_t data = SPDR;
	if(RESET == data){
	 	SPDR = 0x00;
    	transaction.state = WAIT_OPCODE;
   	return;
	}
	else{
		switch (transaction.state){
   		case WAIT_OPCODE:
				transaction.opcode = data;
				transaction.state = WAIT_HIBYTE;
	         SPDR = data;     				/**return the opcode as next byte return so master knows what data is being returned**/
	         return ;
	      case WAIT_HIBYTE:
	      	transaction.data0 = data;	/** data0 MUST have the identifying fields for the opcode **/
	      	transaction.state = WAIT_LOBYTE;
	      	spi_process_get_message();
				SPDR = HIBYTE(transaction.result);
				return;
   	  	case WAIT_LOBYTE:
	      	transaction.data1 = data;
	      	transaction.state = WAIT_RESET;
      	   SPDR = LOBYTE(transaction.result);
				return;
   	   case WAIT_RESET:
      	  	SPDR = 0x00;
      	  	spi_process_set_message();
      		transaction.state = WAIT_OPCODE;
	      	return;
		}
	}
};

/*********************************************** memset16 ***************************************
** used to set values in a 16 bit contigous location
**/
void memset16(uint16_t* a, uint16_t value, uint8_t size){
	volatile uint8_t index = -1;
	while(++index < size){
		a[index] = value;
	};
};

























