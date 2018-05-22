/*
 * settings.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_SETTINGS_H_
#define INCLUDES_SETTINGS_H_
#include "common.h"
typedef struct {
	uint8_t setup_state;
	uint16_t min_signal_width_us;
	uint16_t max_signal_width_us;
	uint16_t inter_channel_width_us;
	uint16_t frame_width_us;
	uint8_t num_hw_input;
	uint16_t upper_calibration[NUM_ANALOG_INPUTS];
	uint16_t lower_calibration[NUM_ANALOG_INPUTS];
} SETTINGS;

void settings_serialize_to_eeprom(SETTINGS*);
void settings_deserialize_from_eeprom(SETTINGS*);

void calibration_serialize_to_eeprom(SETTINGS*);
void calibration_deserialize_to_eeprom(SETTINGS*);

#endif /* INCLUDES_SETTINGS_H_ */
