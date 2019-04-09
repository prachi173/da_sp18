
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
#include <stdlib.h>

//defining ADC0 as 0 to initialize
//using PC0 to read in temperature

#define ADC0 0x00
#define ADC1 0x01
#define AVCC 5000
volatile int OVFCount; //declare global overflow counter
//initializing and calling functions

void USART_init(void);
void USART_send(char *data);
void ADC_init(void);
uint16_t read_adc(uint8_t adc_temp);
char outs[256];
volatile unsigned int temp; //define unsigned 16 bit value for voltage reading from adc
volatile unsigned int t3; //define unsigned 8bit value for three digits
	
void main(void)
{
	USART_init(); //initialize the USART
	ADC_init(); //setup analog to digital converter 
	_delay_ms(500);
	
	while(1){
					
		/* Initialize AT commands */
		unsigned char AT[] = "AT\r\n";
		//Set device mode to Station mode
		unsigned char AT_CWMODE[] = "AT+CWMODE=1\r\n";
		//Wifi connection, (replaced mine with SSID and Password)
		unsigned char AT_CWJAP[] = "AT+CWJAP=\"ssid\",\"password\"\r\n";
		//single IP Address Mode
		unsigned char AT_CIPMUX[] = "AT+CIPMUX=0\r\n";
		//Start TCP connection to Thingspeak.com at port 80
		unsigned char AT_CIPSTART[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
		//string length
		unsigned char AT_CIPSEND[] = "AT+CIPSEND=100\r\n";
		
//send to USART
		_delay_ms(200);
		USART_send(AT);
		_delay_ms(1000);
		USART_send(AT_CWMODE);
		_delay_ms(5000);
		USART_send(AT_CWJAP);
		_delay_ms(5000);
		USART_send(AT_CIPMUX);
		_delay_ms(5000);
		USART_send(AT_CIPSTART);
		_delay_ms(5000);
		USART_send(AT_CIPSEND);
		_delay_ms(5000);

	
			
		temp = read_adc(ADC0); //set temp as ADC value read from read_adc function
		temp = ((temp*500)/1024); //convert temp using prescaler of 1024
		t3 = (temp*1.8)+32;
	//transfer from ASCII to decimal value and send to print
	//
	
	snprintf(outs,sizeof(outs), "GET https://api.thingspeak.com/update?api_key=WI421PP7666NY2VZ&field1=0 \r\n", t3);
	USART_send(outs);//send data
	_delay_ms(5000);
	}
}
	//commented out from previous code since it's not necessary
		//USART_send(t1+48);
		//USART_send(t2+48);
		//USART_send(t3+48);
		//USART_send('C');
		//USART_send('\n');
		//USART_send('\r');



		 //delay for a second

	


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

void USART_send(char *data ) {
	while ((*data != '\0')) {
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}
