/*
 * _7seg.h
 *
 * Created: 27/06/2022 17:19:04
 *  Author: Fernando Kirchner
 */ 


#ifndef DISPLAY7SEG_H_
#define DISPLAY7SEG_H_

#define F_CPU 16000000UL
#include <avr/io.h>

#define set_bit(adress,bit) (adress|=(1<<bit))
#define clr_bit(adress,bit) (adress&=~(1<<bit))
#define tst_bit(adress,bit) (adress&(1<<bit))

class Display7seg
{
	public:
		Display7seg();
		~Display7seg();
		
		void attachToPorts(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3);
		void updateDisplay(volatile unsigned char *num);
		
	private:
		unsigned char d0, d1, d2, d3;
};


#endif /* DISPLAY7SEG_H_ */