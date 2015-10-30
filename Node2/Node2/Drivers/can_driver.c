/*
 * can.c
 *
 * Created: 16.10.2015 11:26:11
 *  Author: haakoneh
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart_driver.h"
#include "can_driver.h"
#include "mcp2515_driver.h"
#include "pwm_driver.h"

#include <util/delay.h>

void can_init(){
	//Enter config mode
	mcp2515_init();
	printf("CANCTRL (expect 0x87): 0x%02x\n", mcp2515_read(MCP_CANCTRL));

	//RX0 - Turn mask/filter off
	mcp2515_bit_modify(MCP_RXB0CTRL, 0b01100000, 0xFF);
	//RX0 - Disable rollover
	mcp2515_bit_modify(MCP_RXB0CTRL, 0b00000100, 0);

	mcp2515_bit_modify(MCP_CANINTE, MCP_RX0IF, 0xff);
	
	//Enable normal mode
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);

	//Enable interrupt when message is recieved (RX0IE = 1)
}

void can_message_send(can_message_t* message){
	while (!can_transmit_complete()) {}
	//Use standard ID to set the message
	mcp2515_write(MCP_TXB0SIDH, (int8_t)(message->id >> 3));
	mcp2515_write(MCP_TXB0SIDL, (int8_t)(message->id << 5));

	//Set correct data lenght and use data frame, max 8 bytes
	mcp2515_write(MCP_TXB0DLC, (0x0F) & (message->length));

	//For loop to set data bytes
	for (uint8_t i = 0; i < message->length; i++) {
		mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
	}

	mcp2515_request_to_send(1);
}

int can_error(){
	uint8_t error = mcp2515_read(MCP_TXB0CTRL);
	if (test_bit(error, 4) || test_bit(error, 5)) {
		return 0;
	}
	return 1;
}

int can_transmit_complete(){
	//Check if TX buffer is not pending
	if(test_bit(mcp2515_read(MCP_TXB0CTRL), 3)){
		return 0;
	} else {
		return 1;
	 }
}

void can_interrupt_vector(){
	//Clear interrupt flag
	rx_flag = 1;
}

can_message_t can_data_receive(){
	can_message_t message;
	
	//Check if RX buffer have message
	//if (rx_flag == 1) {
	if (mcp2515_read(MCP_CANINTF) & MCP_RX0IF) {
		//Get ID if message
		message.id = (mcp2515_read(MCP_RXB0SIDH) << 3) | (mcp2515_read(MCP_RXB0SIDL) >> 5);

		//Get lenght of message
		message.length = (0x0F) & (mcp2515_read(MCP_RXB0DLC));

		//Get message data
		for(uint8_t i = 0; i < message.length; i++){
			message.data[i] = mcp2515_read(MCP_RXB0D0 + i);
		}

		//Clear flag
		rx_flag = 0;
		mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	} else {
		message.id = -1;
	}

	return message;
}

//Interrupt routine for CAN bus
ISR(INT0_vect){
	_delay_ms(10);
	can_interrupt_vector();
}

void can_test(){
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));
	//mcp2515_set_loopback_mode();
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);

/*
	can_message_t testmessage;

	testmessage.id = 1;
	testmessage.lenght = 8;
	for (uint8_t i = 0; i < testmessage.lenght; i++){
			testmessage.data[i] = 20*i;
	}*/
	
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));
	can_message_t rcv;
	while(1){
/*
		can_message_send(&testmessage);
		_delay_us(10);*/
		//while(!can_buffer_empty()){}
		rcv = can_data_receive();
		can_print_message(&rcv);
		
/*
		testmessage.data[0]++;
		testmessage.id++;*/
		_delay_ms(600);
	}
}

void can_print_message(const can_message_t *message) {
	if (message->id == -1) {
		printf("No message in buffer\n\n");
	} else {
		printf("Message id: %d\nMessage length %d\n", message->id, message->length);
		printf("Message data: [ %d", message->data[0]);
		for(uint8_t i = 1; i < message->length; i++) {
			printf(", %d",message->data[i]);
		}
		printf(" ]\n\n");
	}
}



void can_read_joy_message(){
	can_message_t joy_message;
	//while(test_bit(MCP_RXB0CTRL)){}
	joy_message = can_data_receive();
	
	if(joy_message.id == -1) { return; }
		
	//can_print_message(&joy_message);
	
	pwm_set_servo(-joy_message.data[0]);
}