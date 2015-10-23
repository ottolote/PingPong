/*
 * spi_driver.c
 *
 * Created: 16.10.2015 11:28:11
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "spi_driver.h"

#define SPI_SCK PB7 
#define SPI_SS PB4
#define SPI_MISO PB6
#define SPI_MOSI PB5

void spi_init(){
	//Set MOSI, SCK and SS as output pins 	
	DDRB |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
	
	//Set MISO as input pin
	DDRB &= (1 << SPI_MISO);
	
	//Enable SPI in master mode and set clock rate fosc/16
	DDRB |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}


uint8_t spi_read(){
	//Dummy send in order to read from slave
	spi_send(0);
	
	//Wait to receive data
	while(!(SPSR & (1 << SPIF)));
	
	return SPSR;
}

void spi_send(char data){
	//Start transmission
	SPDR = data;
	
	//Wait for transmission, checks if register is empty 
	while(!(SPSR & (1 << SPIF)));
}

void spi_select(){
	//Set !SS to 0 for selection of slave
	PORTB &= ~(1 << SPI_SS);
}

void spi_deselect(){
	//Set !SS to 1 for deselection of slave
	PORTB |= (1 << SPI_SS);
}
/*
//Not necessary 
void spi_test(){
	printf("Start spi....\n");	
	spi_init();
	
	char test = 'h';
	
	printf("Start while loop....\n");
	while(1) {
		spi_send(test);
		printf(spi_read());
		
		_delay_ms(500);
	}
}*/