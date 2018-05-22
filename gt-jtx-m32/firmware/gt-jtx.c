/*
 * gt-jtx.cpp
 *	
 *  Author: Girish Sarwal
 *	 Email: g.sarwal@gmail.com
 *       , girish.sarwal@gluedtomatoes.com
 *
 */

#include "includes/common.h"
typedef struct {
	uint16_t hw_controls[NUM_PHYSICAL_INPUTS]; /** stores the sensor values, pots or switches **/
	uint16_t ppm[NUM_OUTPUTS_INCLUDING_SYNC];		/** stores the final output ppm sent to the rf module **/

	uint16_t 	signal_traversal_us;
	uint16_t 	mid_signal_width_us;
	uint16_t 	sync_signal_width_us;
	uint8_t 	error;			/** startup errors **/
	uint8_t 	channel;
	uint8_t 	idx;
   uint16_t test;

	SPITRANSACTION transaction;

	/** from here on everything will be serialized to eeprom **/
	SETTINGS settings;			/** serialized to eeprom **/
	MODEL model;				/** serialized to eeprom **/
} RUNTIME, *PRUNTIME;


volatile RUNTIME runtime;


/***************************************** Function Declarations *****************************************/
uint16_t read_analog(uint8_t);
uint16_t micros_to_ticks(uint16_t);
uint8_t get_key_pressed(uint8_t port, uint8_t key);
void process_key_inputs(void);

void reset(void);
void setup_hardware(void);
void read_ad_channel_value(uint8_t);
void read_switch(uint8_t ch, uint8_t port, uint8_t pin);
void calculate_signal_params(void);

void increment_trim(uint8_t channel);
void decrement_trim(uint8_t channel);

void calibrate_channel(uint8_t channel);

void memset16(uint16_t* array, uint16_t value, uint8_t size);
void model_save_trim(uint8_t channel);



/***************************************** Business Objects *****************************************/

void runtime_new (uint8_t debug);

void model_write_to_eeprom(PMODEL);
void model_read_from_eeprom(PMODEL);

void settings_write_to_eeprom(PSETTINGS);
void settings_read_from_eeprom(PSETTINGS);

void calibration_write_to_eeprom(void);
void calibration_read_to_eeprom(void);


/***************************************** Interrupt Declarations *****************************************/
ISR(SPI_STC_vect);     	/**SPI byte received **/
ISR(TIMER1_COMPA_vect); /**PPM time elapsed **/


/************************************MAIN Routine *****************************************
*/

