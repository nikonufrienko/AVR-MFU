/*
 * terminal.h
 *
 * Created: 22.10.2019 16:34:33
 *  Author: Admin
 */ 


#ifndef TERMINAL_H_
#define TERMINAL_H_
#ifndef LCD_LIB_H_
#include "lcd_lib.h"
#endif
#include "mySerial.h"

char sm[2]={' ','\0'};
uint8_t Iswork=0;
ISR(USART_RX_vect)
{
	if(Iswork)
	{
	SPK_PORT|=(1<<SPK);
	print("r");
	sm[0]=UDR0;
	lcd_stream_write(sm);
	SPK_PORT&=~(1<<SPK);
	}
}
void term_settings()
{	
	while(!(PIND&(1<<SELECT1_BUT)))_delay_ms(10);
	clear();
	uint8_t speed = 0; //9600
	while((PIND&(1<<SELECT1_BUT)))
	{
		x_stream=0;
		y_stream=1;
		lcd_stream_write("Baudrate:");
		switch(speed)
		{
			case 0:lcd_stream_write("9600");
				break;
			case 1:lcd_stream_write("4800");
				break;
			case 2:lcd_stream_write("300 ");
				break;
		}
		if((!(PINB&(1<<LEFT_BUT)))&&(speed!=0))
		{
			speed--;
			while(!(PINB&(1<<LEFT_BUT)))_delay_ms(10);
		}
		else if((!(PIND&(1<<RIGHT_BUT)))&&(speed!=2))
		{
			speed++;
			while(!(PIND&(1<<RIGHT_BUT)))_delay_ms(10);
		}
		_delay_ms(10);
	}
	switch(speed)
	{
		case 0:uartInit(103);break;
		case 1:uartInit(207);break;
		case 3:uartInit(3332);break;
	}
	Iswork=1;
	x_stream=0;
	y_stream=0;
	print("has been started");
	sei();
	while(!(PIND&(1<<SELECT1_BUT)))
		_delay_ms(10);
	clear();
	while((PIND&(1<<SELECT2_BUT)));
	UCSR0B&=~(1<<RXCIE0);//отключаем прерывания
	cli();
	clear();
	Iswork=0;
}
/*
void InTerm()
{
	while((PIND&(1<<SELECT2_BUT)));
	UCSR0B&=~(1<<RXCIE0);//отключаем прерывания
}*/
#endif /* TERMINAL_H_ */