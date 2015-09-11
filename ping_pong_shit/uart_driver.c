/*
 * uart_driver.c
 *
 * Created: 28.08.2015 14:25:51
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include <stdio.h>

#include "uart_driver.h"

static FILE uart_io = FDEV_SETUP_STREAM(uart_sendChar, uart_getChar, _FDEV_SETUP_RW);

void uart_init(unsigned int ubrr){
	//Set baud rate
	UBRR0L = (unsigned int) ubrr;
	
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	stdout = &uart_io;
	stdin = &uart_io;

}

void uart_sendChar(unsigned char data){
	//Wait for empty transmitter buffer
	while(!(UCSR0A & (1<<UDRE0)));
		
	//Put data into buffer, then send the data
	UDR0 = data;
}


unsigned int uart_getChar(){
	//Wait for the data to be received 
	while(!(UCSR0A & (1<<RXC0))){}
	
	//Get status and the 9th bit, then data from buffer
	return UDR0;
}
