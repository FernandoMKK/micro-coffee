/*
 * Serial-debugger.c
 *
 * Created: 26/06/2022 08:06:09
 * Author : Fernando Kirchner
 *
 * Description: This code intends to emulate a debugger via serial. It is able to send and read commands
 */ 
#include "USART.h"

enum commands {SHOW_PROFIT, DEBUG_MODE, EXIT};
volatile enum commands _command;
volatile unsigned char command_OK = 0;

unsigned char debug_mode = 0;
volatile unsigned char show_OK = 0;


ISR(USART_RX_vect)
{
	command_OK = 0;
	show_OK = 0;
	char receivedByte = UDR0;
		
	if (receivedByte == '0')
		_command = 0;
	if (receivedByte == '1')
		_command = 1;
	if (receivedByte == '2')
		_command = 2;
	command_OK = 1;
}
void setup()
{
	/*-- Setting direction registers --*/
	DDRD = 0b00000010;
	/*--Setting the USART registers--*/
	UCSR0B = (1<<TXEN0) | (1<<RXEN0); //Enables the TX and RX
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); //Sets bytesize 8
	UCSR0B |= (1<<RXCIE0); //Enables the RX interrupt
	
	//Sets the baudrate
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	
	//Enables global interrupt
	sei();
}

int main(void)
{
	setup();
    show_menu();
    while (1) 
    {
		if(command_OK)
		{
			switch(_command){
				case SHOW_PROFIT:
				if(debug_mode == 1){
					USART_Transmit("mostrando profit");
					USART_TransmitChar(NEWLINE);
				}
				if(show_OK == 0)
					show_profit();
					show_OK = 1;
				break;
				
				case DEBUG_MODE:
				if(debug_mode == 1){
					USART_Transmit("Modo debug");
					USART_TransmitChar(NEWLINE);
				}
				debug_mode = 1;
				break;
				
				case EXIT:
				debug_mode = 0;
				if(show_OK == 0)
					show_menu();
					show_OK = 1;
				break;
			}
		}
		_delay_ms(250);
    }
}

