/*
 * circular_buffer.c
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */
#include "../includes/circular_buffer.h"
void circular_buffer_reset(CIRCULAR_BUFFER_T* buffer) {
	if(!buffer)
		return;
	buffer->head = buffer->tail= 0;
};

uint8_t circular_buffer_empty(CIRCULAR_BUFFER_T* buffer) {
	return (buffer->head == buffer->tail);
};
uint8_t circular_buffer_full(CIRCULAR_BUFFER_T* buffer) {
	return ((buffer->head + 1) % MAX_BUFFER_SIZE) == buffer->tail;
};
void circular_buffer_get(CIRCULAR_BUFFER_T* buffer, uint8_t *data) {
    if(data && !circular_buffer_empty(buffer))
    {
        *data = buffer->data[buffer->tail];
        buffer->tail = (buffer->tail+ 1) % MAX_BUFFER_SIZE;
    }
};
void circular_buffer_put(CIRCULAR_BUFFER_T* buffer, uint8_t data) {
	buffer->data[buffer->head] = data;
	buffer->head = (buffer->head + 1) % MAX_BUFFER_SIZE;
	if(buffer->head == buffer->tail)
	{
	    buffer->tail = (buffer->tail + 1) % MAX_BUFFER_SIZE;
	}
};
