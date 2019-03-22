/*
 * DA2B_C.c
 *
 * Created: 3/9/2019 12:56:24 PM
 * Author : patel
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>


int main(void)
{
	DDRB |= (1<<2); //output PB2
	PORTB = 0x00; //set leds off
	
	DDRD &= (0<<2); //set pd2 to input
	PORTD |= (1<<2); //set portd to enable pind2
	
	EICRA = 0x02; //set falling edge interrupt request 
	EIMSK = (1<<INT0); //enable int0 through mask register
	sei(); //enable interrupts

    while (1);

}

ISR (INT0_vect) //interrupt function
{

	PORTB |= (1<<2);
	_delay_ms(1250);
	PORTB = 0x00;

}

