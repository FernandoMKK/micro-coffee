/*
 * USART.c
 *
 * Created: 26/06/2022 09:09:59
 *  Author: Fernando Kirchner
 */ 

#include "USART.h"

void USART_Transmit(char *data)
{
	while(*data != '\0'){
		while(!(UCSR0A & (1<<UDRE0))); //Waits for empty buffer to transmit
		
		//Put data into buffer and send data
		UDR0 = *data;
		data++;
	}
}

void USART_TransmitChar(char data)
{
	while(!(UCSR0A & (1<<UDRE0))); //Waits for empty buffer to transmit
	UDR0 = data; //Put data into buffer and send data
}

void show_menu()
{
	USART_Transmit("0 - Mostrar Lucro");
	USART_TransmitChar(NEWLINE);
	
	USART_Transmit("1 - Debug");
	USART_TransmitChar(NEWLINE);
	
	USART_Transmit("2 - Sair Debug");
	USART_TransmitChar(NEWLINE);
}

void show_profit()
{
	USART_Transmit("Peca 1: ");
	USART_Transmit("xx");
	USART_TransmitChar(NEWLINE);
	USART_Transmit("Peca 2: ");
	USART_Transmit("xx");
	USART_TransmitChar(NEWLINE);
	USART_Transmit("Peca 3: ");
	USART_Transmit("xx");
	USART_TransmitChar(NEWLINE);
}