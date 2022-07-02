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
LCD lcd;

//Variables
volatile unsigned char menu_index = 1;

enum commands {SHOW_PROFIT, DEBUG_MODE, EXIT};
volatile unsigned char _command;

enum states {WAITING_GLASS, SELECTING_DRINK, MILLING_COFFEE, PREPARING_DRINK, REMOVING_GLASS, REFUELLING_MACHINE};
volatile enum states _state;

volatile unsigned int timer0_cicle_counter = 0;
volatile unsigned int timer0_cicle_setpoint = 0;
volatile unsigned int timer1_cicle_counter = 0;

//Flags
volatile bool debug_mode = false;

ISR(INT0_vect)
{
	if(_state == SELECTING_DRINK){
		if(menu_index < 7)
			menu_index++;
		else
			menu_index = 1;
			
		_7seg.updateDisplay(&menu_index);
		drinkMaker.updateLCD(&menu_index);
	}
}

ISR(INT1_vect)
{
	if(tst_bit(PIND, PORTD2) == 0){
		lcd.clear();
		_state = MILLING_COFFEE;
	}
	else
		_state = REFUELLING_MACHINE;
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
			drinkMaker.debug_mode = true;
			break;
		
		case EXIT:
			debug_mode = false;
			drinkMaker.debug_mode = false;
			serial.show_menu();
			break;
	}
}

ISR(TIMER1_OVF_vect)
{
	timer1_cicle_counter++;
	
	if(timer1_cicle_counter == 91){
		drinkMaker.stopMilling();
		_state = PREPARING_DRINK;
		timer1_cicle_counter = 0;
		TCCR1B = 0; //TC1 disabled
	}
	
}

ISR(TIMER0_OVF_vect)
{
	timer0_cicle_counter++;
	
	if(timer0_cicle_counter == timer0_cicle_setpoint){
		drinkMaker.closeValves();
		lcd.clear();
		_state = REMOVING_GLASS;
		timer0_cicle_counter = 0;
		TCCR0B = 0; //TC0 disabled
	}
}

void setTimerSetPoint(volatile unsigned char *menu_index)
{
	switch(*menu_index){
		case 1: timer0_cicle_setpoint = 366;  break; //6 seconds
		case 2: timer0_cicle_setpoint = 183; break; //3 seconds
		case 3: timer0_cicle_setpoint = 122; break; //2 seconds
		case 4: timer0_cicle_setpoint = 366; break; //6 seconds
		case 5: timer0_cicle_setpoint = 183; break; //3 seconds
		case 6: timer0_cicle_setpoint = 366;  break; //6 seconds
		case 7: timer0_cicle_setpoint = 183; break; //3 seconds
	}
}

int main(void)
{
	//Timers configuration
	TCCR0A = 0; //TC0 Normal mode
	TCCR0B = 0; //TC0 disabled
	TIMSK0 = (1<<TOIE0); //Enables timer 0 overflow interrupt 
	
	TCCR1A = 0; //TC1 Normal mode
	TCCR1B = 0; //TC1 disabled
	TIMSK1 = (1<<TOIE1); //Enables timer 1 overflow interrupt
	
	//Interrupts configuration
	EICRA = (1<<ISC11) | (1<<ISC10); //Rising edge to INT1
	EICRA |= (1<<ISC01) | (1<<ISC00); //Rising edge to INT0
	
	EIMSK = (1<<INT1) | (1<<INT0); //Enables INT0 and INT1
	sei();
	
	//Direction ports configuration
	clr_bit(DDRD, PORTD2); //D2 as input
	clr_bit(DDRD, PORTD3); //D3 as input
	clr_bit(DDRC, PORTC4); //C4 as input
	set_bit(DDRC, PORTC5); //C5 as output
	
	_7seg.attachToPorts(PORTC0, PORTC1, PORTC2, PORTC3);
	serial.show_menu();
	lcd.clear();
	
	_state = WAITING_GLASS;
	
	while (1)
	{
		if(tst_bit(PINC, PORTC4) > 0){
			set_bit(PORTC, PORTC5); //Turns on LED on/off.
			switch(_state){
				case WAITING_GLASS:
				if(debug_mode == true){
					serial.transmit("WAINTING GLASS");
					serial.transmitChar(NEWLINE);
				}
				lcd.write("Insira o copo");
				if(tst_bit(PINB, PORTB7) > 0){
					_state = SELECTING_DRINK;
				}
				
				case SELECTING_DRINK:
				if(debug_mode == true){
					serial.transmit("SELECTING DRINK");
					serial.transmitChar(NEWLINE);
				}
				break;
				
				case MILLING_COFFEE:
				if(debug_mode == true){
					serial.transmit("MILLING_COFFEE");
					serial.transmitChar(NEWLINE);
				}
				if(drinkMaker.drinkHasCoffee(&menu_index)){
					drinkMaker.millCoffee();
					TCCR1B = (1<<CS11); //Set TC1 prescale 8 and enables timer
				}
				else{
					lcd.clear();
					_state = PREPARING_DRINK;
				}
				break;
				
				case PREPARING_DRINK:
				if(debug_mode == true){
					serial.transmit("PREPARING DRINK");
					serial.transmitChar(NEWLINE);
				}
				setTimerSetPoint(&menu_index);
				drinkMaker.openValves(&menu_index);
				TCCR0B = (1<<CS02) | (1<<CS00); //Set prescale 1024 and enables timer 0
				break;
				
				case REMOVING_GLASS:
				if(debug_mode == true){
					serial.transmit("REMOMVING GLASS");
					serial.transmitChar(NEWLINE);
				}
				lcd.write("Retire o Copo");
				if(tst_bit(PINB, PORTB7) == 0){
					lcd.clear();
					lcd.write("Obrigado");
					_delay_ms(1000);
					_state = SELECTING_DRINK;
				}
				break;
				
				case REFUELLING_MACHINE:
				if(debug_mode == true){
					serial.transmit("REFUELLING_MACHINE");
					serial.transmitChar(NEWLINE);
				}
				drinkMaker.refuel();
				break;
			}
		}
		else{
			clr_bit(PORTC, PORTC5);
		}	
	}
}
