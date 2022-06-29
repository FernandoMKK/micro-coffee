/*
 * micro-coffee-firmware.cpp
 *
 * Created: 27/06/2022 15:40:20
 * Author : Faculdade
 */ 

#include "drinkMaker.h"
#include "display7seg.h"
#include "serial.h"
#include <util/delay.h>

//Creates objetcs
DrinkMaker drinkMaker;
Serial serial;
Display7seg _7seg;

//Variables
volatile unsigned char menu_index = 1;

enum commands {SHOW_PROFIT, DEBUG_MODE, EXIT};
volatile unsigned char _command;

//Flags
volatile bool debug_mode = false;

ISR(INT0_vect)
{
	if(menu_index < 7)
		menu_index++;
	else
		menu_index = 1;
}

ISR(INT1_vect)
{
	if(tst_bit(PIND, PORTD2) == 0)
		drinkMaker.makeDrink(&menu_index);
	else
		drinkMaker.refuel();
}

ISR(USART_RX_vect)
{
	char receivedByte = UDR0;
	
	if (receivedByte == '0')
		_command = 0;
	if (receivedByte == '1')
		_command = 1;
	if (receivedByte == '2')
		_command = 2;
	

	switch(_command){
		case SHOW_PROFIT:
			drinkMaker.show_profit();
			break;
			
		case DEBUG_MODE:
			debug_mode = true;
			break;
			
		case EXIT:
			debug_mode = false;
			serial.show_menu();
			break;
	}
}


int main(void)
{
	//Interrupts configuration
	EICRA = (1<<ISC11) | (1<<ISC10); //Rising edge to INT1
	EICRA |= (1<<ISC01) | (1<<ISC00); //Rising edge to INT0
	
	EIMSK = (1<<INT1) | (1<<INT0); //Enables INT0 and INT1
	sei();
	
	//Direction ports configuration
	clr_bit(DDRD, PORTD2); //D2 as input
	clr_bit(DDRD, PORTD3); //D3 as input
	
	_7seg.attachToPorts(PORTC0, PORTC1, PORTC2, PORTC3);
	serial.show_menu();
	
    while (1) 
    {
		_7seg.updateDisplay(&menu_index);
    }
}

