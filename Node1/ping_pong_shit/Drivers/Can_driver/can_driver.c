/*
 * can.c
 *
 * Created: 16.10.2015 11:26:11
 *  Author: haakoneh
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>

#include "../Uart_driver/uart_driver.h"
#include "../oled_driver.h"
#include "can_driver.h"
#include "../MCP2515_driver/mcp2515_driver.h"
#include "../Joystick_driver/joystick_driver.h"
#include "../../Test_code/test_code.h"
#include "../Button_driver/button_driver.h"
#include "../../game.h"


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
	static uint8_t prevX;
	static uint8_t prevY;
	static uint8_t prevL;
	static uint8_t prevR;
	
	joy_message.id = 0x000000; 
	joy_message.length = 5;
	

	//reduce sent messages when joystick is not changing - NOT WORKING WHEN prev < data.
	if(		/*abs((int)(prevX - joy_message.data[0])) > JOYSTICK_ERROR_MARGIN || 
			abs((int)(prevY - joy_message.data[1])) > JOYSTICK_ERROR_MARGIN*/ 1) {
		joy_message.data[CAN_DATA_JOY_X] = read_converted(JOYSTICK_X);
		joy_message.data[CAN_DATA_JOY_Y] = read_converted(JOYSTICK_Y);
		joy_message.id |= (1<<JOY_CAN_ID);
			
		//flash_diode();
	} 
	prevX = joy_message.data[CAN_DATA_JOY_X];
	prevY = joy_message.data[CAN_DATA_JOY_Y];
	
	//reduce sent messages when slider = prev
	if(		/*abs(((int)prevL - (int)joy_message.data[2])) > JOYSTICK_ERROR_MARGIN ||
			abs(((int)prevR - (int)joy_message.data[3])) > JOYSTICK_ERROR_MARGIN*/ 1) {
		joy_message.data[CAN_DATA_SLIDER_L] = joystick_read(SLIDE_L);
		joy_message.data[CAN_DATA_SLIDER_R] = joystick_read(SLIDE_R);
		joy_message.id |= (1<<SLIDER_CAN_ID);
		//flash_diode();
	}	
	prevL = joy_message.data[CAN_DATA_SLIDER_L];
	prevR = joy_message.data[CAN_DATA_SLIDER_R];
	
	
	if(button_rising_edge_detect(3)){
		joy_message.data[CAN_DATA_BUTTON] = 3;
		joy_message.id |= (1<<BUTTON_CAN_ID);
		ball_rolling = 1;
	} else { joy_message.data[CAN_DATA_BUTTON] = 0; }
	
	
	can_message_send(&joy_message);
	//flash_diode();
}

void can_handle_message(){
	static can_message_t message;
	message = can_data_receive();
	can_data_receive();
		
	//can_print_message(&message);
			
	if(message.id == IR_CAN_ID) {
		flash_diode();
		game_not_lost = 0;
	}
		
}	

#define SLP 110

void can_play_music(){
	static can_message_t message;
	message.id = (1<<MUSIC_PLAY_CAN_ID);
	message.length = 0;
	//puts("NEVER GONNA GIVE");
	can_message_send(&message);
	while(1) {
		
		oled_clear_screen();
		oled_pos(1, 2);
		oled_printf("NEVER");
		_delay_ms(2*SLP);
		oled_clear_screen();

		oled_pos(2, 4);
		oled_printf("GONNA");
		_delay_ms(2*SLP);
		oled_clear_screen();

		oled_pos(3, 6);
		oled_printf("GIVE");
		_delay_ms(4*SLP);
		oled_clear_screen();

		oled_pos(4, 8);
		oled_printf("YOU");
		_delay_ms(4*SLP);
		oled_clear_screen();

		oled_pos(5, 10);
		oled_printf("UP");
		_delay_ms(8*SLP);

		oled_clear_screen();


		//_delay_ms(1*SLP);
		oled_pos(1, 8);
		oled_printf("NEVER");
		_delay_ms(2*SLP);
		oled_clear_screen();

		oled_pos(2, 7);
		oled_printf("GONNA");
		_delay_ms(2*SLP);
		oled_clear_screen();

		oled_pos(3, 6);
		oled_printf("LET");
		_delay_ms(4*SLP);
		oled_clear_screen();

		oled_pos(4, 5);
		oled_printf("YOU");
		_delay_ms(4*SLP);
		oled_clear_screen();

		oled_pos(5, 2);
		oled_printf("DOWN");
		_delay_ms(8*SLP);


		oled_clear_screen();


		//_delay_ms(1*SLP);
		oled_pos(1, 2);
		oled_printf("NEVER");
		_delay_ms(2*SLP);
		oled_clear_screen();

		oled_pos(2, 4);
		oled_printf("GONNA");
		_delay_ms(2*SLP);
		oled_clear_screen();

		oled_pos(3, 6);
		oled_printf("RUN");
		_delay_ms(4*SLP);
		oled_clear_screen();

		oled_pos(4, 8);
		oled_printf("AROUND");
		_delay_ms(8*SLP);
		oled_clear_screen();

		oled_pos(5, 10);
		oled_printf("AND");
		_delay_ms(6*SLP);


		oled_clear_screen();

		oled_pos(2, 8);
		oled_printf("DESERT");
		_delay_ms(6*SLP);
		oled_clear_screen();

		oled_pos(4, 4);
		oled_printf("YOU");
		_delay_ms(10*SLP);
		_delay_ms(18);
	}
}