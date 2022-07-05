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
	
	coffee_valve.attachToPort(PORTB2);
	milk_valve.attachToPort(PORTB4);
	chocolate_valve.attachToPort(PORTB5);
	
	motor.setup();
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
			if(this->coffee >= 1){
				this->coffee--;
				this->money += 1.50;
				
				if(this->debug_mode){
					serial.transmit("Bebida 1 Selecionada!");
					serial.transmitChar(NEWLINE);
				}
				
				this->drink_available = true;
			}
			else{
				this->drink_available = false;
				this->show_avaliable_drinks();	
			}
				
			break;
			
		case 2:
			if(this->coffee >= 0.50 && this->milk > 0.50){
				this->coffee -= 0.50;
				this->milk -= 0.50;
				this->money += 2.00;
				
				if(this->debug_mode){
					serial.transmit("Bebida 2 Selecionada!");
					serial.transmitChar(NEWLINE);
				}
				
				this->drink_available = true;
			}
			else{
				this->available_drinks = false;
				this->show_avaliable_drinks();
			}
			break;
			
		case 3:
			if(this->coffee >= 0.33 && this->milk >= 0.33 && this->chocolate >= 0.33){
				this->coffee -= 0.33;
				this->milk -= 0.33;
				this->chocolate -= 0.33;
				this->money += 2.75;
				
				if(this->debug_mode){
					serial.transmit("Bebida 3 Selecionada!");
					serial.transmitChar(NEWLINE);
				}
				
				this->drink_available = true;
			}
			else{
				this->drink_available = false;
				this->show_avaliable_drinks();
			}
			break;
			
		case 4:
			if(this->milk >= 1){
				this->milk--;
				this->money += 1.00;
				
				if(this->debug_mode){
					serial.transmit("Bebida 4 Selecionada!");
					serial.transmitChar(NEWLINE);
				}
				
				this->drink_available = true;
			}
			else{
				this->drink_available = false;
				this->show_avaliable_drinks();
			}
			break;
			
		case 5:
			if(this->milk >= 0.50 && this->chocolate >= 0.50){
				this->milk -= 0.50;
				this->chocolate -= 0.50;
				this->money += 1.80;
				
				if(this->debug_mode){
					serial.transmit("Bebida 5 Selecionada!");
					serial.transmitChar(NEWLINE);
				}
				
				this->drink_available = true;
			}
			else{
				this->drink_available = false;
				this->show_avaliable_drinks();
			}
			break;
			
		case 6:
			if(this->chocolate >= 1){
				this->chocolate--;
				this->money += 1.50;
				
				if(this->debug_mode){
					serial.transmit("Bebida 6 Selecionada!");
					serial.transmitChar(NEWLINE);
				}
				
				this->drink_available = true;
			}
			else{
				this->drink_available = false;
				this->show_avaliable_drinks();
			}
			break;
			
		case 7:
			if(this->coffee >= 0.50 && this->chocolate >= 0.50){
				this->coffee -= 0.50;
				this->chocolate -= 0.50;
				this->money += 2.10;
				
				if(this->debug_mode){
					serial.transmit("Bebida 7 Selecionada!");
					serial.transmitChar(NEWLINE);
				}
				
				this->drink_available = true;
			}
			else{
				this->drink_available = false;
				this->show_avaliable_drinks();
			}
			break;
	}
}

void DrinkMaker::show_avaliable_drinks()
{
	char buf[10];
	this->available_drinks = 0b00000000; //Reset available drinks flag
	
	if(this->coffee >= 1)
		this->available_drinks |= 0b00000001;	
	if(this->coffee >= 0.50 && this->milk >= 0.50)
		this->available_drinks |= 0b00000010;
	if(this->coffee >= 0.33 && this->milk >= 0.33 && this->chocolate >= 0.33)
		this->available_drinks |= 0b00000100;
	if(this->milk >= 1)
		this->available_drinks |= 0b00001000;
	if(this->milk >= 0.50 && this->chocolate >= 0.50)
		this->available_drinks |= 0b00010000;
	if(this->chocolate >= 1)
		this->available_drinks |= 0b00100000;
	if(this->coffee >= 0.50 && this->chocolate >= 0.50)
		this->available_drinks |= 0b01000000;
	
	if(this->debug_mode){
		serial.transmitChar(NEWLINE);
		serial.transmit("Bebida Indisponivel");
		serial.transmitChar(NEWLINE);
		
		serial.transmit("Bebidas Disponiveis:");
		serial.transmitChar(NEWLINE);
	}
	
	lcd.clear();
	lcd.write("Bebida");
	lcd.setCursor_inLine(2);
	lcd.write("Indisponivel");
	_delay_ms(500);
	
	lcd.clear();
	lcd.write("Disponiveis:");
	lcd.setCursor_inLine(2);
	//Prints all avaliable drinks by comparing the "avaliable_drinks" byte with each respective drink bit.
	for(int i = 0; i < 7; i++){
		if((this->available_drinks & (1<<i)) > 0){
			sprintf(buf, "%d", i+1);
			
			if(debug_mode){
				serial.transmit(buf);
				serial.transmitChar(NEWLINE);
			}
			
			lcd.write(buf);
			lcd.write(" ");
		}
	}
	_delay_ms(1000);
}

