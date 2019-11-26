/*
 * ALL.c
 *
 * Created: 22.10.2019 15:35:35
 * Author : Admin
 */ 
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <avr/io.h>
#define KEYS KEYS
#define UP_BUT PD3
#define DOWN_BUT PD7
#define LEFT_BUT PB0
#define RIGHT_BUT PD6
#define SELECT1_BUT PD2
#define SELECT2_BUT PD5
#define SPK PB1
#define SPK_PORT PORTB
#include <util/delay.h>
#include "lcd_lib.h"
#include "snake.h"
#include "terminal.h"
#include "music.h"
void keyboard_init()
{
	DDRD&=~((1<<DOWN_BUT)|(1<<RIGHT_BUT)|(1<<UP_BUT)|(1<<SELECT1_BUT)|(1<<SELECT2_BUT));
	DDRB&=~(1<<LEFT_BUT);
	DDRB|=(1<<SPK);
	PORTD=(1<<DOWN_BUT)|(1<<RIGHT_BUT)|(1<<UP_BUT)|(1<<SELECT1_BUT)|(1<<SELECT2_BUT);
	PORTB=(1<<LEFT_BUT);
}

const unsigned char music_on [] = {
	0x7E, 0x4A, 0x34, 0x00, 0x78, 0x20, 0x58, 0x00, 0x70, 0x08, 0x78, 0x00, 0x78, 0x20, 0x78, 0x48,
	0x78, 0x00, 0x18, 0x20, 0x78, 0x00, 0x78, 0x20, 0x10, 0x78, 0x00, 0x08, 0x78, 0x08, 0x00, 0x78,
	0x50, 0x20, 0x00, 0x00, 0x00, 0x7C, 0x08, 0x30, 0x08, 0x7C, 0x00, 0x58, 0x20, 0x18, 0x00, 0x44,
	0x54, 0x28, 0x00, 0x78, 0x50, 0x20, 0x00, 0x78, 0x00, 0x78, 0x10, 0x68, 0x00, 0x58, 0x20, 0x18
};
const unsigned char games [] = {
	0x00,0x7E, 0x20, 0x10, 0x7E, 0x00, 0x7C, 0x04, 0x04, 0x00, 0x7C, 0x14, 0x08, 0x00, 0x7C, 0x50, 0x20,
	0x00, 0x7C,0x00
};
const unsigned char terminal [] = {
	0x00, 0x02, 0x7E, 0x02, 0x00, 0x38, 0x54, 0x48, 0x00, 0x7C, 0x14, 0x08, 0x00, 0x7C, 0x08, 0x10,
	0x08, 0x7C, 0x00, 0x7C, 0x20, 0x10, 0x7C, 0x00, 0x7C, 0x20, 0x7C, 0x00, 0x78, 0x24, 0x7C, 0x00,
	0x78, 0x04, 0x7C, 0x00,
};/*
ISR(USART_RX_vect)
{
	print("r");
	sm[0]=UDR0;
	lcd_stream_write(sm);
}*/

int main(void)
{	LCDlibInit();
	init_displ();
	clear();
	keyboard_init();
	uint8_t IsPlaying=0;
	int buff=0;
	sei();
    while (1) 
    {
		
		draw_line(20,2,36,terminal,buff==0);
		draw_line(20,3,20,games,buff==1);
		draw_line(20,4,64,music_on,buff==2);
		if((!(PIND&(1<<DOWN_BUT)))&&(buff!=2))
		{
			buff++;
			while(!(PIND&(1<<DOWN_BUT)));
			//_delay_ms(100);
		}
		else if((!(PIND&(1<<UP_BUT)))&&(buff!=0))
		{
			buff--;
			while(!(PIND&(1<<UP_BUT)));
			//_delay_ms(100);
		}
		if(!(PIND&(1<<SELECT1_BUT)))
		{
			switch(buff)
			{
				case 0:term_settings();
					break;
				case 1: snakeStart();			
					break;
				case 2:
					if(IsPlaying)
					{
						cli();
						TIMSK1&=~(1<<OCIE1A);
						//clear();
						//_delay_ms(1000);
						IsPlaying=0;
					} 
					else
					{
						sei();
						init_t1();
						IsPlaying=1;
					}
					break;
			}
			
			while(!(PIND&(1<<SELECT1_BUT)));
			_delay_ms(10);
			clear();
		}
				
    }
}

