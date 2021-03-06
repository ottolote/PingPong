/*
 * can.c
 *
 * Created: 16.10.2015 11:26:11
 *  Author: haakoneh
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../Uart_driver/uart_driver.h"
#include "can_driver.h"
#include "../MCP2515_driver/mcp2515_driver.h"
#include "../pwm_driver.h"
#include "../solenoid_driver.h"
#include "../IR_driver/ir_driver.h"
#include "../../Controller/pi.h"
#include "../../timer.h"
#include "../music_driver.h"

//	Initializes the can by setting the necessary modes 
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
}

//	Sends the given over can
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

//	Checks if transmit complete
int can_transmit_complete(){
	//Check if TX buffer is not pending
	if(test_bit(mcp2515_read(MCP_TXB0CTRL), 3)){
		return 0;
	} else {
		return 1;
	 }
}

//	Receives the messages, returns it with id = -1 if no message in buffer
can_message_t can_data_receive(){
	can_message_t message;
	
	//Check if RX buffer have message
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
		mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	} else {
		message.id = -1;
	}

	return message;
}

//	Test the can in loopback mode
void can_test(){
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));
	//mcp2515_set_loopback_mode();
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);


	can_message_t testmessage;

	testmessage.id = 1;
	testmessage.length = 8;
	for (uint8_t i = 0; i < testmessage.length; i++){
			testmessage.data[i] = 20*i;
	}
	
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));
	can_message_t rcv;
	while(1){
		can_message_send(&testmessage);
		_delay_us(10);
		while(!can_buffer_empty()){}
		rcv = can_data_receive();
		can_print_message(&rcv);
		

		testmessage.data[0]++;
		testmessage.id++;
		_delay_ms(600);
	}
}

//	Simple function who prints any can message
void can_print_message(const can_message_t *message) {
	if (message->id == -1) {
		printf("No message in buffer\n\n");
	} else {
		printf("Message id: %x\nMessage length %d\n", message->id, message->length);
		printf("Message data: [ %d", message->data[0]);
		for(uint8_t i = 1; i < message->length; i++) {
			printf(", %d",message->data[i]);
		}
		printf(" ]\n\n");
	}
}

//	Handles messages who are recieved
void can_handle_message(){
	static can_message_t message;
	message = can_data_receive();
	can_data_receive();

	if(message.id & (1<<JOY_CAN_ID)) {
		pwm_set_servo(-message.data[CAN_DATA_JOY_X]);
	}
			
	if(message.id & (1<<SLIDER_CAN_ID)) {
		pi_update_posref(message.data[CAN_DATA_SLIDER_R]);
	}
	
	if(message.id & (1<<BUTTON_CAN_ID)) {
		if(message.data[CAN_DATA_BUTTON] == 3) {
			solenoid_out();
		}
	}

	if(message.id == (1<<MUSIC_PLAY_CAN_ID)) {
		timer_disable();
		music_init();
	}
}

//	Transmit a can message to node containing IR data
void can_ir_transmit(){
	static can_message_t ir_message;
	ir_message.length = 1;
	ir_message.id = IR_CAN_ID;
	ir_message.data[0] = last_ir_val;
	can_message_send(&ir_message);
}