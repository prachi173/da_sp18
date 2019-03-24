/*
 * DA2C_T2B.c
 *
 * Created: 3/22/2019 11:24:03 PM
 * Author : patel
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

volatile int OVFCount; //intialize global counter so we can use it in all functions

int main(void)
{

	DDRB = 0x04; //set PB2 to output
	DDRC = 0x00; //set PortC to input
	PORTC = 0x02; //pull up PINC1 to read input
	TCCR0B = (1<<CS00) | (1<<CS02); // prescaler 1024
	TIMSK0 |= (1<<TOIE0); //enable overflow interrupt
	//	TCCR0A = 0x00; //normal mode is default
	TCNT0 = 0x03; //set timer to start at 3 or 253 counts
	OVFCount = 0; //initialize counter
	sei(); //enable interrupt
	
	while(1)
	{
		PORTB = 0x00; //set PORTB2 to 0
		if(PINC & (1<<PINC1)) //if PINC1 is ON
		{
			PORTB = 0X04; //set PORTB2 to ON
			TCNT0 = 0X03; //reset timer to 3
			OVFCount = 0; //reset overflow counter
			while(OVFCount < 39) //until timer has cycled 39 times, do nothing
			{
				
			}
			
		}

	}
}

ISR(TIMER0_OVF_vect) //overflow interrupt function
{
	
	OVFCount++; //increment overflow
	if(OVFCount < 39) //until less 39
	{
		TCNT0 = 0x03; //reset timer
		TIFR0 = 0x01; //reset overflow flag
	}

}
