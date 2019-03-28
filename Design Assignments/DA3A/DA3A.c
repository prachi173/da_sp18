/*
 * DA3A.c
 *
 * Created: 3/27/2019 9:37:20 PM
 * Author : patel
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL)))-1)

void USART_init(unsigned int ubrr);
void USART_tx_string(char *data);

volatile int OVFCount; //declare global overflow counter
char outs[80]; //char limit 
int x; //integer 
float n; //floating number
char str[] = "hello world!"; //string
char empty[] = " "; //empty space

int main(void)
{
	OVFCount = 0; //setup CTC timer 
	OCR0A = 240;
	TCCR0A |= (1<<WGM01);
	DDRC = 0X00;
	PORTC = 0XFF;
	DDRB = 0xFF;
	TIMSK0 |= (1<<OCIE2A);
	TCCR0B |= (1<<CS00) | (1<<CS02);
	sei(); //interrupt enable
	USART_init(BAUD_PRESCALLER); //initialize the USART
	_delay_ms(250); 
	USART_tx_string("\r\nConnected!\r\n"); //we're alive!
	
    while (1) 
    {
		while(OVFCount >= 33){ //when 1 second
		USART_tx_string(str); //print string
		USART_tx_string(empty);	//print empty
		x = rand();		//genrate random integer
		snprintf(outs,sizeof(outs),"%2d\r\n",x); //printing number
		USART_tx_string(outs); 
		USART_tx_string(empty);	
		n = 1.1234; //the floating number 
		dtostrf(n, 4, 4, outs); //assign decimal limits
		USART_tx_string(outs); //print it
		USART_tx_string(empty);	
		OVFCount = 0; //reset counter
			}
			
    }
}

void USART_init(unsigned int ubrr){
	//initializing USART RS-232 using the given code from professor
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);
	
	UCSR0C = (3 << UCSZ00);
}

void USART_tx_string(char *data){
	//send a string to the RS-232
	while ((*data != '\0')){
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = *data;
		data++;
	}
}
ISR(TIMER0_COMPA_vect) //external timer initializing
{
	OVFCount++;

}