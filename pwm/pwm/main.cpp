/*
 * pwm.cpp
 *
 * Created: 27/06/2022 16:11:18
 * Author : Fernando Kirchner
 *
 * Description: Generates a square wave of 1Khz frequency to move a DC motor. 
 *				It activates pressing a button at D2 and deactivates pressing a button at D3.
 */ 

#include "motor.h"


int main(void)
{
	Motor myMotor;
	myMotor.setup();
	myMotor.turnOff();
	
	DDRD = 0x00;
    
	while (1) 
    {
		if(tst_bit(PIND, PORTD2) > 0)
			myMotor.turnOn();
		if(tst_bit(PIND, PORTD3) > 0)
			myMotor.turnOff();
			
    }
}

