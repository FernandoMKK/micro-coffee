/*
 * valve.h
 *
 * Created: 27/06/2022 17:00:09
 *  Author: Fernando Kirchner
 */ 


#ifndef VALVE_H_
#define VALVE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(adress,bit) (adress|=(1<<bit))
#define clr_bit(adress,bit) (adress&=~(1<<bit))
#define tst_bit(adress,bit) (adress&(1<<bit))

class Valve
{
	public:
		Valve();
		~Valve();
		
		void attachToPort(unsigned char port);
		void turnOn();
		void turnOff();
		
		bool getState();
	
	private:
		unsigned char port;
		bool state; 
};

#endif /* VALVE_H_ */