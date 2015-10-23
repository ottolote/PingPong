/*
 * oled_driver.c
 *
 * Created: 18.09.2015 10:26:52
 *  Author: ottokl
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "oled_driver.h"
#include "../font8x8.h"
#include "SRAM_driver.h"

#define F_CPU 4915200UL // 4.9152 MHz

static FILE oled_stdout = FDEV_SETUP_STREAM(oled_print_char, NULL, _FDEV_SETUP_WRITE);

volatile char *oled_command_addr = (char *) 0x1000;
volatile char *oled_data_addr = (char *) 0x1200;

//Global variables in order ro remeber current page and column
uint8_t global_page, global_col;
uint8_t arrow_global = 0;



void oled_init(){
	oled_write_command(0xae);    // display off
	oled_write_command(0xa1);    //segment remap
	oled_write_command(0xda);    //common pads hardware: alternative
	oled_write_command(0x12);
	oled_write_command(0xc8);    //common output scan direction:com63~com0
	oled_write_command(0xa8);    //multiplex ration mode:63
	oled_write_command(0x3f);
	oled_write_command(0xd5);    //display divide ratio/osc. freq. mode
	oled_write_command(0x80);
	oled_write_command(0x81);    //contrast control
	oled_write_command(0x50);
	oled_write_command(0xd9);    //set pre-charge period
	oled_write_command(0x21);
	oled_write_command(0x20);    //Set Memory Addressing Mode
	oled_write_command(0x02);	 //0x00 = horisontal, 0x01 = vertical, 0x02 = page mode
	oled_write_command(0xdb);    //VCOM deselect level mode
	oled_write_command(0x30);
	oled_write_command(0xad);    //master configuration
	oled_write_command(0x00);
	oled_write_command(0xa4);    //out follows RAM content
	oled_write_command(0xa6);    //set normal display
	oled_write_command(0xaf);    // display on
	
	oled_clear_screen();
	oled_pos(2,3);
	oled_printf(" #SWAG");
	oled_pos(3,7);
	oled_printf("  4");
	oled_pos(4,9);
	oled_printf(" LYFE");
	_delay_ms(800);
	oled_clear_screen();
}

void oled_write_command(uint8_t command){
	*oled_command_addr = command;
}

void oled_write_data(uint8_t data){
	*oled_data_addr = data;
}

void oled_home(){
	oled_pos(0, 0);
}

void oled_back(){
	
}

void oled_goto_page(unsigned int page) {
	oled_write_command(0xB0 + page);
	global_page = page;
	_delay_us(50);
}

void oled_clear_line(){
	for (int i = 0; i<128; i++) {
		oled_write_data(0b00000000);
	}
}

void oled_clear_screen() {
	for (uint8_t i = 0; i<8; i++) {
		oled_goto_page(i);
		oled_clear_line();
		_delay_ms(50);
	}
}

void oled_pos(unsigned int row, unsigned int column){
	oled_goto_page(row);
	if(column < (128/FONTWIDTH)){
		//Save column program goes to
		global_col = column * FONTWIDTH;
		//Set lower column address
		*oled_command_addr = 0x00 + (column * (FONTWIDTH)>>4);
		//Set higher column address                                                                                                                                                                                                                                                                                                                                                                     
		*oled_command_addr = 0x10 + (column * (FONTWIDTH)>>4);
	}
}

void oled_print_char(char ch){
	for (uint8_t i = 0; i<FONTWIDTH; i++) {
		oled_write_data(pgm_read_byte(&font[ch - FONTOFFSET][i]));
	}
}

void oled_printf(char* fmt, ...){
	va_list v;
	va_start(v, fmt);
	vfprintf(&oled_stdout, fmt, v);
	va_end(v);
}

//Arrow functions

uint8_t oled_get_arrow_page(){
	return arrow_global;
}

void oled_print_arrow(uint8_t row, uint8_t col){
	arrow_global = row;
	oled_pos(row, col);
	oled_write_data(0b00011000);
	oled_write_data(0b00011000);
	oled_write_data(0b01111110);
	oled_write_data(0b00111100);
	oled_write_data(0b00011000);
}

void oled_clear_arrow(uint8_t row, uint8_t col){
	oled_pos(row, col);
	oled_write_data(0b00000000);
	oled_write_data(0b00000000);
	oled_write_data(0b00000000);
	oled_write_data(0b00000000);
	oled_write_data(0b00000000);
}

void oled_move_arrow(signed int joystick_Y, unsigned int menu_min, unsigned int menu_max){
	if(joystick_Y < 0 && arrow_global < menu_max) {
		oled_clear_arrow(arrow_global, 0);
		arrow_global++;
		oled_print_arrow(arrow_global, 0);
	} else if (joystick_Y > 0 && arrow_global > menu_min) {
		oled_clear_arrow(arrow_global, 0);
		arrow_global--;
		oled_print_arrow(arrow_global, 0);
	}
}

unsigned int oled_arrow_handler(signed int joystick_Y, unsigned int menu_min, unsigned int menu_max){
	oled_move_arrow(joystick_Y, menu_min, menu_max);
	_delay_ms(500);
	return arrow_global;
}

//------------------------------------------------------------------------------------------
//----------------------------------SRAM Functions------------------------------------------
//------------------------------------------------------------------------------------------

void oled_sram_clear_line(){
	for (uint8_t i = 0; i < 128; i++) { 
		SRAM_write(global_page * 128 + i, 0);
	}
}

void oled_sram_clear(){
	for (uint8_t i = 0; i<8; i++) {
		oled_goto_page(i);
		oled_sram_clear_line();
		_delay_ms(50);
	}
}

void oled_sram_print_char(char character){
	for (int i = 0; i < FONTWIDTH; i++) {
		SRAM_write(global_page * 128 + global_col + i, 0);
	}
	
	global_col =+ FONTWIDTH;
}

void oled_sram_print(char *data){
	int i = 0;
	
	while(data[i] != '\0'){
		oled_sram_print_char(data[i]);
		i++;
	}
}

void oled_refresh() {
	for (int page = 0; page < 8; page++) {
		oled_goto_page(page);
		for (int col = 0; col < 128; col++) {
			*oled_data_addr = SRAM_read(page * 128 + col);
		}
	}
}