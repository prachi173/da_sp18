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
	DDRB = 0xFF; //set PORTB to output
	TIMSK0 |= (1<<TOIE0); //enable overflow interrupt
	TCNT0 = 0x20; //start timer at 0x20 or 32 (224 counts)
	sei(); //enable interrupts
	TCCR0B |= (1<<CS00) | (1<<CS02); //prescaler 1024
	
    while (1) 
    {
//main loop. we have nothing other than interrupt function.
	}
}
ISR(TIMER0_OVF_vect) //overflow interrupt function
{
	OVFCount++; //increment overflow
	if(OVFCount < 15) //until timer reaches 15 timer cycles (3360)
	{
		TCNT0 = 0x20; //reset timer
		TIFR0 = 0x01; //reset overflow flag
	}
	if(OVFCount == 15) //when timer cycles 15 times
		{
			PORTB = 0x00; //set PORTB to 0
		}
	if(15 < OVFCount & OVFCount < 25) //between 15 and 25 cycles, reset timer and overflow flag)
	{
		TCNT0 = 0x20;
		TIFR0 = 0x01;
	}
	if(OVFCount == 25) //at 25 cycles
	{
		PORTB = 0x04; //set PORTB2 on
		OVFCount = 0; //reset overflow counter
	}
}
