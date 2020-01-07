/*
 * Usart.cpp
 *
 *  Created on: 07-Jan-2020
 *      Author: Girish Sarwal
 *      Email:	girish.sarwal@gluedtomatoes.com
 *      		g.sarwal@gmail.com
 */

#include "../include/usart.h"

Usart::Usart() {
	// TODO Auto-generated constructor stub

}
void Usart::reset(){
	state = USART_WAIT_HEADER;
}

Usart::~Usart() {
	// TODO Auto-generated destructor stub
}

