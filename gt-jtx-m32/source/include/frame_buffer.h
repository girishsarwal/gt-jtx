/*
 * circular_buffer.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_FRAME_BUFFER_H_
#define INCLUDES_FRAME_BUFFER_H_
#include "common.h"
#include "frame.h"

typedef struct{
	FRAME* data[MAX_BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;

} FRAME_BUFFER;

void frame_buffer_reset(FRAME_BUFFER* buffer);
uint8_t frame_buffer_empty(FRAME_BUFFER* buffer);
uint8_t frame_buffer_full(FRAME_BUFFER* buffer);
void frame_buffer_get(FRAME_BUFFER* buffer, FRAME* data);
void frame_buffer_put(FRAME_BUFFER* buffer, FRAME* data);

#endif /* INCLUDES_CIRCULAR_BUFFER_H_ */
