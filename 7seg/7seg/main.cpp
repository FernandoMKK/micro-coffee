/*
 * 7seg.cpp
 *
 * Created: 27/06/2022 17:18:45
 * Author : Fernando Kirchner
 */ 
#include "display7seg.h"
#include <avr/interrupt.h>

volatile unsigned char menu_index = 1;

ISR(INT0_vect)
{
	if(menu_index < 7)
		menu_index++;
	else
		menu_index = 1;
}

int main(void)
{
    Display7seg myDisplay;
	
	/*--Setting the direction of the ports--*/
	DDRD = 0x00; //Sets the button (PORTD2) as input
	myDisplay.attachToPorts(PORTC0, PORTC1, PORTC2, PORTC3);
	
	/*--Setting the interruption registers--*/
	EICRA = (1<<ISC01) | (1<<ISC00); //Trigs INT0 with rising-edge
	EIMSK = (1<<INT0); //Enables the INT0 interrupt
	sei(); //Enables global interrupt
	
	
    while (1) 
    {
		myDisplay.updateDisplay(&menu_index);			
    }
}

