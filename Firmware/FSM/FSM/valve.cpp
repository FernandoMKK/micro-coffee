/*
 * valve.cpp
 *
 * Created: 27/06/2022 17:03:29
 *  Author: Fernando Kirchner
 */ 

#include "valve.h"

Valve::Valve()
{
	//ctor
}

Valve::~Valve()
{
	//dtor
}

void Valve::attachToPort(unsigned char port)
{
	this->port = port;
	set_bit(DDRB, port);
}

void Valve::turnOn()
{
	set_bit(PORTB, this->port);
	this->state = true;
}

void Valve::turnOff()
{
	clr_bit(PORTB, this->port);
	this->state = false;
}

bool Valve::getState()
{
	return this->state;
}