/*
 * serial.cpp
 *
 * Created: 27/06/2022 17:42:16
 *  Author: Fernando Kirchner
 */ 

#include "serial.h"

Serial::Serial()
{
	setBaudRate(9600);
	setStopBit(1);
	setByteSize(8);
	setParity('N');
	
	/*-- Setting direction registers --*/
	DDRD |= 0b00000010;
	
	/* -- Setting serial registers --*/
	UCSR0B = (1<<TXEN0) | (1<<RXEN0); //Enables the TX and RX
	UCSR0B |= (1<<RXCIE0); //Enables the RX interrupt
	
	//Sets assync mode
	clr_bit(UCSR0C, UMSEL01);
	clr_bit(UCSR0C, UMSEL00);
	
	sei(); //Enables global interrupt
}

Serial::~Serial()
{
	//dtor
}

void Serial::setBaudRate(unsigned int baud_rate)
{
	this->bd = baud_rate;
	unsigned int baud_prescale = (((F_CPU / (this->bd*16UL)))-1);
	
	UBRR0H = (baud_prescale >> 8);
	UBRR0L = baud_prescale;
}

void Serial::setStopBit(unsigned char stop_bit)
{
	this->sb = stop_bit;
	
	switch(this->sb){
		case 1:
			clr_bit(UCSR0C, USBS0);
			break;
		
		case 2:
			set_bit(UCSR0C, USBS0);
			break;
			
		default:
			clr_bit(UCSR0C, USBS0);
			break;
	}
		
}

void Serial::setByteSize(unsigned char byte_size)
{
	this->bs = byte_size;
	
	switch(this->bs){
		case 5:
			clr_bit(UCSR0C, UCSZ02);
			clr_bit(UCSR0C, UCSZ01);
			clr_bit(UCSR0C, UCSZ00);
			break;
			
		case 6:
			clr_bit(UCSR0C, UCSZ02);
			clr_bit(UCSR0C, UCSZ01);
			set_bit(UCSR0C, UCSZ00);
			break;
			
		case 7:
			clr_bit(UCSR0C, UCSZ02);
			set_bit(UCSR0C, UCSZ01);
			clr_bit(UCSR0C, UCSZ00);
			break;
			
		case 8:
			clr_bit(UCSR0C, UCSZ02);
			set_bit(UCSR0C, UCSZ01);
			set_bit(UCSR0C, UCSZ00);
			break;
			
		case 9:
			set_bit(UCSR0C, UCSZ02);
			set_bit(UCSR0C, UCSZ01);
			set_bit(UCSR0C, UCSZ00);
			break;
			
		default:
			clr_bit(UCSR0C, UCSZ02);
			set_bit(UCSR0C, UCSZ01);
			set_bit(UCSR0C, UCSZ00);
			break;
	}
}

void Serial::setParity(unsigned char parity)
{
	this->pr = parity;
	
	switch(this->pr){
		case 'N':
			clr_bit(UCSR0C, UPM01);
			clr_bit(UCSR0C, UPM00);
			break;
		
		case 'E':
			set_bit(UCSR0C, UPM01);
			set_bit(UCSR0C, UPM00);
			break;
		
		case 'O':
			set_bit(UCSR0C, UPM01);
			clr_bit(UCSR0C, UPM00);
			break;
		default:
			clr_bit(UCSR0C, UPM01);
			clr_bit(UCSR0C, UPM00);
			break;
	}
}

void Serial::transmit(char *data)
{
	while(*data != '\0'){
		while(!(UCSR0A & (1<<UDRE0))); //Waits for empty buffer to transmit
		
		//Put data into buffer and send data
		UDR0 = *data;
		data++;
	}
}

void Serial::transmitChar(char data)
{
	while(!(UCSR0A & (1<<UDRE0))); //Waits for empty buffer to transmit
	UDR0 = data; //Put data into buffer and send data
}

void Serial::show_menu()
{
	this->transmit("0 - Mostrar Lucro");
	this->transmitChar(NEWLINE);
	
	this->transmit("1 - Debug");
	this->transmitChar(NEWLINE);
	
	this->transmit("2 - Sair Debug");
	this->transmitChar(NEWLINE);
	
	this->transmit("3 - Mostrar Ingredientes Restantes");
	this->transmitChar(NEWLINE);
	this->transmitChar(NEWLINE);
}