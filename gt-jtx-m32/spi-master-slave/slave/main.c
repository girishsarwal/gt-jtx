// ***********************************************************
// Project:
// Author:
// Module description:
// ***********************************************************

#include <avr\io.h>              // Most basic include files
#include <avr\interrupt.h>       // Add the necessary ones
#include <avr\signal.h>          // here
#define HIBYTE(x) (x>>8)
#define LOBYTE(x) (x & 0xFF)

volatile uint8_t currentOpCode = 0;
volatile uint16_t currentData = 0;
volatile uint8_t state = 0;

volatile uint16_t ppm[] =
        {
            100,
            200,
            300,
            400,
            500,
            600,
            700,
            800,
        };

uint16_t processMessage(uint8_t opcode);

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

enum STATE{
   WAIT_RESET  = 0x00,
	WAIT_OPCODE = 0x01,
   WAIT_HIBYTE = 0x02,
   WAIT_LOBYTE = 0x03,
};

ISR(SPI_STC_vect);
void setupSPISlave(){
	state = WAIT_OPCODE;
	DDRB = (1<<PB4);
	//SPDR = 0xFF;
	SPCR = (1<<SPE) | (1<<SPIE) | (1<<SPR0) | (1<<SPR1)| (1<<CPOL);
	sei();
};


int main(void) {
   setupSPISlave();
   while(1) {             // Infinite loop; define here the
		/**other gt-jtx stuff **/		
   }

};
uint16_t processMessage(uint8_t data){
 	switch (currentOpCode)
	{
     	case RESET:
  	     	state = WAIT_OPCODE;
  	     	SPDR = 0x00;		
     	   break;
      case GCV:
        	currentData = (ppm[(data >> 4)]);
        	break;
   }
};

ISR(SPI_STC_vect){
	uint8_t data = SPDR;
	if(RESET == data){
	 	SPDR = 0x00;
    	state = WAIT_OPCODE;
   	return;
	}
	else{
		switch (state){
   		case WAIT_OPCODE:
      		currentOpCode = data;
	         state = WAIT_HIBYTE;
	         SPDR = data;     				/**return the opcode as next byte return so master knows what data is being returned**/
	         return ;
	      case WAIT_HIBYTE:
				currentData = processMessage(data);
				SPDR = HIBYTE(currentData);
      	   state = WAIT_LOBYTE;
				return;
   	  	case WAIT_LOBYTE:
      	   SPDR =  LOBYTE(currentData);
         	state = WAIT_RESET;
				return;
   	   case WAIT_RESET:
      	  	SPDR = 0x00;
      		state = WAIT_OPCODE;
	      	return;
		}
	}
};
