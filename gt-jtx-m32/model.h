/*
 * model.h
 *
 * Created: 2/23/2015 1:38:18 PM
 *  Author: gsarwal
 */ 


#ifndef MODEL_H_
#define MODEL_H_
#include "gt-jtx-defines.h"
#include <avr/io.h>
#define MODEL_NAME_LENGTH 16

typedef struct MODEL{
	char name[MODEL_NAME_LENGTH];
	uint8_t trims[NUM_CHANNELS];
	uint8_t reverse;
};

void model_init(struct MODEL* model);
void model_load_defaults(struct MODEL* model);
void model_change_trim(struct MODEL* model, uint8_t channel_step);


#endif /* MODEL_H_ */