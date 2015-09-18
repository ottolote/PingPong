/*
 * oled_driver.c
 *
 * Created: 18.09.2015 10:26:52
 *  Author: ottokl
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>
#include "oled_driver.h"
#include "font8x8.h"

static FILE oled_stdout = FDEV_SETUP_STREAM(oled_print_char, NULL, _FDEV_SETUP_WRITE);

volatile char *oled_command_addr = (char *) 0x1000;
volatile char *oled_data_addr = (char *) 0x1200;

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
	oled_goto_page(2);
	oled_printf(" #SWAG");
	oled_goto_page(3);
	oled_printf("  4");
	oled_goto_page(4);
	oled_printf(" LYFE");
	_delay_ms(3000);
	oled_clear_screen();
}

void oled_write_command(uint8_t command){
	*oled_command_addr = command;
}

void oled_write_data(uint8_t data){
	*oled_data_addr = data;
}

void oled_home(){
	
}

void oled_back(){
	
}

void oled_goto_page(unsigned int page) {
	oled_write_command(0xB0 + page);
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