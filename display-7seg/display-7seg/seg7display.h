/*
 * seg7display.h
 *
 * Created: 25/06/2022 16:36:36
 *  Author: Fernando Kirchner
 */ 


#ifndef SEG7DISPLAY_H_
#define SEG7DISPLAY_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define set_bit(adress,bit) (adress|=(1<<bit))
#define clr_bit(adress,bit) (adress&=~(1<<bit))
#define tst_bit(adress,bit) (adress&(1<<bit))

void update7segDisplay(unsigned char *num);

#endif /* SEG7DISPLAY_H_ */