void DrinkMaker::show_remaining_ingredients()
{
	char buf[10];
	
	serial.transmitChar(NEWLINE);
	serial.transmit("Cafe restante: ");
	int coffeeInt = int(this->coffee);
	float coffeeFrac = this->coffee - coffeeInt;
	int coffeeIntFrac = trunc(coffeeFrac*100);
	sprintf(buf, "%d.%02d", coffeeInt, coffeeIntFrac);
	serial.transmit(buf);
	
	serial.transmitChar(NEWLINE);
	serial.transmit("Leite restante: ");
	int milkInt = int(this->milk);
	float milkFrac = this->milk - milkInt;
	int milkIntFrac = trunc(milkFrac*100);
	sprintf(buf, "%d.%02d", milkInt, milkIntFrac);
	serial.transmit(buf);
	
	serial.transmitChar(NEWLINE);
	serial.transmit("Chocolate restante: ");
	int chocolateInt = int(this->chocolate);
	float chocolateFrac = this->chocolate - chocolateInt;
	int chocolateIntFrac = trunc(chocolateFrac*100);
	sprintf(buf, "%d.%02d", chocolateInt, chocolateIntFrac);
	serial.transmit(buf);
	serial.transmitChar(NEWLINE);
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

void DrinkMaker::millCoffee()
{
	lcd.write("Moendo...");
	lcd.setCursor_inLine(2);
	lcd.write("Aguarde");
	motor.turnOn();
}

void DrinkMaker::stopMilling()
{
	motor.turnOff();
}

void DrinkMaker::openValves(volatile unsigned char *drink_index)
{
	lcd.write("Servindo");
	switch(*drink_index){
		case 1:
		coffee_valve.turnOn();
		break;
		
		case 2:
		coffee_valve.turnOn();
		milk_valve.turnOn();
		break;
		
		case 3:
		coffee_valve.turnOn();
		milk_valve.turnOn();
		chocolate_valve.turnOn();
		break;
		
		case 4:
		milk_valve.turnOn();
		break;
		
		case 5:
		milk_valve.turnOn();
		chocolate_valve.turnOn();
		break;
		
		case 6:
		chocolate_valve.turnOn();
		break;
		
		case 7:
		coffee_valve.turnOn();
		chocolate_valve.turnOn();
		break;
	}
}

void DrinkMaker::closeValves()
{
	coffee_valve.turnOff();
	milk_valve.turnOff();
	chocolate_valve.turnOff();
}

bool DrinkMaker::drinkHasCoffee(volatile unsigned char *drink_index)
{
	switch(*drink_index){
		case 1:
		return true;
		break;
		
		case 2:
		return true;
		break;
		
		case 3:
		return true;
		break;
		
		case 4:
		return false;
		break;
		
		case 5:
		return false;
		break;
		
		case 6:
		return false;
		break;
		
		case 7:
		return true;
		break;
	}
}

void DrinkMaker::updateLCD(volatile unsigned char *drink_index)
{
	switch(*drink_index){
		case 1:
		lcd.clear();
		lcd.write("Cafe Puro");
		lcd.setCursor_inLine(2);
		lcd.write("R$ 1.50");
		break;
		
		case 2:
		lcd.clear();
		lcd.write("Macchiato");
		lcd.setCursor_inLine(2);
		lcd.write("R$ 2.00");
		break;
		
		case 3:
		lcd.clear();
		lcd.write("Mocha");
		lcd.setCursor_inLine(2);
		lcd.write("R$ 2.75");
		break;
		
		case 4:
		lcd.clear();
		lcd.write("Leite Puro");
		lcd.setCursor_inLine(2);
		lcd.write("R$ 1.00");
		break;
		
		case 5:
		lcd.clear();
		lcd.write("Achocolatado");
		lcd.setCursor_inLine(2);
		lcd.write("R$ 1.80");
		break;
		
		case 6:
		lcd.clear();
		lcd.write("Chocolate");
		lcd.setCursor_inLine(2);
		lcd.write("R$ 1.50");
		break;
		
		case 7:
		lcd.clear();
		lcd.write("Chocafe");
		lcd.setCursor_inLine(2);
		lcd.write("R$ 2.10");
		break;
	}
}