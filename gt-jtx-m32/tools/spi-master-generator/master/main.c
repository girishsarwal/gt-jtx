	    		// ***********************************************************
// Project:
// Author:
// Module description:
// ***********************************************************

#include <avr\io.h>              // Most basic include files
#include <avr\interrupt.h>       // Add the necessary ones
#include <avr\signal.h>          // here
#include <util\delay.h>

enum OPCODE{
	NOP = 0x00,
   SETTUP = 0x01,
   SETTDN = 0x02,
   GETT = 0x03,
   SETREV = 0x04,
   GETREV = 0x05,
   SCUP = 0x06,
   GCUP = 0x07,
   SCDN = 0x08,
   GCDN = 0x09,
   SPPMLEN = 0x0A,
   GPPMLEN = 0x0B,
   SPPMICL = 0x0C,
   GPPMICL = 0x0D,
   SSTIM = 0x0E,
   GSTIM = 0x0F,
   GCV = 0x10,
   RESET = 0xFF,
};

void setupSPI(){
	DDRB = (1<<PB2) | (1<<PB3) | (1<<PB5);		//miso, ss and sck
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1) | (1<<CPOL);	//spi enable, master, clock to ck/128
};
char spiExchange(char data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));		//while busy flag
	return SPDR;
};
uint16_t channelValue= 0x00;
char retex= 0;

int main(void) {
	setupSPI();
	char m;
   _delay_ms(50);
   spiExchange(0xFF);
   while(1) {            							 // Infinite loop; define here the
		retex = spiExchange(SPPMLEN);					/**GCV**/
		m = 0 ;
   }
};






