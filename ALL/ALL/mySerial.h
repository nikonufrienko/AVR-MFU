/*
 * mySerial.h
 *
 * Created: 16.04.2019 18:49:49
 *  Author: НИКОЛАЙ
 */ 


#ifndef MYSERIAL_H_
#define MYSERIAL_H_
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
/*
103 for 9600
207 for 4800
3332 for 300
*/
void uartInit(unsigned int speed)
{
	UBRR0H=(uint8_t)(speed>>8);
	UBRR0L=(uint8_t) speed;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);//приём передача прерывния 
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}
void send_byte(uint8_t bytik)
{
	while(!( UCSR0A &( 1 << UDRE0)));//ждем готовности к передаче
	UDR0=bytik;
}
void print(char str[])
{
	uint8_t i=0;
	while(str[i]!='\0')//идём до конца строки
	{
		send_byte((uint8_t)str[i]);
		i++;
	}
}
#endif /* MYSERIAL_H_ */