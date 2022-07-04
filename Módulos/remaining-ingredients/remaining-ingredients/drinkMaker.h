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
		
	private:
		Serial serial;
		unsigned char coffee;
		unsigned char milk;
		unsigned char chocolate;
		unsigned char available_drinks;
		float money;
		
};

#endif /* DRINKMAKER_H_ */