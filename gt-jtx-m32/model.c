/*
 * model.cpp
 *
 * Created: 2/23/2015 1:53:42 PM
 *  Author: gsarwal
 */ 

#include "model.h"


/************************************************************************/
/*	function: intialize the model values
	params:
		model: the model number
*/
/************************************************************************/
void model_init(struct MODEL* model){
	memset(model->name, 0 , 16);
	memset(model->trims, 0, sizeof(uint8_t) * NUM_CHANNELS);

	model->reverse = 0;
}


/************************************************************************/
/*	function: Loads default values to the model and saves the model to eeprom
	params: 
		model: the model number
*/
/************************************************************************/
void model_load_defaults(struct MODEL* model){
	model_init(model);
	
	memcpy(&(model->name), DEFAULT_NAME, sizeof(char) * MODEL_NAME_LENGTH);
	uint8_t idx = -1;
	while(++idx < NUM_CHANNELS){
		model->trims[idx] = TRIM_CENTER;
	}
}


/************************************************************************/
/*	function: change the model trims and set it in eeprom
	params:
		model: the model number
		channel_step:
			HN: channel 0-16
			LN:
				D4: sign (+/-)
				D3: size of step (0-7)
*/                                                                     
/************************************************************************/

void model_change_trim(struct MODEL* model, uint8_t channel_step){
	
}