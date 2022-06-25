/*
 * display-7seg.c
 *
 * Created: 25/06/2022 15:51:44
 * Author : Fernando Kirchner
 *
 * Description: This project intend to simulate the 7 segments LED display. By pressing the button (PORTD2), a counter increments
 *				and changes the number shown at the display. The counter counts up from 1 to 7 in loop.
 */ 

#include "seg7display.h"

volatile unsigned char menu_index = 1;

ISR(INT0_vect)
{
	if(menu_index < 7)
		menu_index++;
	else
		menu_index = 1;
}

void setup()
{
	/*--Setting the direction of the ports--*/
	DDRD = 0x00; //Sets the button (PORTD2) as input
	DDRC = 0xFF; //Sets the 4 outputs (C0, C1, C2 and C3) to decode binary to 7seg as outputs
	
	/*--Setting the interruption registers--*/
	EICRA = (1<<ISC01) | (1<<ISC00); //Trigs INT0 with rising-edge
	EIMSK = (1<<INT0); //Enables the INT0 interrupt
	sei(); //Enables global interrupt
}

int main(void)
{
    setup();
    while (1) 
    {
		update7segDisplay(&menu_index);
    }
}

