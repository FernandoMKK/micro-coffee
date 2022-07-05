/*
 * display7seg.cpp
 *
 * Created: 27/06/2022 17:22:43
 *  Author: Fernando Kirchner
 */ 

#include "display7seg.h"

Display7seg::Display7seg()
{
	//ctor
}

Display7seg::~Display7seg()
{
	//dtor
}

void Display7seg::attachToPorts(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3)
{
	set_bit(DDRC, d0);
	set_bit(DDRC, d1);
	set_bit(DDRC, d2);
	set_bit(DDRC, d3);
}

void Display7seg::updateDisplay(volatile unsigned char *num)
{
	switch(*num){
		case 0:
		clr_bit(PORTC, PORTC0);
		clr_bit(PORTC, PORTC1);
		clr_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
		
		case 1:
		set_bit(PORTC, PORTC0);
		clr_bit(PORTC, PORTC1);
		clr_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
		
		case 2:
		clr_bit(PORTC, PORTC0);
		set_bit(PORTC, PORTC1);
		clr_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
		
		case 3:
		set_bit(PORTC, PORTC0);
		set_bit(PORTC, PORTC1);
		clr_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
		
		case 4:
		clr_bit(PORTC, PORTC0);
		clr_bit(PORTC, PORTC1);
		set_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
		
		case 5:
		set_bit(PORTC, PORTC0);
		clr_bit(PORTC, PORTC1);
		set_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
		
		case 6:
		clr_bit(PORTC, PORTC0);
		set_bit(PORTC, PORTC1);
		set_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
		
		case 7:
		set_bit(PORTC, PORTC0);
		set_bit(PORTC, PORTC1);
		set_bit(PORTC, PORTC2);
		clr_bit(PORTC, PORTC3);
		break;
	}
}
