
#define F_CPU 16000000L
#include "ks0108.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include "arial_bold_14.h"
#include "corsiva_12.h"

void main(){
// Wait a little while the display starts up
	for(volatile uint16_t i=0; i<15000; i++);
	
	// Initialize the LCD
	ks0108Init(0);
	


	while(1);
}


