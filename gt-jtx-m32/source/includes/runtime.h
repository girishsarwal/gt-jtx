/*
 * runtime.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_RUNTIME_H_
#define INCLUDES_RUNTIME_H_
#include "common.h"
#include "settings.h"
#include "model.h"
#include "circular_buffer.h"
#include "usart.h"
#include "spi.h"

typedef struct {
	uint16_t physical_inputs[NUM_PHYSICAL_INPUTS];
	uint16_t ppm[NUM_OUTPUTS_INCLUDING_SYNC];

	uint16_t 	signal_traversal_us;
	uint16_t 	mid_signal_width_us;
	uint16_t 	sync_signal_width_us;
	uint8_t 	error;			/** startup errors **/
	uint8_t 	channel;
	uint8_t 	idx;

	CIRCULAR_BUFFER_T rx_queue;		/** receive queue from front end **/
	CIRCULAR_BUFFER_T tx_queue;		/** transmit queue to front end **/

	uint8_t		usart_state;
	/** from here on everything will be serialized to eeprom so it gets loaded on the next bootup **/

	SETTINGS settings;			/** serialized to eeprom **/
	MODEL model;				/** serialized to eeprom **/
} RUNTIME, *PRUNTIME;

void runtime_new(RUNTIME*);
void runtime_reset(RUNTIME*);
void runtime_setup_hardware(RUNTIME*);
void runtime_calculate_signal_parameters(RUNTIME*);
void runtime_read_analog_input(RUNTIME*, uint8_t);
void runtime_read_digital_input(RUNTIME*, uint8_t, uint8_t);
void runtime_increment_trim(RUNTIME*, uint8_t);
void runtime_decrement_trim(RUNTIME*, uint8_t);
void runtime_calibrate_channel(RUNTIME*, uint8_t);

void runtime_process_trim_inputs(RUNTIME*);

void runtime_initialize(RUNTIME*);
void runtime_run(RUNTIME*);

void runtime_serialize_to_eeprom(RUNTIME*);
void runtime_deserialize_from_eeprom(RUNTIME*);

uint16_t read_analog(uint8_t);
uint8_t get_key_pressed(uint8_t, uint8_t);


#endif /* INCLUDES_RUNTIME_H_ */
