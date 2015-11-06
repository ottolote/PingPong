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
#include "joystick_driver.h"
#include "../test_code.h"

#include <util/delay.h>



/*
//Global variable for joystick 
can_message_t joy_message;*/


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
	return (!test_bit(mcp2515_read(MCP_TXB0CTRL), 3));
}

/*
void can_interrupt_vector(){
	//Clear interrupt flag
	rx_flag = 1;
}*/

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
		mcp2515_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	} else {
		message.id = -1;
	}

	return message;
}

/*
//Interrupt routine for CAN bus
ISR(INT0_vect){
	_delay_ms(10);
	can_interrupt_vector();
}*/

void can_test(){
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	//mcp2515_set_loopback_mode();

	can_message_t testmessage;

	testmessage.id = 1;
	testmessage.length = 8;
	for (uint8_t i = 0; i < testmessage.length; i++){
		testmessage.data[i] = 20*i;
	}
	
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));
	//can_message_t rcv;
	while(1){
		can_message_send(&testmessage);

		_delay_us(10); //fix for loopback mode
		/*
		rcv = can_data_receive();*/
		can_print_message(&testmessage);
		testmessage.data[0]++;
		testmessage.id++;
		
		flash_diode();
		_delay_ms(1000);
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

void can_joy_test(){
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));	
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	printf("CANCTRL: %02x\n", mcp2515_read(MCP_CANCTRL));

	uint8_t id = 1;
	while(1){
		can_joystick_transmit(id);
		//id++;
		//_delay_ms(1000);
		flash_diode();
	}
}

void can_joystick_transmit(){
	static can_message_t joy_message;
	joy_message.id = JOY_CAN_ID; 
	joy_message.length = 2;
	
	joy_message.data[0] = read_converted(JOYSTICK_X);
	joy_message.data[1] = read_converted(JOYSTICK_Y);

	//can_print_message(&joy_message);

	can_message_send(&joy_message);
}

void can_button_transmit(uint8_t button_channel) {
		static can_message_t button_message;
		button_message.id = BUTTON_CAN_ID;
		button_message.length = 1;
		
		button_message.data[0] = button_channel;



		can_message_send(&button_message);
}

void can_handle_message(){
	static can_message_t message;
	message = can_data_receive();
	if(message.id != -1){
		flash_diode();
	}
	switch(message.id){
		case IR_CAN_ID:
			printf("ir edge detect: %d\n",message.data[0]);
			return;
		default:
			return;
	}
}