/*
 * can.c
 *
 * Created: 16.10.2015 11:26:11
 *  Author: haakoneh
 */ 

#include "can_driver.h"
#include "mcp2515_driver.h"

#include <stdio.h>

void can_init(){
	mcp2515_init();
	
	//Turn off filters for RXB0, disable rollover
	mcp2515_bit_modify(MCP_RXB0CTRL, MCP_FILTER_MASK, 0xff);
	mcp2515_bit_modify(MCP_RXB0CTRL, MCP_ROLLOVER_MASK, 0x00);

	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	//mcp2515_bit_modify()
	
	//Turn on interrupt for RX0
	mcp2515_bit_modify(MCP_CANINTE, MCP_RX0IE_MASK, 0xff);

	
}

void can_send(can_message_t message){
	while(mcp2515_read(MCP_TXB0CTRL) & 0b00001000) {;}

	mcp2515_write(message.id >> 3,  MCP_TXB0SIDH);
	mcp2515_write(message.id << 5,  MCP_TXB0SIDL);
	mcp2515_write(message.length,   MCP_TXB0DLC);

	for(int i = 0; i < message.length; i++){
		mcp2515_write(message.data[i], MCP_TXB0D0 + i);
	}

	mcp2515_request_to_send(MCP_TXB0D0);
}

can_message_t can_receive(){
	can_message_t message;

	memset(&message, 0, sizeof(can_message_t));

	if (mcp2515_read(MCP_CANINTF) & 0x01) {
		message.id      = (mcp2515_read(MCP_RXB0SIDH) << 3) | (mcp2515_read(MCP_RXB0SIDL) >> 5);
		message.length  = (mcp2515_read(MCP_RXB0DLC)) & (0x0f);
		for(int i = 0; i < message.length; i++){
			message.data[i] = mcp2515_read(MCP_RXB0D0 + i);
		}
	} else {
		message.id = - 1;
	}
	
	return message;
}

void can_print_message(can_message_t message){
	printf("Message (id: %d, lenght: %d, data = [", message.id, message.length);
	for(uint8_t i = 0; i < 8; i++){
		printf("%d ", message.data[i]);
	}
	printf("]\n");
}
