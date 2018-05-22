/*
 * frame.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_USART_H_
#define INCLUDES_USART_H_
#include "common.h"

enum USART_STATE {
	WAIT_HEADER = 0x00,
	IN_MSG = 0x01,
	AFTER_ESC = 0x02,
};

typedef struct {
	uint8_t opcode;
	uint8_t length;
	uint8_t* data;
	uint8_t checksum;
} FRAME;

#endif /* INCLUDES_USART_H_ */
