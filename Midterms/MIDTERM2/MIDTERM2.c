
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>							
#include <stdio.h>	


// include i2c, uart and APDS9960 library files
// APDS9960 library file was edited to add the following
//#define APDS_WRITE			(0x39 << 1) | 0
//#define APDS_READ			(0x39 << 1) | 1					
								
#include "i2c_master.h"                   
#include "i2c_master.c"					
#include "uart.h"	                         
#include "uart.c"		
#include "APDS9960_def.h"

void color_rgb(); 
void APDS9960_init();

uint8_t baud_rate = MYUBRR;	
uint8_t RL, RH;	
uint8_t BL, BH;
uint8_t GL, GH;

uint8_t config;
char outs[255];														

int main(void){
	char buffer[20], float_[10];

	uint8_t red = 0;		
	uint8_t green = 0;							
	uint8_t blue = 0;
	
	init_uart(9600); //initialize UART
	_delay_ms(500);
	USART_SendString("We are live!"); //message to confirm uart is working
	_delay_ms(5000);
	i2c_init(); //initialize i2c
	APDS9960_init(); //initialize sensor

	unsigned char AT_CHECK[] = "AT\r\n"; //AT command working check			
	unsigned char CWMODE_SET[] = "AT+CWMODE=1\r\n"; //Mode set to 1
	unsigned char CWJAP_LOGIN[] = "AT+CWJAP=\"mywifi\",\"mypass\"\r\n"; //login personal wifi 
	unsigned char CIPMUX_SET[] = "AT+CIPMUX=0\r\n"; // CIPMUX set to 0
	unsigned char CIPSTART_SEND[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"; //connect to thingspeak
	unsigned char CIPSEND_FINISH[] = "AT+CIPSEND=100\r\n"; //end communication
	
	//send strings to print and delays added to establish proper communication
	_delay_ms(3000);
	USART_SendString(AT_CHECK);
	_delay_ms(10000);
	USART_SendString(CWMODE_SET);
	_delay_ms(10000);
	USART_SendString(CWJAP_LOGIN);
	_delay_ms(20000);

	while(1){
		
		//send strings to print and delays added to establish proper communication
		USART_SendString(CIPMUX_SET);
		_delay_ms(100000);
		USART_SendString(CIPSTART_SEND);							
		_delay_ms(10000);
		USART_SendString(CIPSEND_FINISH);								
		_delay_ms(10000);
		
		color_rgb(&red, &blue, &green);										
		//print to update field1 with red, field2 with green and field3 with blue
		snprintf(outs,sizeof(outs),"GET https://api.thingspeak.com/update?api_key=7JVWL1NQ7916EIRM&field1=0%05u&field2=%05u&field3=%05u\r\n", red,green,blue);
		USART_SendString(outs);										
		_delay_ms(10000);
	}
	
	return 0;
}

void color_rgb(uint8_t *red, uint8_t *blue, uint8_t *green){
	//read colors
	i2c_readReg(APDS_WRITE, APDS9960_RDATAL, &RL, 1);
	i2c_readReg(APDS_WRITE, APDS9960_RDATAH, &RH, 1);
	i2c_readReg(APDS_WRITE, APDS9960_GDATAL, &GL, 1);
	i2c_readReg(APDS_WRITE, APDS9960_GDATAH, &GH, 1);
	i2c_readReg(APDS_WRITE, APDS9960_BDATAL, &BL, 1);
	i2c_readReg(APDS_WRITE, APDS9960_BDATAH, &BH, 1);
	*red = RH << 8 | RL;
	*blue = BH << 8 | BL;
	*green = GH << 8 | GL;
	
	
}
void APDS9960_init(){
	
	//reading and writing colors to register
	i2c_readReg(APDS_WRITE, APDS9960_ID, &config,1);
	
	if(config != APDS9960_ID_1)
	while(1)
	{
		config = 1 << 1 | 1 << 0 | 1 << 3 | 1 << 4;
	}
	
	i2c_writeReg(APDS_WRITE, APDS9960_ENABLE, &config, 1);
	config = DEFAULT_ATIME;
	
	i2c_writeReg(APDS_WRITE, APDS9960_ATIME, &config, 1);
	config = DEFAULT_WTIME;
	
	i2c_writeReg(APDS_WRITE, APDS9960_WTIME, &config, 1);
	config = DEFAULT_PROX_PPULSE;
	
	i2c_writeReg(APDS_WRITE, APDS9960_PPULSE, &config, 1);
	config = DEFAULT_POFFSET_UR;
	
	i2c_writeReg(APDS_WRITE, APDS9960_POFFSET_UR, &config, 1);
	config = DEFAULT_POFFSET_DL;
	
	i2c_writeReg(APDS_WRITE, APDS9960_POFFSET_DL, &config, 1);
	config = DEFAULT_CONFIG1;
	
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG1, &config, 1);
	config = DEFAULT_PERS;
	
	i2c_writeReg(APDS_WRITE, APDS9960_PERS, &config, 1);
	config = DEFAULT_CONFIG2;
	
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG2, &config, 1);
	config = DEFAULT_CONFIG3;
	
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG3, &config, 1);
}

void init_uart(uint16_t baudrate){ 
//initialize uart
	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);
	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0); 
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); 
}

void uart_outchar(unsigned char c){
//output uart data
	while(!(UCSR0A & (1<<UDRE0))); // wait until sending is possible
	UDR0 = c; // output character saved in c
}

void uart_outstring(char *s){
	while(*s){
		uart_outchar(*s);
		s++;
	}
}

