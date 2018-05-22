/*
 * spi.h
 *
 *  Created on: 22-May-2018
 *      Author: girishsarwal
 */

#ifndef INCLUDES_SPI_H_
#define INCLUDES_SPI_H_
#include "common.h"

enum SPI_STATE {
   WAIT_RESET  = 0x00,
   WAIT_OPCODE = 0x01,
   WAIT_HIBYTE = 0x02,
   WAIT_LOBYTE = 0x03,
};


typedef struct {
	uint8_t opcode;
	uint8_t state;
	uint8_t data0;
	uint8_t data1;
	uint16_t result;
} SPITRANSACTION;


#endif /* INCLUDES_SPI_H_ */
