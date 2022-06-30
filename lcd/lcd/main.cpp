/*
 * lcd.cpp
 *
 * Created: 30/06/2022 11:57:12
 * Author : Faculdade
 */ 

#include "LCD.h"

LCD myLCD;

int main(void)
{
	myLCD.init();
	myLCD.setCursor_inLine(1);
	
	myLCD.write("Ola corno!");
	_delay_ms(500);
    /* Replace with your application code */
    while (1) 
    {
		myLCD.clear();
		myLCD.write("Linha 1...");
		_delay_ms(1000);
		myLCD.setCursor_inLine(2);
		myLCD.write("Linha 2...");
		_delay_ms(1000);
		myLCD.clear();
		myLCD.write("Apagando!");
		_delay_ms(1000);
    }
}

