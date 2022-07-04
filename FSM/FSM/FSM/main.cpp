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
volatile unsigned char menu_index = 0;

enum commands {SHOW_PROFIT, DEBUG_MODE, EXIT};
volatile unsigned char _command;

enum states {WAITING_GLASS, SELECTING_DRINK, MILLING_COFFEE, PREPARING_DRINK, REMOVING_GLASS, REFUELLING_MACHINE};
volatile enum states _state;

volatile unsigned int timer0_cicle_counter = 0;
volatile unsigned int timer0_cicle_setpoint = 0;
volatile unsigned int timer1_cicle_counter = 0;

//Flags
volatile bool debug_mode = false;
volatile bool show_ok = false;
volatile bool stateHandler = false;
volatile bool stateHandler2 = false;

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
		drinkMaker.makeDrink(&menu_index);
		if(drinkMaker.drink_available == true){
			lcd.clear();
			_state = MILLING_COFFEE;
			stateHandler = false;
		}
		else{
			_state = SELECTING_DRINK;
			show_ok = false;
		}
		
	}
	else{
		_state = REFUELLING_MACHINE;
		stateHandler = false;
	}
	
		
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
		show_ok = false;
		stateHandler = false;
		serial.transmit("parando moedor");
		serial.transmitChar(NEWLINE);
		//drinkMaker.stopMilling();
		_state = PREPARING_DRINK;
		lcd.clear();
		timer1_cicle_counter = 0;
	}
	
}

ISR(TIMER0_OVF_vect)
{
	timer0_cicle_counter++;
	
	if(timer0_cicle_counter == timer0_cicle_setpoint){
		serial.transmit("Fechando Valvula");
		serial.transmitChar(NEWLINE);
		lcd.clear();
		_state = REMOVING_GLASS;
		show_ok = false;
		timer0_cicle_counter = 0;
		
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
	lcd.init();
	
	//motor configuration
	DDRB |= (1<<PORTB3); //Sets the PB3 as output (OC2A)
	
	TCCR2A = (1<<WGM21); //Sets the timer mode as CTC
	TCCR2A |= (1<<COM2A0); //Toggle output on compare match
	TCCR2B = 0; //Disable Timer 2
	OCR2A = 124; //Sets the compare value at 124
	
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
					_delay_ms(100);
				}
				if(show_ok == false){
					lcd.write("Insira o copo");
					show_ok = true;
				}
				
				if(tst_bit(PINB, PORTB7) > 0){
					_state = SELECTING_DRINK;
					show_ok = false;
				}
				
				case SELECTING_DRINK:
				if(debug_mode == true){
					serial.transmit("SELECTING DRINK");
					serial.transmitChar(NEWLINE);
					_delay_ms(100);
				}
				if(show_ok == false){
					lcd.clear();
					lcd.write("Selecione bebida");
					show_ok = true;
				}
				break;
				
				case MILLING_COFFEE:
				if(debug_mode == true){
					serial.transmit("MILLING_COFFEE");
					serial.transmitChar(NEWLINE);
					_delay_ms(100);
				}
				if(stateHandler == false){
					if(drinkMaker.drinkHasCoffee(&menu_index)){
						stateHandler = true;
						drinkMaker.millCoffee();
						TCCR1B = (1<<CS11); //Set TC1 prescale 8 and enables timer
					}
					else{
						lcd.clear();
						_state = PREPARING_DRINK;
						show_ok = false;
					}
					
				}
				
				break;
				
				case PREPARING_DRINK:
				TCCR2B = 0; //Motor disabled
				TCCR1B = 0; //TC1 disabled
				if(debug_mode == true){
					serial.transmit("PREPARING DRINK");
					serial.transmitChar(NEWLINE);
					_delay_ms(100);
				}
				if(stateHandler2 == false){
					stateHandler2 = true;
					lcd.clear();
					setTimerSetPoint(&menu_index);
					drinkMaker.openValves(&menu_index);
					TCCR0B = (1<<CS02) | (1<<CS00); //Set prescale 1024 and enables timer 0
				}
				
				break;
				
				case REMOVING_GLASS:
				TCCR0B = 0; //TC0 disabled
				drinkMaker.closeValves();
				if(debug_mode == true){
					serial.transmit("REMOMVING GLASS");
					serial.transmitChar(NEWLINE);
					
				}
				if (show_ok==false)
				{
					lcd.clear();
					lcd.write("Retire o copo");
					show_ok = true;
				}
				_delay_ms(100);
				if(tst_bit(PINB, PORTB7) == 0){
					lcd.clear();
					lcd.write("Obrigado");
					_delay_ms(1000);
					lcd.clear();
					_state = WAITING_GLASS;
					show_ok = false;
					stateHandler = false;
					stateHandler2 = false;
				}
				break;
				
				case REFUELLING_MACHINE:
				if(debug_mode == true){
					serial.transmit("REFUELLING_MACHINE");
					serial.transmitChar(NEWLINE);
					_delay_ms(100);
				}
				drinkMaker.refuel();
				_state = WAITING_GLASS;
				break;
			}
		}
		else{
			clr_bit(PORTC, PORTC5);
			_state = WAITING_GLASS;
		}	
	}
}
