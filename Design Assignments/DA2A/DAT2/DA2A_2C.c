/*
 * DA2A_2C.c
 *
 * Created: 3/2/2019 9:02:42 PM
 * Author : patel
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << PORTB2); //set portb2 to output
	DDRC &= ~(1 << PORTC2);//set portc2 to input

		while (1)
	{
		DDRC &= ~(1 << PORTC2); //set portc to input (this is for loop)
		if((PINC & (1 << PORTC2))) //if PINC2 is on
		{
			DDRC |= (1 << PORTC2); //Set PORTC to output 
			PORTC |= (1 << PORTC2);//Set PORTC2 to ON (this reads switch as ON)
			
			PORTB |= (1<<PORTB2); //OUTPUT PB2 (LED3)
			_delay_ms(1250); //Delay for 1.25s 
			PORTB &= ~(1<<PORTB2); //Turn off PB2 (LED3)
			DDRC &= ~(1 << PORTC2);	//Set Portc to input again and loop		
		}
		else
		{
			PORTB &= ~(1<<PORTB2); //if PINC2 is not on, keep LED3 OFF.
		}
		
	}
	

	return 1;
}

