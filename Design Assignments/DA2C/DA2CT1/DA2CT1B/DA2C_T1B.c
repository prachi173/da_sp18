/*
 * DA2C_T1_C.c
 *
 * Created: 3/21/2019 8:51:54 PM
 * Author : patel
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

int main(void)
{
	uint8_t OVFCount = 0; //initialize OVF Counter
	DDRB = 0x04; //set PB2 to output
	DDRC = 0x00; //set PortC to input
	PORTC = 0x02; //pull up PinC1 to read input
	TCCR0B = (1<<CS00) | (1<<CS02); // prescaler 1024
//	TCCR0A = 0x00; // Normal Mode
	TCNT0 = 0x03; // Reset timer to start at 3 (counts 253 times)
	
	while(1)
	{
	OVFCount = 0; //reset overflow to 0
	PORTB = 0x00; //set portb to 0. keeps it off until prompted
	if(PINC & (1<<PINC1)) //if PINC1 is on
	{
		PORTB = 0x04; //set PortB2 to on
		TCNT0 = 0x03; //Reset Timer to 3 but continue ovfcount
				
		while(OVFCount < 40) //until overflow reaches 40 cycles
		{
			while((TIFR0 & 0x01) == 0){} //do nothing if overflow flag is 0
				OVFCount++; //increment overflow counter
				if(OVFCount < 40) //otherwise until it reaches 40
				{
					TIFR0 = 0x01; //reset overflow
					TCNT0 = 0x03; //reset timer
				}
			}
			
		}

	}
}
