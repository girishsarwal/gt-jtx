/*
 * gt_jtx_defines.h
 *
 * Created: 2/23/2015 2:17:28 PM
 *  Author: gsarwal
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#define DEFAULT_PILOT_NAME "MAVERICK"
#define PILOT_NAME_LENGTH	24

#define DEFAULT_NAME "New Model"
#define MODEL_NAME_LENGTH	16

#define AIL		0
#define ELE		1
#define THR		2
#define RUD		3
#define CH5 	4
#define CH6		5
#define CH7		6
#define CH8		7
#define SYNC	8

#define MAX_CHANNEL CH8
#define NUM_CHANNELS MAX_CHANNEL + 1

#define CALIBRATION_UPPER_END	0
#define CALIBRATION_LOWER_END	1024

#define TRIM_UPPER_END 200
#define TRIM_LOWER_END 0
#define TRIM_CENTER 100

#define SIG_TRIM_AIL_PLUS 		0
#define SIG_TRIM_AIL_MINUS 		1
#define SIG_TRIM_ELE_PLUS 		2
#define SIG_TRIM_ELE_MINUS 		3
#define SIG_TRIM_THR_PLUS 		4
#define SIG_TRIM_THR_MINUS 		5
#define SIG_TRIM_RUD_PLUS 		6
#define SIG_TRIM_RUD_MINUS 		7

#define PORT_ANALOG	PORTA
#define DDR_ANALOG	DDRA
#define PIN_ANALOG	PINA

#define PORT_TRIM		PORTB
#define DDR_TRIM		DDRB
#define PIN_TRIM		PINB

#define PORT_PPM		PORTD
#define DDR_PPM		DDRD
#define PIN_PPM		PIND

#define PORT_MENU		PORTD
#define DDR_MENU		DDRD
#define PIN_MENU		PIND

#define PORT_SPEAK	PORTD
#define DDR_SPEAK		DDRD


#define SIG_PPM				5
#define SIG_SPEAK			6
#define SIG_MENU_KEY		7

#define TRIM_TRACK_WIDTH	2
#define CHANNEL_TRACK_WIDTH 7


#define MAGIC_NUMBER		29543

#define MIN_SIGNAL			700
#define MAX_SIGNAL			1700
#define ISI					300
#define PPM_FRAME_WIDTH		22500