/*
 * runtime.c
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */
#include "../includes/runtime.h"

void runtime_new(RUNTIME* runtime){
	runtime = (RUNTIME*) malloc (sizeof(RUNTIME));
	/** try to read from the eeprom first **/
	memset(&runtime, 0, sizeof(RUNTIME));
	/** load debug values for testing **/
	runtime->settings.setup_state = 252;
	runtime->settings.min_signal_width_us = 700;
	runtime->settings.max_signal_width_us = 1700;
	runtime->settings.inter_channel_width_us = 300;
	runtime->settings.frame_width_us = 22500;
	runtime->settings.num_hw_input = 16;
	memset16(&runtime->settings.upper_calibration, 0, NUM_ANALOG_INPUTS);
	memset16(&runtime->settings.lower_calibration, 1024, NUM_ANALOG_INPUTS);
	/** create a model for testing **/
	eeprom_write_block((uint8_t*)0, &runtime->settings, sizeof(SETTINGS));
};

void runtime_reset(RUNTIME* runtime) {
	/** create input objects **/
	runtime->idx = -1;
	/** set values to match servo spec **/
	memset16(runtime->ppm, runtime->mid_signal_width_us, NUM_OUTPUTS_INCLUDING_SYNC);
	/**reset the servo signals **/
	runtime->ppm[SYNC] = runtime->sync_signal_width_us;
	/** reset to the first channel **/
	runtime->channel = CH1;
	/** usart status	 */
	runtime->usart_state = WAIT_HEADER;
	/** setup the queues **/
};

void runtime_setup_hardware(RUNTIME* runtime) {
	/**
	 * Analog Inputs
	 */
	DDR_ANALOG = 0x00;
	PORT_ANALOG = 0x00;
	/**
	 * Digital Inputs
	 */
	DDR_DIGITAL = 0x00;
	PORT_DIGITAL = 0x00;
	/**
	 *  Trim switches
	 */
	DDR_TRIM = 0x00;
	PORT_TRIM = 0xFF;

	/** setup the usart
	 *
	 */
	UBRRL = UBRRVAL;
	UBRRH = (UBRRVAL>>8);
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
			(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);

	/** Setup PPM generator
	 *
	 */
	TCNT1 = 0;
	TCCR1B |= (1<<CS10) ;									/** F_CPU - 1Mhz Timebase **/
	TCCR1B	|=	(1<<WGM12);									/** CTC Mode**/
	TCCR1A |= (1<<COM1A0);									/** hardware ctc **/

	/** Enable the ADC to read analog channels
	 *
	 */

	ADMUX |= (1<<REFS0);
	ADCSRA|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);			/**F_CPU/64 Prescalar **/
	ADCSRA |= (1<<ADEN); 									/** enable the ADC **/
}

/**************************************** calculate_signal_params *****************************
** Calculates the signal limits and other values used in deciding the final ppm signal
**/
void runtime_calculate_signal_parameters(RUNTIME* runtime) {
	runtime->mid_signal_width_us = ((runtime->settings.min_signal_width_us + runtime->settings.max_signal_width_us)/2) + TRIM_CENTER;
	runtime->sync_signal_width_us = (runtime->settings.frame_width_us - (NUM_OUTPUTS * (runtime->mid_signal_width_us + runtime->settings.inter_channel_width_us))); /**whatever is left **/
	runtime->signal_traversal_us = runtime->settings.max_signal_width_us - runtime->settings.min_signal_width_us;
}

/**************************************** read_ad_channel_value *****************************
** This method reads up the analog value of a channel to the ppm array
** the first 8 channels can have either analog or digital values.
** the next 8 channels can only have digital values.
** analog or digital depends on whether one puts a POT ot a SPDT switch on that channel
**/
void runtime_read_analog_input(RUNTIME* runtime, uint8_t ch) {
	uint16_t value = read_analog(ch);

	float calibratedChannelRange = runtime->settings.upper_calibration[ch] - runtime->settings.lower_calibration[ch];
	float stickMoveRatio = (value - runtime->settings.lower_calibration[ch])/calibratedChannelRange;

	runtime->physical_inputs[ch] = runtime->settings.min_signal_width_us + ((stickMoveRatio * (float)runtime->signal_traversal_us)) + runtime->model.trims[ch];
}


/**************************************** read_switch *****************************
** This method reads up the digital value of a digital input (D0-D7) into the
** correct hardware channels. These inputs can only accept a digital switch
**
** uint8_t ch - the channel number in the inputs array
** uint8_t - the signal which will be read
**/
void runtime_read_digital_input(RUNTIME* runtime, uint8_t ch, uint8_t pin) {
	volatile uint8_t value = get_key_pressed(PORT_DIGITAL, pin);
	//volatile uint8_t value = (get_key_pressed(port, pin) ^ ((reverse & mask) == mask));  /** reverse is not a mixes feature. program it there **/
	runtime->physical_inputs[ch] = runtime->settings.min_signal_width_us + (value * (float)runtime->signal_traversal_us);
}

