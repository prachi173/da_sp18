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
	DDRB = 0xFF; //set PB2 to output
	TCCR0B = (1<<CS00) | (1<<CS02); // prescaler 1024
	TCNT0 = 0x01; // reset timer
	
	while(1)
	{
		while((TIFR0 & 0x01) == 0){} //do nothing if flag is 0
		OVFCount++; //increment OVFCounter
		if (OVFCount <= 13) //until ovfcounter reaches 13 timer cycles)
		{
			TCNT0 = 0x01; //keep reseting to 0x01 (counts from 1 to 256 so 255 times)
			TIFR0 = 0x01; //reset overflow flag
		}
		if(OVFCount == 13) //when the overflow reaches 13 cycles
		{
			PORTB = 0x00; //set PORTB to 0
			
		}
		if(13 < OVFCount < 22) //until overflow reaches 22
		{
			TCNT0 = 0x01; //keep reseting timer
			TIFR0 = 0x01; //keep resetting overflow flag
		} 
		if(OVFCount == 22) //when the overflow reaches 22 cycles
		{
			PORTB = 0x04; //set PORTB2 on.
			OVFCount = 0; //reset overflow counter
		}
	}
}
