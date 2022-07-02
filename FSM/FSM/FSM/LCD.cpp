/*
 * lcd.cpp
 *
 * Created: 30/06/2022 12:04:34
 *  Author: Faculdade
 */ 

#include "LCD.h"

LCD::LCD()
{
	set_bit(DDRD, PORTD4);
	set_bit(DDRD, PORTD5);
	set_bit(DDRD, PORTD6);
	set_bit(DDRD, PORTD7);
	
	set_bit(DDRB, PORTB0);
	set_bit(DDRB, PORTB1);
}

LCD::~LCD()
{
	//dtor
}

void LCD::cmd_LCD(unsigned char c, char cd)
{
	if(cd==0)
		clr_bit(CONTR_LCD,RS);
	else
		set_bit(CONTR_LCD,RS);

	//primeiro nibble de dados - 4 MSB
	#if (nibble_dados)								//compila codigo para os pinos de dados do LCD nos 4 MSB do PORT
		DADOS_LCD = (DADOS_LCD & 0x0F)|(0xF0 & c);
	#else											//compila codigo para os pinos de dados do LCD nos 4 LSB do PORT
		DADOS_LCD = (DADOS_LCD & 0xF0)|(c>>4);
	#endif
	
	pulso_enable();

	//segundo nibble de dados - 4 LSB
	#if (nibble_dados)								//compila codigo para os pinos de dados do LCD nos 4 MSB do PORT
		DADOS_LCD = (DADOS_LCD & 0x0F) | (0xF0 & (c<<4));
	#else											//compila codigo para os pinos de dados do LCD nos 4 LSB do PORT
		DADOS_LCD = (DADOS_LCD & 0xF0) | (0x0F & c);
	#endif
	
	pulso_enable();
	
	if((cd==0) && (c<4))				//se for instrução de retorno ou limpeza espera LCD estar pronto
		_delay_ms(2);
}

void LCD::init()
{
	clr_bit(CONTR_LCD,RS);	//RS em zero indicando que o dado para o LCD será uma instrução	
	clr_bit(CONTR_LCD,E);	//pino de habilitação em zero
	
	_delay_ms(20);	 		//tempo para estabilizar a tensão do LCD, após VCC ultrapassar 4.5 V (na prática pode
							//ser maior). 
	//interface de 8 bits						
	#if (nibble_dados)
		DADOS_LCD = (DADOS_LCD & 0x0F) | 0x30;		
	#else		
		DADOS_LCD = (DADOS_LCD & 0xF0) | 0x03;		
	#endif						
							
	pulso_enable();			//habilitação respeitando os tempos de resposta do LCD
	_delay_ms(5);		
	pulso_enable();
	_delay_us(200);
	pulso_enable();	/*até aqui ainda é uma interface de 8 bits.
					Muitos programadores desprezam os comandos acima, respeitando apenas o tempo de
					estabilização da tensão (geralmente funciona). Se o LCD não for inicializado primeiro no 
					modo de 8 bits, haverá problemas se o microcontrolador for inicializado e o display já o tiver sido.*/
	
	//interface de 4 bits, deve ser enviado duas vezes (a outra está abaixo)
	#if (nibble_dados) 
		DADOS_LCD = (DADOS_LCD & 0x0F) | 0x20;		
	#else		
		DADOS_LCD = (DADOS_LCD & 0xF0) | 0x02;
	#endif
	
	pulso_enable();		
   	cmd_LCD(0x28,0); 		//interface de 4 bits 2 linhas (aqui se habilita as 2 linhas) 
							//são enviados os 2 nibbles (0x2 e 0x8)
   	cmd_LCD(0x08,0);		//desliga o display
   	cmd_LCD(0x01,0);		//limpa todo o display
   	cmd_LCD(0x0C,0);		//mensagem aparente cursor inativo não piscando   
   	cmd_LCD(0x80,0);		//inicializa cursor na primeira posição a esquerda - 1a linha
}

void LCD::write(char *c)
{
	for (; *c!=0;c++) cmd_LCD(*c,1);
}

void LCD::write_in_flash(const char *c)
{
	for (;pgm_read_byte(&(*c))!=0;c++) cmd_LCD(pgm_read_byte(&(*c)),1);
}

void LCD::ident_num(unsigned int valor, unsigned char *disp)
{
	unsigned char n;

	for(n=0; n<tam_vetor; n++)
		disp[n] = 0 + conv_ascii;		//limpa vetor para armazenagem do digitos

	do
	{
		*disp = (valor%10) + conv_ascii;	//pega o resto da divisao por 10
		valor /=10;						//pega o inteiro da divisão por 10
		disp++;

	}while (valor!=0);
}

void LCD::setCursor_inLine(unsigned char line)
{
	switch(line){
		case 1:
			cmd_LCD(0x80, 0);
			break;
		case 2:
			cmd_LCD(0xC0, 0);
			break;
	}	
}

void LCD::clear()
{
	cmd_LCD(LCD_CMD_CLEAR_DISPLAY, 0);
	setCursor_inLine(1);
}