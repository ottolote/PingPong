/*
 * mcp2515.c
 *
 * Created: 16.10.2015 11:27:35
 *  Author: haakoneh
 */ 

#define F_CPU 4915200UL // 4.9152 MHz
#define F_OSC 4915200UL // 4.9152 MHz
#define UART_BAUD 9600

#include <util/delay.h>
#include <avr/io.h>
#include "uart_driver.h"
#include "spi_driver.h"
#include "mcp2515_driver.h"

void mcp2515_init() {
	volatile uint8_t value_check;

	//Start SPI driver
	spi_init();

	mcp2515_reset();
	
	//Testing for correct initialization
	value_check = mcp2515_read(MCP_CANSTAT);
	//printf("mcp read done\n");
	while ((value_check & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is not in config mode after reset\n");
		mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_CONFIG);
		mcp2515_reset();
		_delay_ms(1000);
	}
}

void mcp2515_write(uint8_t address, uint8_t data){
	spi_select();

	spi_send(MCP_WRITE);
	spi_send(address);
	spi_send(data);

	spi_deselect();
}

void mcp2515_request_to_send(uint8_t command){
	//Check last 3 bits of given command
	if (command <= 7) {
		command = MCP_RTS | command;
	} else {
		command = MCP_RTS;
	}

	//Select, send RTS command, then deselect
	spi_select();
	spi_send(command);
	spi_deselect();
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	spi_select();

	spi_send(MCP_BITMOD);
	spi_send(address);
	spi_send(mask);
	spi_send(data);

	spi_deselect();
}

void mcp2515_reset(){
	spi_select();
	//printf("spi_select done\n");
	spi_send(MCP_RESET);	
	//printf("spi_send done\n");
	spi_deselect();
	//printf("spi_deselect done\n");
}


uint8_t mcp2515_read(uint8_t address){
	spi_select();
	
	//Send command and address, then read result
	
	//printf("Send MCP_READ\n");
	spi_send(MCP_READ);
	//printf("Send addr\n");
	spi_send(address);

	uint8_t result = spi_read();
	//printf("SPI_read\n");
	spi_deselect();

	return result;	
}

uint8_t mcp2515_read_status(){
	uint8_t result;

	spi_select();
	spi_send(MCP_READ_STATUS);
	result = spi_read();

	spi_deselect();

	return result;
}
