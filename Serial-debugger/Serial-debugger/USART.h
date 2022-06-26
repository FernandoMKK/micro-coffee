/*
 * USART.h
 *
 * Created: 26/06/2022 09:08:36
 *  Author: Fernando Kirchner
 */ 


#ifndef USART_H_
#define USART_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL)))-1)
#define NEWLINE 13

void USART_Transmit(char *data);
void USART_TransmitChar(char data);
void show_menu();
void show_profit();

#endif /* USART_H_ */