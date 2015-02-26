/*
 * tx.h
 *
 * Created: 2/23/2015 1:56:25 PM
 *  Author: gsarwal
 */ 
#ifndef TX_H_
#define TX_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#include "gt-jtx-defines.h"
#include "model.h"

#define LOWER_CALIBRATION_MISSING	0x01
#define UPPER_CALIBRATION_MISSING	0x02
#define DEFAULT_MODEL_MISSING		0x04

struct TX{
	uint16_t	eeprom_state;						/** must match magic number, if not means eeprom is new and needs setup **/
	uint8_t		setup_state;						/** has any critical error flags, each bit corresponds to
														D2 - default model missing
														D1 - upper_calibration missing
														D0 - lower_calibration missing **/
	
	char		pilot_name[PILOT_NAME_LENGTH];		/** name of the pilot **/
	
	struct MODEL		model;								/** the model **/
	
	uint16_t 	min_signal_width;					/** ppm max signal width (ms) **/
	uint16_t 	max_signal_width;					/** ppm min signal width (ms) **/
	uint16_t	mid_signal_width;					/** the 'center' of the signal. This PPM value should represent 50% physical stick **/
	uint16_t 	inter_channel_width;				/** ppm inter channel width (ms) **/
	uint16_t 	frame_width;						/** the total width of the ppm frame (ms) **/
	uint16_t	sync_signal_width;					/** the length of the sync signal (ms), calculated based on other variables **/
	uint16_t	signal_traversal;					/** the range of the servo signal (ms), for lerps to calculate signal **/
	uint16_t	calibration_upper[NUM_CHANNELS];
	uint16_t	calibration_lower[NUM_CHANNELS];
};

void tx_init(struct TX* tx);
void tx_load_defaults(struct TX* tx);
void tx_load_no_calibration (struct TX* tx);
void tx_load_default_upper_calibration(struct TX* tx);
void tx_load_default_lower_calibration(struct TX* tx);

void tx_set(struct TX* tx, uint16_t min_signal_width_us, uint16_t max_signal_width_us, uint16_t inter_channel_width_us, uint16_t frame_width_us);
void tx_set_calibration_upper(struct TX* tx, uint16_t channel, uint16_t value);
void tx_set_calibration_lower(struct TX* tx, uint16_t channel, uint16_t value);
void tx_save_to_eeprom(struct TX* tx, void* to_location);
void tx_load_from_eeprom(struct TX* tx, void* from_location);
void tx_set_model(struct TX* tx, void* from_location, struct MODEL* model);

void tx_set_setup_state(struct TX* tx, uint8_t mask, uint8_t value);

void tx_calculate_parameters(struct TX* tx);

#endif /* TX_H_ */