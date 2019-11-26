#define E5_ 94
#define D5_ 107
#define C5_ 113
#define B4_ 127
#define A4_ 142
uint8_t music[] ={D5_,C5_,D5_,D5_,0,D5_,C5_,D5_,D5_,0,C5_,D5_,E5_,E5_,D5_,C5_,C5_,B4_,B4_,0,B4_,
A4_,B4_,B4_,0,B4_,A4_,B4_,B4_,0,B4_,A4_,C5_,C5_,D5_,D5_,A4_,A4_,B4_,B4_};
void init_t0(uint8_t note)
{
	TCCR0A = (1<<WGM01);
	TCCR0B=(1<<CS00)|(1<<CS02);//сброс по совпадению делитель на 1024
	OCR0A = note;//число дл€ сравнени€
	/*в итоге получаем частоту равную ~2  гц
	такм образом можно получить звук частотой 1 кгц (один такт - 0 второй такт - 1)
	*/
	TIMSK0 = (1<<OCIE0A);
 
}
void init_t1()
{
	TCCR1B=(1<<CS10)|(1<<CS12)|(1<<WGM12);
	OCR1AH=0b00001000;
	OCR1AL=0x00;
	TIMSK1=(1<<OCIE1A);//разрешаем прерыв
}
uint8_t buff=1;
uint8_t buff3=0;
ISR(TIMER0_COMPA_vect)
{
	if(buff)//нота не пуста
	{
		if(buff3)
		{
			PORTB|=(1<<SPK);
			buff3=0;
		}
		else
		{
			PORTB&=~(1<<SPK);
			buff3=1;
		}
	}
}
uint8_t ind=0;
uint8_t buff2=0;

ISR(TIMER1_COMPA_vect)
{
	if(ind<40) ind++;
	else ind=0;
	init_t0(music[ind]/2);
	if(music[ind]==0) buff=0;
	else buff=1;
}
