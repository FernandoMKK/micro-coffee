/*
 * drinkMaker.cpp
 *
 * Created: 29/06/2022 17:14:32
 *  Author: Faculdade
 */ 

#include "drinkMaker.h"


DrinkMaker::DrinkMaker()
{
	this->money = 0;
	this->coffee = 0;
	this->milk = 0;
	this->chocolate = 0;
	this->available_drinks = 0;
	//ctor
}

DrinkMaker::~DrinkMaker()
{
	//dtor
}

void DrinkMaker::refuel()
{
	this->coffee = 10;
	this->milk = 5;
	this->chocolate = 7;
	
	this->money = 0;
	
	serial.transmit("Maquina reabastecida");
	serial.transmitChar(NEWLINE);
	
}

void DrinkMaker::makeDrink(volatile unsigned char *drink_index)
{
	switch(*drink_index){
		case 1:
			if(this->coffee > 0){
				this->coffee--;
				this->money += 1.50;
				
				serial.transmit("Bebida 1 Selecionada!");
				serial.transmitChar(NEWLINE);
			}
			else
				this->show_avaliable_drinks();
			break;
			
		case 2:
			if(this->coffee > 0 && this->milk > 0){
				this->coffee--;
				this->milk--;
				this->money += 2.00;
				
				serial.transmit("Bebida 2 Selecionada!");
				serial.transmitChar(NEWLINE);
			}
			else
				this->show_avaliable_drinks();
			break;
			
		case 3:
			if(this->coffee > 0 && this->milk > 0 && this->chocolate > 0){
				this->coffee--;
				this->milk--;
				this->chocolate--;
				this->money += 2.75;
				
				serial.transmit("Bebida 3 Selecionada!");
				serial.transmitChar(NEWLINE);
			}
			else
				this->show_avaliable_drinks();
			break;
			
		case 4:
			if(this->milk > 0){
				this->milk--;
				this->money += 1.00;
				
				serial.transmit("Bebida 4 Selecionada!");
				serial.transmitChar(NEWLINE);
			}
			else
				this->show_avaliable_drinks();
			break;
			
		case 5:
			if(this->milk > 0 && this->chocolate > 0){
				this->milk--;
				this->chocolate--;
				this->money += 1.80;
				
				serial.transmit("Bebida 5 Selecionada!");
				serial.transmitChar(NEWLINE);
			}
			else
				this->show_avaliable_drinks();
			break;
			
		case 6:
			if(this->chocolate > 0){
				this->chocolate--;
				this->money += 1.50;
				
				serial.transmit("Bebida 6 Selecionada!");
				serial.transmitChar(NEWLINE);
			}
			else
				this->show_avaliable_drinks();
			break;
			
		case 7:
			if(this->coffee > 0 && this->chocolate > 0){
				this->coffee--;
				this->chocolate--;
				this->money += 2.10;
				
				serial.transmit("Bebida 7 Selecionada!");
				serial.transmitChar(NEWLINE);
			}
			else
				this->show_avaliable_drinks();
			break;
	}
}

void DrinkMaker::show_avaliable_drinks()
{
	char buf[1];
	this->available_drinks = 0b00000000; //Reset available drinks flag
	
	if(this->coffee > 0)
		this->available_drinks |= 0b00000001;	
	if(this->coffee > 0 && this->milk > 0)
		this->available_drinks |= 0b00000010;
	if(this->coffee > 0 && this->milk > 0 && this->chocolate > 0)
		this->available_drinks |= 0b00000100;
	if(this->milk > 0)
		this->available_drinks |= 0b00001000;
	if(this->milk > 0 && this->chocolate > 0)
		this->available_drinks |= 0b00010000;
	if(this->chocolate > 0)
		this->available_drinks |= 0b00100000;
	if(this->coffee > 0 && this->chocolate > 0)
		this->available_drinks |= 0b01000000;
	
	serial.transmit("Bebida Indisponivel");
	serial.transmitChar(NEWLINE);
	
	serial.transmit("Bebidas Disponiveis:");
	serial.transmitChar(NEWLINE);
	
	//Prints all avaliable drinks by comparing the "avaliable_drinks" byte with each respective drink bit.
	for(int i = 0; i < 7; i++){
		if((this->available_drinks & (1<<i)) > 0){
			sprintf(buf, "%d", i+1);
			serial.transmit(buf);
			serial.transmitChar(NEWLINE);
		}
	}
}

void DrinkMaker::show_profit()
{
	int moneyInt = int(this->money);
	float moneyFrac = this->money - moneyInt;
	int moneyIntFrac = trunc(moneyFrac*100);
	
	char buf[10];
	sprintf(buf, "R$ %d.%02d", moneyInt, moneyIntFrac);
	serial.transmit("Valor Arrecadado: ");
	serial.transmit(buf);
	serial.transmitChar(NEWLINE);
}	