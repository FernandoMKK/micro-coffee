/*
 * seg7display.c
 *
 * Created: 25/06/2022 16:33:14
 *  Author: Fernando Kirchner
 */ 

#include "seg7display.h"

void update7segDisplay(unsigned char *num)
{
	switch(*num){
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