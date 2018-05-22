/*
 * circular_buffer.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_CIRCULAR_BUFFER_H_
#define INCLUDES_CIRCULAR_BUFFER_H_
#include "common.h"
#define MAX_BUFFER_SIZE 128
enum BUFFER_STATUS {
	BUFFER_OK,
	BUFFER_EMPTY,
	BUFFER_FULL,
};

typedef struct{
	uint8_t * data[MAX_BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;

} CIRCULAR_BUFFER_T;

void circular_buffer_reset(CIRCULAR_BUFFER_T* buffer);
uint8_t circular_buffer_empty(CIRCULAR_BUFFER_T* buffer);
uint8_t circular_buffer_full(CIRCULAR_BUFFER_T* buffer);
void circular_buffer_get(CIRCULAR_BUFFER_T* buffer, uint8_t *data);
void circular_buffer_put(CIRCULAR_BUFFER_T* buffer, uint8_t data);

#endif /* INCLUDES_CIRCULAR_BUFFER_H_ */
