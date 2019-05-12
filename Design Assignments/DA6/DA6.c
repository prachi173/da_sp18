
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "libraries\MPU6050_def.h"					    /* Include MPU6050 register define file */
#include "libraries\i2c_master.h"                       /* Include i2c Master header file */
#include "libraries\i2c_master.c"					
#include "libraries\uart.h"	                            /* Include USART header file */
#include "libraries\uart.c"				

#define MPU6050_WRITE 0xD0
#define MPU6050_read 0xD1

float Acc_x, Acc_y, Acc_z, Gyro_x, Gyro_y, Gyro_z; // Values for acceleration and position


int main(void){
	char buffer[20], float_[10];
	float Xa, Ya, Za;
	float Xg, Yg, Zg;
	init_MPU6050(); //initialize MPU6050
	init_uart(9600); //initialize UART
	i2c_init(); //initialize i2c
	
	while(1){
		start_read();
		
		//divide raw value by sensitivity scale to get real values
		Xa = Acc_x/16384.0; 
		Ya = Acc_y/16384.0;
		Za = Acc_z/16384.0;

		Xg = Gyro_x/16.4;
		Yg = Gyro_y/16.4;
		Zg = Gyro_z/16.4;
		
		
		// take values in buffer to send all parameters over USART
		dtostrf( Xa, 3, 2, float_ ); 
		sprintf(buffer,"Xa: %s  ",float_);
		USART_SendString(buffer);

		dtostrf( Ya, 3, 2, float_ ); 
		sprintf(buffer,"Ya: %s  ",float_);
		USART_SendString(buffer);

		dtostrf( Za, 3, 2, float_ );
		sprintf(buffer,"Za: %s    ",float_);
		USART_SendString(buffer);


		dtostrf( Xg, 3, 2, float_ ); 
		sprintf(buffer,"Xg: %s  ",float_);
		USART_SendString(buffer);

		dtostrf( Yg, 3, 2, float_ );
		sprintf(buffer,"Yg: %s  ",float_);
		USART_SendString(buffer);

		dtostrf( Zg, 3, 2, float_ );
		sprintf(buffer,"Zg: %s \r\n ",float_);
		USART_SendString(buffer);

		_delay_ms(1000);
	}
	
	return 0;
}



void init_uart(uint16_t baudrate){ 
//uart register setup function
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

void init_MPU6050(void){
	
	//gyro function
	
	_delay_ms(150);	//power up time is > 100ms
	i2c_start(MPU6050_WRITE); 
	i2c_write(SMPLRT_DIV); // write sample rate of 1khz
	i2c_write(0x07); // sample rate = gyroscope output rate / (1 + SMPLRT_DIV)
	i2c_stop();

	i2c_start(MPU6050_WRITE);
	i2c_write(PWR_MGMT_1);
	i2c_write(0x01); //X axis gyroscope reference freq
	i2c_stop();

	i2c_start(MPU6050_WRITE);
	i2c_write(CONFIG); //write to configuration register
	i2c_write(0x00); //fs is 8kHz
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(GYRO_CONFIG); 
	i2c_write(0x18); //gyro configuration refister set range to +- 2000 deg C
	i2c_stop();

	i2c_start(MPU6050_WRITE);
	i2c_write(INT_ENABLE); //write enable register
	i2c_write(0x01);
	i2c_stop();
}

void start_read(void){
//reading function
	i2c_start(MPU6050_WRITE);
	i2c_write(ACCEL_XOUT_H); // set pointer
	i2c_stop();

	i2c_start(MPU6050_read);
	Acc_x = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Acc_y = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Acc_z = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	
	Gyro_x = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Gyro_y = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Gyro_z = (((int)i2c_read_ack()<<8) | (int)i2c_read_nack());
	i2c_stop();
}


