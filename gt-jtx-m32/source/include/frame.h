/*
 * frame.h
 *
 *  Created on: 12-Jun-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_FRAME_H_
#define INCLUDES_FRAME_H_
#include "common.h"

typedef struct {
	uint8_t opcode;
	uint8_t length;
	uint8_t* data;
	uint8_t checksum;
} FRAME;

void frame_new(FRAME*, uint8_t, uint8_t*, uint8_t);
#endif /* INCLUDES_FRAME_H_ */