/**************************************** increment_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void runtime_increment_trim(RUNTIME* runtime, uint8_t ch){
	if(++(runtime->model.trims[ch]) > TRIM_UPPER_END){
		(runtime->model.trims[ch]) = TRIM_UPPER_END;
	}
	model_save_trim(&runtime->model, ch);
}
/**************************************** decrement_trim *****************************
** This method increments the trim values of a channel and clamps them between the min and max
** trim values
**/
void runtime_decrement_trim(RUNTIME* runtime, uint8_t ch){
	if(runtime->model.trims[ch] > 0 && --runtime->model.trims[ch]<= TRIM_LOWER_END){
		runtime->model.trims[ch] = TRIM_LOWER_END;
	}
	model_save_trim(&runtime->model, ch);
}


/**************************************** calibrate_channel *****************************
** This method keeps on updating the values if the calibration settings of a channel
** call the method in a loop
**/
void runtime_calibrate_channel(RUNTIME* runtime, uint8_t ch) {
	uint16_t calibration = read_analog(ch);
	if(runtime->settings.upper_calibration[ch] < calibration)
		runtime->settings.upper_calibration[ch] = calibration;
	if(runtime->settings.lower_calibration[ch] > calibration)
		runtime->settings.lower_calibration[ch] = calibration;
}

/*****************************************  process_key_inputs ****************************************
	* processes Key inputs on the gt-jtx. These are trim keys only for the gt-jtx-m32
	* most of controls elsewise will be via SPI
	* this needs to be called in a loop for continous polling. Call in the main loop
**/

void runtime_process_trim_inputs(RUNTIME* runtime) {
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH1_PLUS)) runtime_increment_trim(runtime, CH1);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH1_MINUS)) runtime_decrement_trim(runtime, CH1);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH2_PLUS)) runtime_increment_trim(runtime, CH2);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH2_MINUS)) runtime_decrement_trim(runtime, CH2);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH3_PLUS)) runtime_increment_trim(runtime, CH3);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH3_MINUS)) runtime_decrement_trim(runtime, CH3);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH4_PLUS)) runtime_increment_trim(runtime, CH4);
	if(get_key_pressed(PIN_TRIM, SIG_TRIM_CH4_MINUS)) runtime_decrement_trim(runtime, CH4);
};


void runtime_initialize(RUNTIME* runtime) {
	runtime_setup_hardware(runtime);
	runtime_calculate_signal_parameters(runtime);
	runtime_reset(runtime);
	/**Enable global interrupts **/
	sei();
	/** start our timer **/
	TIMSK |= (1<<OCIE1A);
	/** enable the adc **/
	ADCSRA |= (1<<ADSC);
}

void runtime_run(RUNTIME* runtime) {
	while(1){
			/** read analog inputs **/
			runtime_read_analog_input(runtime, AI0);
			runtime_read_analog_input(runtime, AI1);
			runtime_read_analog_input(runtime, AI2);
			runtime_read_analog_input(runtime, AI3);
			runtime_read_analog_input(runtime, AI4);
			runtime_read_analog_input(runtime, AI5);
			runtime_read_analog_input(runtime, AI6);
			runtime_read_analog_input(runtime, AI7);

			/** read digital inputs **/
			runtime_read_digital_input(runtime, INP8, DI0);
			runtime_read_digital_input(runtime, INP9, DI1);
			runtime_read_digital_input(runtime, INP10, DI2);
			runtime_read_digital_input(runtime, INP11, DI3);
			runtime_read_digital_input(runtime, INP12, DI4);
			runtime_read_digital_input(runtime, INP13, DI5);
			runtime_read_digital_input(runtime, INP14, DI6);
			runtime_read_digital_input(runtime, INP15, DI7);


			/** apply the mixes **/
//			uint8_t idx_mixes = -1;
//			while(++idx_mixes < MAX_MIXES){
//				mix_apply_transform(&(runtime.model.mixes[idx_mixes]));
//			};
		}
}

void runtime_serialize_to_eeprom(RUNTIME* runtime) {
	model_serialize_to_eeprom(&runtime->model);
}
void runtime_deserialize_from_eeprom(RUNTIME* runtime) {

}


/**************************************** read_analog *****************************
** This method reads the analog input from the ADC.
**/

uint16_t read_analog(uint8_t ch) {
	ch = ch & 0x07;					/** zero out all bits except last three for limiting channel **/
	ADMUX &=0xE0;					/**reset channel selection **/
	ADMUX |=ch;						/**select the requested channel **/
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|= (1<<ADIF);
	return (ADC);
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
