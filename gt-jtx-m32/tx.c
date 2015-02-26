/*
 * tx.c
 *
 * Created: 2/23/2015 2:01:32 PM
 *  Author: gsarwal
 */ 

#include "tx.h"



/************************************************************************/
/*  function: Intialize a tx object
*/
/************************************************************************/
void tx_init(struct TX* tx){
	
	tx->min_signal_width = 0;
	tx->max_signal_width = 0;
	tx->inter_channel_width = 0;
	tx->frame_width = 0;
	tx->eeprom_state = 0;

	memset(tx->pilot_name, 0, sizeof(char) * 24);
	memset(tx->calibration_lower, 0, sizeof(uint16_t) * NUM_CHANNELS);
	memset(tx->calibration_upper, 0, sizeof(uint16_t) * NUM_CHANNELS);
}

/************************************************************************/
/*	function: load the default values, can be used to reset the tx system
	
*/
/************************************************************************/
void tx_load_defaults(struct TX* tx){
	tx_init(tx);
	
	tx->min_signal_width = MIN_SIGNAL;
	tx->max_signal_width = MAX_SIGNAL;
	tx->inter_channel_width = ISI;
	tx->frame_width = PPM_FRAME_WIDTH;

	tx_load_default_upper_calibration(tx);
	tx_load_default_lower_calibration(tx);

	memcpy(tx->pilot_name, DEFAULT_PILOT_NAME, PILOT_NAME_LENGTH);
	
	model_load_defaults(&(tx->model));
	
	tx->eeprom_state = MAGIC_NUMBER;
}

/************************************************************************/
/*	function: loads default calibration data to upper calibration
	params:
		tx:		the tx object to load to
*/
/************************************************************************/
void tx_load_default_upper_calibration(struct TX* tx){
	uint8_t idx = -1;
	while(++idx < NUM_CHANNELS){
		tx->calibration_upper[idx] = CALIBRATION_UPPER_END;	
	}
}


/************************************************************************/
/*	function: loads default calibration data to lower calibration
	params:
		tx:		the tx object to load to
*/
/************************************************************************/
void tx_load_default_lower_calibration(struct TX* tx){
		uint8_t idx = -1;
		while(++idx < NUM_CHANNELS){
			tx->calibration_lower[idx] = CALIBRATION_LOWER_END;
		}
}

/************************************************************************/
/*	function: set basic values of the tx such as ppm frame timings
	params:
		min_signal_width_us:	minimum signal with in microseconds
		max_signal_width_us:	maximum signal widht in microseconds
		inter_channel_width_us: the delay between channels multiplexing
		frame_width_us:			the total frame width, use to calculate sync channel
*/
/************************************************************************/
void tx_set(struct TX* tx, uint16_t min_signal_width_us, uint16_t max_signal_width_us, uint16_t inter_channel_width_us, uint16_t frame_width_us){
	
}

/************************************************************************/
/*	function: stores the values of the calibration data as test values so
				the calibration routine can set them correctly
				*** These are not default calibration values ***
	params:
		tx:		the tx object on which to set
*/
/************************************************************************/
void tx_load_no_calibration(struct TX* tx) {
	uint8_t idx = -1;
	while(++idx < NUM_CHANNELS){
		tx->calibration_upper[idx] = 0;	
	}
	idx = -1;
	while(++idx < NUM_CHANNELS){
		tx->calibration_lower[idx] = 1024;
	}
}

/************************************************************************/
/*	function: store the upper calibration values
	params:
		channel: the channel for which storing, maps to array index
		val:	the value to store
*/
/************************************************************************/
void tx_set_calibration_upper(struct TX* tx, uint16_t channel, uint16_t val){
	
}

/************************************************************************/
/*	function: store the lower calibration values
	params:
		channel: the channel for which storing, maps to array index
		val:	the value to store
*/
/************************************************************************/
void tx_set_calibration_lower(struct TX* tx, uint16_t channel, uint16_t val){
	
}

/************************************************************************/
/*	function: saves the transmitter settings to eeprom
	params:
		tx:		the transmitter object to save
*/
/************************************************************************/
void tx_save_to_eeprom(struct TX* tx, void* to_location){
	eeprom_write_block(tx, to_location, sizeof(struct TX)); 
}

/************************************************************************/
/*	function: loads the transmitter settings form eeprom
	params:
		tx:		after loading, tx will have the values
*/
/************************************************************************/
void tx_load_from_eeprom(struct TX* tx, void* from_location){
	if(0 == tx){
		tx = (struct TX*)malloc(sizeof(struct TX));
	}
	eeprom_read_block(tx, from_location, sizeof(struct TX));
	
}

/************************************************************************/
/*	function: sets last select model in eeprom
	params:
		tx:		the tx object on which to set
		model:	model data
*/
/************************************************************************/
void tx_set_model(struct TX* tx, void* to_location, struct MODEL* model){
	tx->model = *model;	
	eeprom_update_block(tx, to_location, sizeof(struct TX));
}


/************************************************************************/
/*	function: calculates the parameters required for ppm generation
	params:
		tx:		the tx object on which to set
		model:	model data
*/
/************************************************************************/
void tx_calculate_parameters(struct TX* tx){
	tx->mid_signal_width = (tx->min_signal_width + tx->min_signal_width) /2 + TRIM_CENTER;
	tx->sync_signal_width = (tx->frame_width- (NUM_CHANNELS * (tx->mid_signal_width + tx->inter_channel_width)));	 /**whatever is left **/
	tx->signal_traversal = tx->max_signal_width - tx->min_signal_width;
}

