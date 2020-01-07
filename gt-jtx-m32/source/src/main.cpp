/*
 * main.cpp
 *
 *  Created on: 07-Jan-2020
 *      Author: girishsarwal
 */

#include "../include/runtime.h"


/***************************************** Interrupt Declarations *****************************************/
ISR(TIMER1_COMPA_vect); /** PPM timer elapsed **/
ISR(USART_RXC_vect);	/** USART Data Received **/

Runtime* runtime = NULL;

int main(void) {
	runtime = new Runtime();
}

/***************************************** PPM Interrupt ********************************1*********/
ISR(TIMER1_COMPA_vect) {
	TIMSK &= ~(1 << OCIE1A);
	if ((PIN_PPM & (1 << SIG_PPM)) == (1 << SIG_PPM)) {	//If the actual pin is high, we need to set OCR1A to the complementary delay
		OCR1A = US_TO_TICKS(runtime->m_Cppm[runtime->m_Currentchannel++]);
		if (runtime->m_Currentchannel > SYNC) {
			runtime->m_Currentchannel = CH1;
		}
	} else {
		OCR1A = US_TO_TICKS(runtime->m_settings->m_InterChannelWidthUs);
	}
	TIMSK |= (1 << OCIE1A);
}

ISR(USART_RXC_vect) {
	uint8_t data;
	data = UDR; /** read from the UDR **/
	//circular_buffer_put(&rx_buffer, data); /** add to the circular buffer for main loop to process **/
};
