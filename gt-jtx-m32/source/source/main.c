/*
 * main.c
 *
 *  Author: Girish Sarwal
 *	 Email: g.sarwal@gmail.com
 *       , girish.sarwal@gluedtomatoes.com
 *
 */

#include "../includes/common.h"
#include "../includes/runtime.h"

RUNTIME runtime;

/***************************************** Interrupt Declarations *****************************************/
ISR(TIMER1_COMPA_vect); /**PPM time elapsed **/
ISR(USART_RXC_vect); /** USART byte received **/

/************************************MAIN Routine *****************************************
 */

int main(void) {
	return 0;
}
;

/***************************************** PPM Interrupt ********************************1*********/
ISR(TIMER1_COMPA_vect) {
	TIMSK &= ~(1 << OCIE1A);
	if ((PIN_PPM & (1 << SIG_PPM)) == (1 << SIG_PPM)) {	//If the actual pin is high, we need to set OCR1A to the complementary delay
		OCR1A = US_TO_TICKS(runtime.ppm[runtime.channel++]);
		if (runtime.channel > SYNC) {
			runtime.channel = CH1;
		}
	} else {
		OCR1A = US_TO_TICKS(runtime.settings.inter_channel_width_us);
	}
	TIMSK |= (1 << OCIE1A);
}
;

/***************************************** USART Interrupt *****************************************/
ISR(USART_RXC_vect) {
	uint8_t data;
	data = UDR; /** read from the UDR **/
	circular_buffer_put(&runtime.rx_queue, data); /** add to the circular queue **/
}
;

//
//switch(runtime.usart_state){
//		case WAIT_HEADER:
//			if(FRAME_START == data){
//				runtime.usart_state = IN_MSG;
//				/** reset the buffer **/
//			}
//			break;
//		case IN_MSG:
//			if(DLE == data){
//				runtime.usart_state = AFTER_ESC;
//			} else if (FRAME_END == data){
//				runtime.usart_state = WAIT_HEADER;
//				/** reset the buffer **/
//			} else {
//				/** just add to the buffer **/
//			}
//			break;
//		case AFTER_ESC:
//			data = SUBS(data);	/** substitute the data to get the original byte sent **/
//			/** add the data to the buffer **/
//			runtime.usart_state = IN_MSG;
//			break;
//	}
