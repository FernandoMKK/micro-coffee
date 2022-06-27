/*
 * Valves.cpp
 *
 * Created: 27/06/2022 16:59:02
 * Author : Fernando Kirchner
 *
 * Description: This code opens and closes electronics valves by pressing buttons. 
 */ 

#include "valve.h"


int main(void)
{
    DDRD = 0x00;
	
	Valve valve1, valve2, valve3;
	valve1.attachToPort(PORTB2);
	valve2.attachToPort(PORTB4);
	valve3.attachToPort(PORTB5);
	
    while (1) 
    {
		if(tst_bit(PIND, PORTD2) > 0)
			valve1.turnOn();
		if(tst_bit(PIND, PORTD3) > 0)
			valve2.turnOn();
		if(tst_bit(PIND, PORTD4) > 0)
			valve3.turnOn();
		if(tst_bit(PIND, PORTD5) > 0){
			valve1.turnOff();
			valve2.turnOff();
			valve3.turnOff();
		}
    }
}

