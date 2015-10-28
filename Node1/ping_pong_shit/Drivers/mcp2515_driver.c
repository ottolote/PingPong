/*
 * mcp2515.c
 *
 * Created: 16.10.2015 11:27:35
 *  Author: haakoneh
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
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
	
	if ((value_check & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is not in config mode after reset\n");
	} else {
		;//printf("MCP2515 is in config mode after reset\n");
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
	spi_send(MCP_RESET);	
	spi_deselect();
}


uint8_t mcp2515_read(uint8_t address){
	uint8_t result;

	spi_select();
	
	//Send command and address, then read result
	spi_send(MCP_READ);
	spi_send(address);

	result = spi_read();
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

void mcp2515_set_loopback_mode(){
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
	
	_delay_us(100);
	
	uint8_t operation_mode = mcp2515_read(MCP_CANSTAT) & MODE_MASK;

	if (operation_mode != MODE_LOOPBACK) {
		printf("Not in loopback operation mode\n");
	}
	printf("Op mode: %x\n", operation_mode);
}

void mcp2515_test(){
	mcp2515_set_loopback_mode();

	printf("Test done\n\n");
}

