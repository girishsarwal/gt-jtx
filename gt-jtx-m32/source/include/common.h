/*
 * common.h
 *
 *  Created on: 22-May-2018
 *      Author: Girish Sarwal
 *      Email:  g.sarwal@gmail.com
 *      	 , 	girish.sarwal@gluedtomatoes.com
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#define BAUDRATE 19200	/** USART Baudrate **/
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

#define DEBUG

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <math.h>

/** Analog inputs are 0-7 on Port A**/
#define PORT_ANALOG	PORTA
#define DDR_ANALOG	DDRA
#define PIN_ANALOG	PINA

#define AI0		0    //Mapped to PA0
#define AI1		1    //Mapped to PA1
#define AI2		2    //Mapped to PA2
#define AI3		3    //Mapped to PA3
#define AI4 	4    //Mapped to PA4
#define AI5		5    //Mapped to PA5
#define AI6		6    //Mapped to PA6
#define AI7		7    //Mapped to PA7


#define NUM_ANALOG_INPUTS 8

#define PORT_DIGITAL	PORTB
#define DDR_DIGITAL		DDRB
#define PIN_DIGITAL		PINB

/** Digital inputs are 0-7 on Port B **/
#define DI0		0    //Mapped to PB0
#define DI1		1    //Mapped to PB1
#define DI2		2    //Mapped to PB2
#define DI3		3    //Mapped to PB3
#define DI4 	4    //Mapped to PB4
#define DI5		5    //Mapped to PB5
#define DI6		6    //Mapped to PB6
#define DI7		7    //Mapped to PB7

#define NUM_DIGITAL_INPUTS 8

#define NUM_PHYSICAL_INPUTS (NUM_ANALOG_INPUTS + NUM_DIGITAL_INPUTS)

#define	INP0 	AI0
#define INP1	AI1
#define INP2	AI2
#define INP3	AI3
#define INP4	AI4
#define INP5	AI5
#define INP6	AI6
#define INP7	AI7

#define INP8	NUM_ANALOG_INPUTS + DI0
#define INP9	NUM_ANALOG_INPUTS + DI1
#define INP10	NUM_ANALOG_INPUTS + DI2
#define INP11	NUM_ANALOG_INPUTS + DI3
#define INP12	NUM_ANALOG_INPUTS + DI4
#define INP13	NUM_ANALOG_INPUTS + DI5
#define INP14	NUM_ANALOG_INPUTS + DI6
#define INP15	NUM_ANALOG_INPUTS + DI7

/** Digital inputs are represented by index following the first 16 inputs**/

#define VINP0		16
#define VINP1		17
#define VINP2		18
#define VINP3		19
#define VINP4		20
#define VINP5		21
#define VINP6		22
#define VINP7		23

#define NUM_VIRTUAL_INPUTS 8

#define CH1		0		//Mapped to A
#define CH2		1		//Mapped to E
#define CH3		2		//Mapped to T
#define CH4		3		//Mapped to R
#define CH5		4		//Mapped to 5
#define CH6		5		//Mapped to 6
#define CH7		6		//Mapped to 7
#define CH8		7		//Mapped to 8

/** total inputs are the sum of all **/
#define NUM_INPUTS (NUM_ANALOG_INPUTS + NUM_DIGITAL_INPUTS + NUM_VIRTUAL_INPUTS)

/** Trim ports **/
#define PORT_TRIM		PORTC
#define DDR_TRIM		DDRC
#define PIN_TRIM		PINC

#define SIG_TRIM_CH1_PLUS 		0
#define SIG_TRIM_CH1_MINUS 		1
#define SIG_TRIM_CH2_PLUS 		2
#define SIG_TRIM_CH2_MINUS 		3
#define SIG_TRIM_CH3_PLUS 		4
#define SIG_TRIM_CH3_MINUS 		5
#define SIG_TRIM_CH4_PLUS 		6
#define SIG_TRIM_CH4_MINUS 		7

#define TRIM_UPPER_END 200
#define TRIM_LOWER_END 0
#define TRIM_CENTER 100

/** Output Port Mappings **/

#define NUM_OUTPUTS	8
#define SYNC NUM_OUTPUTS
#define NUM_OUTPUTS_INCLUDING_SYNC	(NUM_OUTPUTS + 1)

/** PPM is output on Port D **/
#define PORT_PPM		PORTD
#define DDR_PPM		DDRD
#define PIN_PPM		PIND

/** Speaker for voice **/
#define PORT_SPEAK		PORTD
#define DDR_SPEAK		DDRD

#define SIG_PPM		5
#define SIG_SPEAK	4

#define HIBYTE(x) (x >> 8)
#define LOBYTE(x) (x & 0xFF)

#define HIWORD(x) (x >> 4)
#define LOWORD(x) (x & 0xF0)

#define FRAME_START 0x7E
#define FRAME_END 0x7E
#define DLE 0x7D
#define SUBS(x)	x ^ 0x20

#define US_TO_TICKS(x) (x * ((F_CPU/1000)/1000))

void memset16(uint16_t* a, uint16_t value, uint8_t size);

#define MAX_BUFFER_SIZE 128
enum BUFFER_STATUS {
	BUFFER_OK,
	BUFFER_EMPTY,
	BUFFER_FULL,
};


#define DEFAULT_SETUP_STATE	252
#define DEFAULT_MIN_SIGNAL_WIDTH 700
#define DEFAULT_MAX_SIGNAL_WIDTH 1700
#define DEFAULT_INTER_CHANNEL_WIDTH 300
#define DEFAULT_FRAME_WIDTH 22500
#define DEFAULT_UPPER_CALIBRATION 0
#define DEFAULT_LOWER_CALIBRATION 1024




#endif /* INCLUDES_COMMON_H_ */
