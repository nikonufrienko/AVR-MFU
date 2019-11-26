/*
 * lcd_lib.h
 *
 * Created: 19.08.2019 18:16:36
 *  Author: Admin
 */ 
#include "nokia5110_chars.h"
#define byte uint8_t
#ifndef LCD_LIB_H_
#define LCD_LIB_H_
#define PORT_1 PORTC
#define cmode_off() PORT_1|=1<<DC //режим передачи графики
#define cmode_on() PORT_1&=~(1<<DC) //режим передачи команд
#define stop() PORT_1|=1<<CE
#define start() PORT_1&=~(1<<CE)
#define RESET PC4
#define DIN PC1
#define DC PC2
#define CE PC3
#define CLK PC0
uint8_t x_stream=0,y_stream =0;
void LCDlibInit()
{
	DDRC=(1<<DIN)|(1<<DC)|(1<<CE)|(1<<CLK)|(1<<RESET);
	PORTC=0x00;
}
void input(byte inp)//передача байта через spi на дисплей
{
	start();
	byte buff=0x80;
	while(buff!=0x00)
	{
		
		PORT_1&=~(1<<CLK);
		if((buff&inp)!=0x00)
		{
			PORT_1|=1<<DIN;
			
		}
		else
		{
			PORT_1&=~(1<<DIN);
		}
		
		//PORTA&=0b11110111;
		PORT_1|=1<<CLK;
		buff>>=1;
	}
	stop();
}
void init_displ()
{
	//_____RESET_____
	PORT_1&=~(1<<RESET);
	PORT_1|=1<<RESET;
	//________________
	/*
	cmode_on();
	input(0x21);
	input(0x80 + 34);
	input(0x04);
	input(0x13);
	input(0x20);
	input(0x0c);
	*/
	input(0x21);
	input(0xc8);
	input(0x06);
	input(0x13);
	//input(0x80 + 34);
	input(0x20);
	input(0x0c);
	cmode_off();
}
void setpos(byte y, byte x)
{
	cmode_on();
	input(0x40 | (y&7));
	input(0x80 | x);
	cmode_off();
}
void clear()
{
	setpos(0,0);
	for (uint8_t y = 0; y < 6; y++)
	for (uint8_t x = 0; x < 84; x++)
	input(0);
	setpos(0,0);
}
void draw_line(uint8_t x, uint8_t y, uint8_t len,const byte *line, byte inver)
{
	
	for(uint8_t i=0;i<len;i++)
	{
		setpos(y,x+i);
		if(inver) input(~line[i]);
		else input(line[i]);
	}
	
}
void lcd_printChar(char code)
{
	
	for(int i =0;i<5;i++)
		input(pgm_read_byte(&CHARSET[code-32][i]));
}
void lcd_print(char *line,uint8_t x, uint8_t y)
{
	setpos(y,x);
	int i =0;
	while(line[i]!='\0')
	{
		if(line[i]!='\n')
			lcd_printChar(line[i]);
		input(0x00);
		i++;
	}
}

void lcd_stream_write(char *line)
{
	
	int i = 0;
	while(line[i]!='\0')
	{
		if((x_stream==0) && (y_stream==0))
			clear();
		setpos(y_stream,x_stream);
		if(line[i]!='\n')
		{
			lcd_printChar(line[i]);
			x_stream+=6;
		}
		input(0x00);
				
		if(((x_stream>80)||(line[i]=='\n'))&&(y_stream<5))//в одну строку помещаеться по 14 символов
		{
			y_stream++;
			x_stream=0;
		}
		else if ((y_stream==5)&&((x_stream>=80)||(line[i]=='\n')))
		{
			x_stream=0;
			y_stream=0;
		}
		i++;
	}	
}
#endif /* LCD_LIB_H_ */