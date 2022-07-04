/*
 * Serial.cpp
 *
 * Created: 27/06/2022 17:36:41
 * Author : Fernando Kirchner
 */ 

#include "serial.h"
#include <util/delay.h>

enum commands {SHOW_PROFIT, DEBUG_MODE, EXIT};
volatile unsigned char _command;
volatile bool command_OK = false;

bool debug_mode = false;
volatile bool show_OK = false;

ISR(USART_RX_vect)
{
	command_OK = false;
	show_OK = false;
	char receivedByte = UDR0;
	
	if (receivedByte == '0')
		_command = 0;
	if (receivedByte == '1')
		_command = 1;
	if (receivedByte == '2')
		_command = 2;
		
	command_OK = true;
}

int main(void)
{
	Serial serial(9600,1,8,'N');
	serial.show_menu();
	
    while (1) 
    {
		if(command_OK)
	    {
		    switch(_command){
			    case SHOW_PROFIT:
					if(debug_mode == true){
						serial.transmit("mostrando profit");
						serial.transmitChar(NEWLINE);
					}
					if(show_OK == false){
						serial.show_profit();
						show_OK = true;
					}
					break;
			    
			    case DEBUG_MODE:
					if(debug_mode == true){
						serial.transmit("Modo debug");
						serial.transmitChar(NEWLINE);
					}
					debug_mode = true;
					break;
			    
			    case EXIT:
					debug_mode = false;
					if(show_OK == false){
						serial.show_menu();
						show_OK = true;
					}
					break;
		    }
	    }
	    _delay_ms(250);
    }
}

