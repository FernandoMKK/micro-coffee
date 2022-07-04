/*
 * motor.h
 *
 * Created: 27/06/2022 16:11:39
 *  Author: Fernando Kirchner
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(adress,bit) (adress|=(1<<bit))
#define clr_bit(adress,bit) (adress&=~(1<<bit))
#define tst_bit(adress,bit) (adress&(1<<bit))

class Motor
{
	public:
		Motor();
		~Motor();
		
		void turnOn();
		void turnOff();
		void setup();
};


#endif /* MOTOR_H_ */