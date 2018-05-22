/*
 * model.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#include "mix.h"
#include "common.h"
#ifndef INCLUDES_MODEL_H_
#define INCLUDES_MODEL_H_

#define MAX_LEN_NAME 16
#define MAX_MIXES 128
typedef struct {
	uint8_t uuid[4];					/** 4 bytes of uuid **/
	MIX mixes[MAX_MIXES];				/** array of mixes **/
	uint8_t trims[NUM_ANALOG_INPUTS];	/** array of trims **/
	uint8_t cbSize;						/** size of this model **/
} MODEL;

void model_save_trim(MODEL*, uint8_t channel);
void model_write_to_eeprom(MODEL*);

void model_serialize_to_eeprom(MODEL*);
void model_deserialize_from_eeprom(MODEL*);

#endif /* INCLUDES_MODEL_H_ */
