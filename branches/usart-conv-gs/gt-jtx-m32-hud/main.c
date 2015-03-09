// ***********************************************************
// Project:
// Author:
// Module description:
// ***********************************************************

#include <avr\io.h>              // Most basic include files
#include <util\delay.h>
#include "ks0108.h"
#include "logo.h"
#include "graphic.h"
#ifndef F_CPU
#define F_CPU 1000000
#endif


#define WARNING -2
#define ERROR -1
#define HOME 0
#define HOME2 1
#define ABOUT 101
#define SETTINGS 201

#define FIRST_TRACK_X 2
#define TRIM_TRACK_OFFSET 1
#define TRACK_Y 10
#define TRIM_TRACK_Y 10
#define TRACK_WIDTH   4
#define TRACK_HEIGHT  50
#define TRIM_TRACK_WIDTH 3
#define TRIM_TRACK_HEIGHT 50
//                 "                     "
char* pPilotName = "PILOT NAME";
char* pHomeLine1 = "A01+100 A09+100 D1:Y";
char* pHomeLine2 = "A02+099 A10+097 D2:N";
char* pHomeLine3 = "A03+098 A11-032 D3:Y";
char* pHomeLine4 = "A04+067 A12-087 D4:N";
char* pHomeLine5 = "A05-045 A13+018 D5:N";
char* pHomeLine6 = "A06+000 A14+072 D6:Y";
char* pHomeLine7 = "A07-032 A16-053 D7:N";
char* pHomeLine8 = "A08+056 A17-076 D8:Y";

uint8_t screen = ABOUT ;
void processDisplay(){
	switch(screen){
		case ABOUT:
			showAbout();
			_delay_ms(200);
			screen = HOME2;
			break;
		case HOME:
			/** read up all the channels using SPI and show bars **/
			GLCD_ClearScreen();
			GLCD_GoTo(0,0); GLCD_WriteString(pHomeLine1);
			GLCD_GoTo(0,1); GLCD_WriteString(pHomeLine2);
			GLCD_GoTo(0,2); GLCD_WriteString(pHomeLine3);
			GLCD_GoTo(0,3); GLCD_WriteString(pHomeLine4);
			GLCD_GoTo(0,4); GLCD_WriteString(pHomeLine5);
			GLCD_GoTo(0,5); GLCD_WriteString(pHomeLine6);
			GLCD_GoTo(0,6); GLCD_WriteString(pHomeLine7);
			GLCD_GoTo(0,7); GLCD_WriteString(pHomeLine8);
			_delay_ms(200);			
			break;
		case HOME2:
			/** read up all the channels using SPI and show bars **/
			GLCD_ClearScreen();
			
			volatile uint8_t x;
			x = FIRST_TRACK_X;
			
			
			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT); x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT); x+= TRIM_TRACK_WIDTH; x++;
			
			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT); x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT);  x+= TRIM_TRACK_WIDTH;x++;
			
			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT);   x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT); x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT); x+= TRACK_WIDTH;x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT); x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT); x+= TRACK_WIDTH;x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT); x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT);x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT); x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT); x+= TRACK_WIDTH;x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT);   x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT);x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT); x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT);x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT);  x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT);x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT); x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT);x+= TRACK_WIDTH; x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT);x+= TRIM_TRACK_WIDTH;x++;

			GLCD_Rectangle(x, TRACK_Y, TRACK_WIDTH, TRACK_HEIGHT);x+= TRACK_WIDTH;x--;
			GLCD_Rectangle(x, TRIM_TRACK_Y, TRIM_TRACK_WIDTH, TRIM_TRACK_HEIGHT);x+= TRIM_TRACK_WIDTH;x++;

			break;			
			
	};
};
void showAbout(){
	GLCD_GoTo(0,1);
	GLCD_WriteString("     GT-JTX-M32      ");
	GLCD_GoTo(0,3);
	GLCD_WriteString("www.gluedtomatoes.com");
	GLCD_GoTo(0,7);
	GLCD_WriteString(pPilotName);
};

int main(void){
	GLCD_Initialize();
	while(1){
		processDisplay();
	}
}


