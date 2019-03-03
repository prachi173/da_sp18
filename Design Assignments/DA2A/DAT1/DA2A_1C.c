/*
 * DA2A_1C.c
 *
 * Created: 3/1/2019 10:56:38 PM
 * Author : patel
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
DDRB |= (1<<PB2);

    while (1) 
    {
		PORTB |= (1<<PB2);
		_delay_ms(435); //For 60% duty cycle and period of 725ms, I used the formula [DC = (Timer_on / (Timer_on + Timer_off))*100)
		PORTB &= ~(1<<PB2);
		_delay_ms(290); //LED connected to PORTB2 stays on for 435ms and off for 290ms. This gives us DC = 60%.

    }
	return 1;
}

