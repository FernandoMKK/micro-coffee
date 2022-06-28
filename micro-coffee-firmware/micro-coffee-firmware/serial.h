/*
 * serial.h
 *
 * Created: 27/06/2022 17:36:54
 *  Author: Fernando Kirchner
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define NEWLINE 13

#define set_bit(adress,bit) (adress|=(1<<bit))
#define clr_bit(adress,bit) (adress&=~(1<<bit))
#define tst_bit(adress,bit) (adress&(1<<bit))

class Serial
{
	public:
		Serial(unsigned int baud_rate, unsigned char stop_bit, unsigned char byte_size, unsigned char parity);
		~Serial();
		void transmit(char *data);
		void transmitChar(char data);
		void show_menu();
		void show_profit(volatile float money);
		
		void setBaudRate(unsigned int baud_rate);
		void setStopBit(unsigned char stop_bit);
		void setByteSize(unsigned char byte_size);
		void setParity(unsigned char parity);
		
	private:
		unsigned int bd;
		unsigned char sb;
		unsigned char bs;
		unsigned char pr;
};

#endif /* SERIAL_H_ */