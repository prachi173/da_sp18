#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#ifndef BAUD
#define BAUD 9600
#endif
#define ubrr (F_CPU/(16UL*BAUD))-1

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>


//defining ADC0 as 0 to initialize
//using PC0 to read in temperature

#define ADC0 0x00
#define ADC1 0x01
#define AVCC 5000
volatile int OVFCount; //declare global overflow counter
//initializing and calling functions

void USART_init(void);
void USART_send(uint8_t ch);
void USART_print(uint8_t* str);
uint8_t USART_Receive(void);
void ADC_init(void);
uint16_t read_adc(uint8_t adc_temp);

void main(void)
{
	//in main function, the data from read_adc function will be received and converted into readable data. Also initialize Timer0
	OVFCount = 0; //setup CTC timer 
	OCR0A = 240;
	TCCR0A |= (1<<WGM01);
	TIMSK0 |= (1<<OCIE2A);
	TCCR0B |= (1<<CS00) | (1<<CS02);
	sei(); //interrupt enable

	uint16_t temp; //define unsigned 16 bit value for voltage reading from adc
	uint8_t t1, t2, t3; //define unsigned 8bit value for three digits
	
	USART_init(); //initialize the USART
	ADC_init(); //setup analog to digital converter 
	_delay_ms(500);
	
	while(1){

		while(OVFCount >= 33){
		temp = read_adc(ADC0); //set temp as ADC value read from read_adc function
		temp = (((5000/1024)*temp)/10) + 2; //convert temp using prescaler of 1024
		
if(temp >= 100){ 
			t1 = temp / 100;
			t2 = temp % 100;
			t3 = t2 % 10;
			t2 = t2 / 10;
		}
		else{
			t1 = 0;
			t2 = temp / 10;
			t3 = temp % 10;
		}
	//transfer from ASCII to decimal value and send to print
	
		USART_send(t1+48);
		USART_send(t2+48);
		USART_send(t3+48);
		USART_send('C');
		USART_send('\n');
		USART_send('\r');
		OVFCount = 0;
}
		 //delay for a second
}
	
}

void USART_init(void){
	
	//initialize the USART FTDI Basic
	//Set Baud Rate and frame format 8N1
	//enable receiver and transmitter
	
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1<<RXEN0) | (1<< TXEN0);
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
}

void ADC_init(void)
{
	ADMUX |= (1<<REFS0); //AVcc - external cap at AREF
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN); 
	//ADC prescaler and ADC enable
}

uint16_t read_adc(uint8_t adc_temp){
	
	
	ADMUX = (ADMUX & 0xF0) | (adc_temp & 0x0F);
	ADCSRA |= (1<<ADSC); //start conversion
	while(ADCSRA & (1<<ADSC)); //wait for conversion to finish
	return ADC; //return the value in ADC to where the function is called (in temp)
	
}

void USART_send(uint8_t ch)
{
	//wait until UDR0 is empty and then transmit ch
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}

uint8_t USART_Receive(void){
	//wait until receiver is not busy then receive UDR0
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void USART_print(uint8_t* str){
	//receive string str and then send to USART_send function to print
	while(*str){
		USART_send(*str);
		str++;
	}
}
ISR(TIMER0_COMPA_vect) //external timer initializing
{
	OVFCount++;

}
