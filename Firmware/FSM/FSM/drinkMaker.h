/*
 * drinkMaker.h
 *
 * Created: 29/06/2022 17:09:59
 *  Author: Faculdade
 */ 


#ifndef DRINKMAKER_H_
#define DRINKMAKER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "motor.h"
#include "valve.h"
#include "LCD.h"

#define set_bit(adress,bit) (adress|=(1<<bit))
#define clr_bit(adress,bit) (adress&=~(1<<bit))
#define tst_bit(adress,bit) (adress&(1<<bit))

class DrinkMaker
{
	public:
		DrinkMaker();
		~DrinkMaker();
		
		void makeDrink(volatile unsigned char *drink_index);
		void refuel();
		void show_avaliable_drinks();
		void show_profit();
		
		void millCoffee();
		void stopMilling();
		
		void openValves(volatile unsigned char *drink_index);
		void closeValves();
		
		void show_remaining_ingredients();
		
		bool drinkHasCoffee(volatile unsigned char *drink_index);
		
		void updateLCD(volatile unsigned char *drink_index);
		bool debug_mode;
		bool drink_available;
		
	private:
		Serial serial;
		Motor motor;
		Valve coffee_valve, milk_valve, chocolate_valve;
		LCD lcd;
		
		float coffee;
		float milk;
		float chocolate;
		unsigned char available_drinks;
		
		float money;
};

#endif /* DRINKMAKER_H_ */