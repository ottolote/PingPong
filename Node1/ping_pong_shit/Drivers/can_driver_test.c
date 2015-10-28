/*
 * can_driver_test.c
 *
 * Created: 24.10.2015 15:47:28
 *  Author: haakoneh
 */ 

#define F_CPU 4915200UL // 4.9152 MHz
#define F_OSC 4915200UL // 4.9152 MHz
#define UART_BAUD 9600

#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>



#include "can_driver.h"
#include "mcp2515_driver.h"

void can_test_loopback(){
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);

	printf("MCP_CANCTRL: 0x%02x (should be 0x47)\n", mcp2515_read(MCP_CANCTRL));
	printf("MCP_CANSTAT: 0x%02x (should be 0x40)\n", mcp2515_read(MCP_CANSTAT));

	can_message_t message_1;
	can_message_t message_2;
	message_1.id = 1;
	message_1.length = 8;
	for(uint8_t i = 0; i < message_1.length; i++){
		message_1.data[i] = i;
	}

	while(1){
		can_send(message_1);
		can_print_message(message_1);

		message_2 = can_receive();

		can_print_message(message_2);
		message_1.data[0]++;


		_delay_ms(900);
	}
}
