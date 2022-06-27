/*
 * motor.cpp
 *
 * Created: 27/06/2022 16:14:28
 *  Author: Fernando Kirchner
 */ 

#include "motor.h"

Motor::Motor()
{
	//ctor
}

Motor::~Motor()
{
	//dtor
}

void Motor::setup()
{
	DDRB |= (1<<PORTB3); //Sets the PB3 as output (OC2A)
	
	TCCR2A = (1<<WGM21); //Sets the timer mode as CTC
	TCCR2A |= (1<<COM2A0); //Toggle output on compare match
	TCCR2B = 0; //Disable Timer 2
	OCR2A = 124; //Sets the compare value at 124
}

void Motor::turnOn()
{
	TCCR2B = (1<<CS21) | (1<<CS20); //Set prescale = 64
}

void Motor::turnOff()
{
	TCCR2B = 0;//Disable timer 2
}