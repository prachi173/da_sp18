/*
 * DA2C_T3A.c
 *
 * Created: 3/23/2019 1:40:33 AM
 * Author : patel
 */ 

/*
 * DA2C_T2A.c
 *
 * Created: 3/22/2019 10:00:25 PM
 * Author : patel
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

volatile int OVFCount; //intialize global counter so we can use it in all functions

int main(void)
{
	OVFCount = 0; //initialize overflow counter
	OCR0A = 227; //compare value at 227
	TCCR0A |= (1<<WGM01); //CTC mode
	
	DDRB = 0xFF; //set PORTB to output
	TIMSK0 |= (1<<OCIE2A); //compare interrupt enable
	TCCR0B |= (1<<CS00) | (1<<CS02); //prescaler 1024
	sei();

    while (1) 
    {
//main loop. no function until interrupt
	}
}
ISR(TIMER0_COMPA_vect) //timer compare interrupt function
{
	OVFCount++; //increment overflow counter

	if(OVFCount == 15) //when timer cycles 253 counts 15 times
		{
			PORTB = 0x00; //set PORTB to 0
		}
		
	if(OVFCount == 25) //when timer cycles 253 counts 25 times
	{
		PORTB = 0x04; //set PORTB2 to ON
		OVFCount = 0; //reset overflow counter
	}
}