int main(void){
	
	runtime_new(1);                        	
	setup_hardware();
	calculate_signal_params();	
	reset();
	/**Enable global interrupts **/
	sei();
   		/** start our timer **/
	TIMSK |= (1<<OCIE1A);
			/** start the adc **/
	ADCSRA |= (1<<ADSC);

	while(1){
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
		read_switch(INP9, PORTD, PIN4);
		read_switch(INP10, PORTD, PIN6);
		read_switch(INP11, PORTD, PIN7);
		read_switch(INP12, PORTB, PIN0);
		read_switch(INP13, PORTB, PIN1);
		read_switch(INP14, PORTB, PIN2);
		read_switch(INP15, PORTB, PIN3);
		read_switch(INP16, PORTB, PIN4);

		/** apply the mixes **/
		uint8_t idx_mixes = -1;
		while(++idx_mixes < MAX_MIXES){
				mix_apply_transform(&(runtime.model.mixes[idx_mixes]));
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
	if(runtime.settings.upper_calibration[ch] < calibration)
		runtime.settings.upper_calibration[ch] = calibration;
	if(runtime.settings.lower_calibration[ch] > calibration)
		runtime.settings.lower_calibration[ch] = calibration;
}

/**************************************** increment_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void increment_trim(uint8_t ch){
	if(++(runtime.model.trims[ch]) > TRIM_UPPER_END){
		(runtime.model.trims[ch]) = TRIM_UPPER_END;
	}
	model_save_trim(ch);
}

/**************************************** decrement_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void decrement_trim(uint8_t ch){
	if(runtime.model.trims[ch] > 0 && --runtime.model.trims[ch]<= TRIM_LOWER_END){
		runtime.model.trims[ch] = TRIM_LOWER_END;
	}
	model_save_trim(ch);
}


void model_save_trim(uint8_t channel) {

}


/**************************************** read_ad_channel_value *****************************
** This method reads up the analog value of a channel to the ppm array
** the first 8 channels can have either analog or digital values.
** the next 8 channels can only have digital values.
** analog or digital depends on whether one puts a POT ot a SPDT switch on that channel
**/
void read_ad_channel_value(uint8_t ch){
	uint16_t value = read_analog(ch);
	
	float calibratedChannelRange = runtime.settings.upper_calibration[ch] - runtime.settings.lower_calibration[ch];
	float stickMoveRatio = (value - runtime.settings.lower_calibration[ch])/calibratedChannelRange;
	
	runtime.input.hw_controls[ch] = runtime.settings.min_signal_width_us + ((stickMoveRatio * (float)runtime.signal_traversal_us)) + runtime.model.trims[ch];
}

/**************************************** read_switch *****************************
** This method reads up the digital value of a channel (8-15) to the switches variable
** on these inputs, only a SPDT will work
**/
void read_switch(uint8_t ch, uint8_t port, uint8_t pin){
	
	volatile uint8_t value = get_key_pressed(port, pin);
	//volatile uint8_t value = (get_key_pressed(port, pin) ^ ((reverse & mask) == mask));  /** reverse is not a mixes feature. program it there **/

	runtime.input.hw_controls[ch] = runtime.settings.min_signal_width_us + (value * (float)runtime.signal_traversal_us);
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

	runtime.mid_signal_width_us = ((runtime.settings.min_signal_width_us + runtime.settings.max_signal_width_us)/2) + TRIM_CENTER;
	runtime.sync_signal_width_us = (runtime.settings.frame_width_us - (NUM_OUTPUTS * (runtime.mid_signal_width_us + runtime.settings.inter_channel_width_us))); /**whatever is left **/
	runtime.signal_traversal_us = runtime.settings.max_signal_width_us - runtime.settings.min_signal_width_us;
};


/***************************************** METHOD setupHardware ****************************************
	* Sets up the hardware, sets up the ports, ppm counters, adc and spi
*/

void setup_hardware(){

	/** setup the SPI Slave **/
	/** Port B has the MISO/MOSI pins. Setup MOSI as input **/
	runtime.transaction.state = WAIT_OPCODE;
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
 	
 	/** PORTD has D:0-D:4 for LCD_CTRL, make D5 (PPM), D6 (SPEAKER), D7 (MENU) as output pins **/
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

	/** set values to match servo spec **/
	memset16(runtime.output.ppm, runtime.mid_signal_width_us, NUM_OUTPUTS_INCLUDING_SYNC);
	/**reset the servo signals **/
	runtime.output.ppm[SYNC] = runtime.sync_signal_width_us;
	/** reset to the first channel **/
	runtime.channel = CH1;
	//OCR1A = micros_to_ticks(runtime.output.ppm[SYNC]);
};

/*****************************************  process_key_inputs ****************************************
	* processes Key inputs on the gt-jtx. These are trim keys only for the gt-jtx-m32
	* most of controls elsewise will be via SPI
	* this needs to be called in a loop for continous polling. Call in the main loop
**/

void process_key_inputs(void){
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH1_PLUS)) increment_trim(CH1);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH1_MINUS)) decrement_trim(CH1);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH2_PLUS)) increment_trim(CH2);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH2_MINUS)) decrement_trim(CH2);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH3_PLUS)) increment_trim(CH3);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH3_MINUS)) decrement_trim(CH3);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH4_PLUS)) increment_trim(CH4);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH4_MINUS)) decrement_trim(CH4);
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



/*****************************************  spi_process_get_message ****************************************
	* processess an SPI message that is required to read something. A byte is called a message
	* this will be executed when the HIBYTE (data0) is received
	* since we need atleast two more bytes to return the result
**/
void spi_process_get_message(void){
 	switch (runtime.transaction.opcode)
	{
		case NOP:
			break;
      case GCV:
        	runtime.transaction.result = (runtime.output.ppm[(runtime.transaction.data0 >> 4)]);
        	break;
   	case GETT:
   			runtime.transaction.result = (runtime.model.trims[(runtime.transaction.data0 >> 4)]);
   			break;
	   case GETREV:
			//runtime.transaction.result = reverse >> (runtime.transaction.data0 >> 4);
	   		break;
		case GCUP:
			runtime.transaction.result = (runtime.settings.lower_calibration[(runtime.transaction.data0 >> 4) - 1]);
			break;
		case GCDN:
			runtime.transaction.result = (runtime.settings.lower_calibration[(runtime.transaction.data0 >> 4) - 1]);
			break;
		case GPPMLEN:
			runtime.transaction.result = runtime.settings.frame_width_us;
			break;
		case GPPMICL:
			runtime.transaction.result = runtime.settings.inter_channel_width_us;
			break;
		case GSTIM:
			runtime.transaction.result = runtime.settings.max_signal_width_us;
			break;
	  	case RESET:
  	     	runtime.transaction.state = WAIT_OPCODE;
  	     	SPDR = 0x00;		
     	   break;
   }
};

