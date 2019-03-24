/*
 * DA2C_T3B.c
 *
 * Created: 3/23/2019 1:56:02 AM
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
	OCR0A = 240; //set compare value at 240
	TCCR0A |= (1<<WGM01); //CTC Mode
	DDRC = 0X00; //Set PORTC to input
	PORTC = 0XFF; //pull up PORTC to read input
	DDRB = 0xFF; //Set PORTB to output
	TIMSK0 |= (1<<OCIE2A); //enable compare interrupt
	TCCR0B |= (1<<CS00) | (1<<CS02); //prescaler 1024
	sei();
//similar to the codes before, here the timer counts from 0 to 240 and the OVFCount increments evertime this timer reaches 240 and goes back to 0. at 41 overflow count, total timer count is 240x41 = 9840. 41 was rounded from 40.6
//the counter difference between Normal Mode and CTC mode is that normal counts from reset timer upto MAX (256) while CTC counts from 0 upto the compare value (OCR0A)
	
	while(1)
	{
		PORTB = 0x00; //set PORTB2 to 0
		if(PINC & (1<<PINC1)) //when PINC1 is on
		{
			PORTB = 0X04; //set PORTB2 to on
			TCNT0 = 0X00; //reset timer
			OVFCount = 0; //reset overflow count (to count time AFTER the interrupt)
			while(OVFCount < 41) //until overflow count is , do nothing
			{
				
			}
			
		}

	}
}

ISR(TIMER0_COMPA_vect) //compare interrupt
{
	OVFCount++;
//increment overflow. the above function could have been implemented here too but since only the delay uses overflow counter, i didn't.
}
