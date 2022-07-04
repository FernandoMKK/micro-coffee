/*
 * lcd.h
 *
 * Created: 30/06/2022 11:58:19
 *  Author: Faculdade
 */ 


#ifndef LCD_H_
#define LCD_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

//uso de funções para salvar dados na memória de programa
#include <avr/pgmspace.h>

//Definicoes de macros para o trabalho com bits
#define set_bit(y,bit) (y|=(1<<bit))
#define clr_bit(y,bit) (y&=~(1<<bit)) 
#define cpl_bit(y,bit) (y^=(1<<bit)) 
#define tst_bit(y,bit) (y&(1<<bit)) 

#define DADOS_LCD    	PORTD  	//4 bits de dados do LCD no PORTD
#define nibble_dados	1		//0 para via de dados do LCD nos 4 LSBs do PORT empregado (Px0-D4, Px1-D5, Px2-D6, Px3-D7) 
								//1 para via de dados do LCD nos 4 MSBs do PORT empregado (Px4-D4, Px5-D5, Px6-D6, Px7-D7)
#define CONTR_LCD 		PORTB  	//PORT com os pinos de controle do LCD (pino R/W em 0).
#define E    			PB1     //pino de habilitacao do LCD (enable)
#define RS   			PB0     //pino para informar se o dado eh uma instrucao ou caractere
#define BACKLITE		PB2

#define tam_vetor	5	//numero de digitos individuais para a conversao por ident_num()
#define conv_ascii	48	//48 se ident_num() deve retornar um numero no formato ASCII (0 para formato normal)

//sinal de habilitação para o LCD
#define pulso_enable() 	_delay_us(1); set_bit(CONTR_LCD,E); _delay_us(1); clr_bit(CONTR_LCD,E); _delay_us(45)

//prototipo das funcoes

#define LCD_CMD_CLEAR_DISPLAY	          0x01
#define LCD_CMD_CURSOR_HOME		          0x02

// Cursor and display shift
#define LCD_CMD_ENTRY_MODE_DEC_NOSHIFT     0x04
#define LCD_CMD_ENTRY_MODE_DEC_SHIFT       0x05
#define LCD_CMD_ENTRY_MODE_INC_NOSHIFT     0x06
#define LCD_CMD_ENTRY_MODE_INC_SHIFT       0x07

// Display control
#define LCD_CMD_DISPLAY_OFF                0x08
#define LCD_CMD_DISPLAY_NO_CURSOR          0x0c
#define LCD_CMD_DISPLAY_CURSOR_NO_BLINK    0x0E
#define LCD_CMD_DISPLAY_CURSOR_BLINK       0x0F

// Cursor and display shift
#define LCD_CMD_DISPLAY_NOSHIFT_CUR_LEFT   0x10
#define LCD_CMD_DISPLAY_NOSHIFT_CUR_RIGHT  0x14
#define LCD_CMD_DISPLAY_SHIFT_LEFT         0x18
#define LCD_CMD_DISPLAY_SHIFT_RIGHT        0x1C

// Function set
#define LCD_CMD_4BIT_1ROW_5X7              0x20
#define LCD_CMD_4BIT_1ROW_5X10             0x24
#define LCD_CMD_4BIT_2ROW_5X7              0x28
#define LCD_CMD_8BIT_1ROW_5X7              0x30
#define LCD_CMD_8BIT_1ROW_5X10             0x34
#define LCD_CMD_8BIT_2ROW_5X7              0x38

class LCD
{
	public:
		LCD();
		~LCD();
	
		void cmd_LCD(unsigned char c, char cd);
		void init();
		void write(char *c);
		void write_in_flash(const char *c);
		void ident_num(unsigned int valor, unsigned char *disp);
		
		void setCursor_inLine(unsigned char line);
		void clear();
};

#endif /* LCD_H_ */