/*****************************************  spi_process_set_message ****************************************
	* processess an SPI message that is required to write something. A byte is called a message
	* this will be executed when the RESULT (result) is received on SPI
**/
void spi_process_set_message(void){
 	switch (runtime.transaction.opcode)
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
			runtime.settings.frame_width_us = (runtime.transaction.data0 << 8) | runtime.transaction.data1;
			calculate_signal_params();
			break;
		case SPPMICL:
			break;
		case SSTIM:
			break;
     	case RESET:
  	     	runtime.transaction.state = WAIT_OPCODE;
  	     	SPDR = 0x00;		
     	   break;
   }
};

void runtime_new (uint8_t debug) {
	/** any error in this function would mean reporting back to client and shutting down the micro
	**/
	memset(&runtime, 0, sizeof(RUNTIME));
	if(debug) {
		/** load debug values for testing **/
		runtime.settings.setup_state = 252;
		runtime.settings.min_signal_width_us = 700;
		runtime.settings.max_signal_width_us = 1700;
		runtime.settings.inter_channel_width_us = 300;
		runtime.settings.frame_width_us = 22500;
		runtime.settings.num_hw_input = 16;		
		memset16(&runtime.settings.upper_calibration, 0, MAX_ANALOG_INPUTS);
		memset16(&runtime.settings.lower_calibration, 1024, MAX_ANALOG_INPUTS);
		/** create a model for testing **/
		eeprom_write_block((uint8_t*)0, &runtime.settings, sizeof(SETTINGS));

	}
	/** load data from eeprom **/
//	eeprom_read_block((uint8_t*)0, &runtime.settings, sizeof(SETTINGS));
}



void mix_apply_transform(PMIX mix)
{
	/** This is the meat of all mixes, every mix when applied will execute this function **/
};


/***************************************** PPM Interrupt ********************************1*********/
ISR(TIMER1_COMPA_vect){
	TIMSK &= ~(1 << OCIE1A);
	if((PIN_PPM & (1 << SIG_PPM)) == (1 << SIG_PPM)){		//If the actual pin is high, we need to set OCR1A to the complementary delay
		OCR1A = micros_to_ticks(runtime.output.ppm[runtime.channel++]);
		if(runtime.channel > SYNC){
			runtime.channel = CH1;
		}
	}		
	else{
		OCR1A = micros_to_ticks(runtime.settings.inter_channel_width_us);
		//OCR1A = micros_to_ticks(runtime.output.ppm[SYNC]);
   }
	TIMSK |= (1<<OCIE1A);
};


/***************************************** SPI Interrupt *****************************************/
ISR(SPI_STC_vect){
	uint8_t data = SPDR;
	if(RESET == data){
	 	SPDR = 0x00;
    	runtime.transaction.state = WAIT_OPCODE;
   	return;
	}
	else{
		switch (runtime.transaction.state){
   		case WAIT_OPCODE:
				runtime.transaction.opcode = data;
				runtime.transaction.state = WAIT_HIBYTE;
	         SPDR = data;     				/**return the opcode as next byte return so master knows what data is being returned**/
	         return ;
	      case WAIT_HIBYTE:
	      	runtime.transaction.data0 = data;	/** data0 MUST have the identifying fields for the opcode **/
	      	runtime.transaction.state = WAIT_LOBYTE;
	      	spi_process_get_message();
				SPDR = HIBYTE(runtime.transaction.result);
				return;
   	  	case WAIT_LOBYTE:
	      	runtime.transaction.data1 = data;
	      	runtime.transaction.state = WAIT_RESET;
      	   SPDR = LOBYTE(runtime.transaction.result);
				return;
   	   case WAIT_RESET:
      	  	SPDR = 0x00;
      	  	spi_process_set_message();
      		runtime.transaction.state = WAIT_OPCODE;
	      	return;
		}
	}
};






