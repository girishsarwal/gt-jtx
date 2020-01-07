/*
 * Usart.h
 *
 *  Created on: 07-Jan-2020
 *      Author: girishsarwal
 */

#ifndef INCLUDE_USART_H_
#define INCLUDE_USART_H_
class Runtime;
#include "runtime.h"

#define USART_WAIT_HEADER 	0x00
#define USART_IN_MSG		0x01
#define USART_AFTER_ESC		0x02

class Usart {
private:
	Runtime* m_pRuntime;
public:
	uint8_t state;
	uint8_t baud;
	uint8_t parity;

public:
	void reset();
	void setRuntime(Runtime* runtime);	/** set the owning runtime **/
	Usart();
	virtual ~Usart();
};




#endif /* INCLUDE_USART_H_ */
