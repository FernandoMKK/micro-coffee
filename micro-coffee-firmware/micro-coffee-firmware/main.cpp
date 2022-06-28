/*
 * micro-coffee-firmware.cpp
 *
 * Created: 27/06/2022 15:40:20
 * Author : Faculdade
 */ 

#include "display7seg.h"
#include "serial.h"
#include <util/delay.h>

//Creates objetcs
Serial serial(9600, 1, 8, 'N');
Display7seg _7seg;

//Variables
volatile unsigned char menu_index = 1;
volatile float money = 0;

enum commands {SHOW_PROFIT, DEBUG_MODE, EXIT};
volatile unsigned char _command;

//Flags
volatile bool command_OK = false;
bool debug_mode = false;
volatile bool show_OK = false;

ISR(INT0_vect)
{
	if(menu_index < 7)
		menu_index++;
	else
		menu_index = 1;
}

ISR(INT1_vect)
{
	if(tst_bit(PIND, PORTD2) == 0){
		switch(menu_index){
			case 1:
				serial.transmit("Opcao 1 Escolhida");
				serial.transmitChar(NEWLINE);
				money += 1.50;
				break;
			case 2:
				serial.transmit("Opcao 2 Escolhida");
				serial.transmitChar(NEWLINE);
				money += 2.00;
				break;
			case 3:
				serial.transmit("Opcao 3 Escolhida");
				serial.transmitChar(NEWLINE);
				money += 2.75;
				break;
			case 4:
				serial.transmit("Opcao 4 Escolhida");
				serial.transmitChar(NEWLINE);
				money += 1.00;
				break;
			case 5:
				serial.transmit("Opcao 5 Escolhida");
				serial.transmitChar(NEWLINE);
				money += 1.80;
				break;
			case 6:
				serial.transmit("Opcao 6 Escolhida");
				serial.transmitChar(NEWLINE);
				money += 1.50;
				break;
			case 7:
				serial.transmit("Opcao 7 Escolhida");
				serial.transmitChar(NEWLINE);
				money += 2.10;
				break;
			default:
				serial.transmit("Opcao invalida");
				serial.transmitChar(NEWLINE);
				break;
		}
	}
	else{
		serial.transmit("Maquina reabastecida");
		serial.transmitChar(NEWLINE);
		money = 0;
	}
	
}

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
		
		if(command_OK)
		{
			switch(_command){
				case SHOW_PROFIT:
					if(debug_mode == true){
						serial.transmit("mostrando profit");
						serial.transmitChar(NEWLINE);
					}
					if(show_OK == false){
						serial.show_profit(money);
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

