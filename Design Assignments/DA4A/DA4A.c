#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>			

int sw_motor = 0;				// motor switch

int main(void)
{
	DDRD = 0x40;	// set PD6 to Output
	DDRC = 0x02;	// set PC0 to input and PC1 to output
	PORTC |= (1<<1);	// enable pull-up
	TCCR0A |= (1<<COM0A1)|(1<<WGM01)|(1<<WGM00);	// clear OC0A on compare match, fast PWM
	TCCR0B |= (1<<CS02)|(1<<CS00);	// prescaler = 1024
	PCICR |= (1<<PCIE1);	// enable pin change interrupt 1
	PCMSK1 |= (1<<PCINT9);	// PinC1 for interrupt
	ADMUX |= (1<<REFS0);	// setup reference voltage at Aref
	ADCSRA |= (1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //setup ADC
	sei();
	
	while (1) {	
//wait for interrupt	
	}
}

ISR (PCINT1_vect) {
	if (!(PINC & (1<<PINC1))) {
		if (sw_motor == 0) {
			OCR0A = 0; //compare match at 0, motor is off
			PORTD &= ~(1<<PORTD6); //turn off PortD6
		}
		if (sw_motor == 1) {
			while ((ADCSRA & (1<<ADIF)) == 0);	// process ADC

			OCR0A = ADC; //set OCR0A to the value of ADC (0 at 0V, 512 at 2.5V and 1023 AT 5v)
			PORTD |= (1<<PORTD6); //Turn on Output1
			}
		_delay_ms(500);
		sw_motor ^= 1;
	}
}
