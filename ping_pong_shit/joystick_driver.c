/*
 * joystick_driver.c
 *
 * Created: 11.09.2015 15:20:45
 *  Author: haakoneh
 */ 

#define F_CPU 4915200UL // 4.9152 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <math.h>
#include "joystick_driver.h"

int center_val_X;
int center_val_Y;

//Sets up driver by stopping clock, ready for interrupt
void joystick_init(){ 
	/*EMCUCR |= (0 << SM0);
	MCUCSR |= (1 << SM1);
	MCUCSR |= (0 << SM2);
	GICR |= (1 << INT0);
	sei();*/
	center_val_X = joystick_readX();
	center_val_Y = joystick_readY();
}
/*
ISR(INT0_vect){
	
}*/




signed int convert_X(unsigned int input){
	/*if(input > center_val_X){
		return ((input - center_val_X) * 100) / (255 - center_val_X);
	} else {
		return -((input) * 100) / center_val_X;
	}*/
	int v = (joystick_readX() - center_val_X);
	return  (v > 127)	? 127 :
			(v < -128)	? -128 :
			 v;
}

signed int convert_Y(unsigned int input){
	/*if(input > center_val_Y){
		return ((input - center_val_Y) * 100) / (255 - center_val_Y);
	} else {
		return ((input - center_val_Y) * 100) / center_val_Y;
	}*/
	int v = (joystick_readY() - center_val_Y);
	return  (v > 127)	? 127 :
			(v < -128)	? -128 :
			v;
}

unsigned int joystick_readX(){
	volatile char* ext_ram = (char*) 0x1400;
	ext_ram[0] = 0x4;
	_delay_us(40);
	uint8_t x = ext_ram[0];
	return x;
}

unsigned int joystick_readY(){
	volatile char* ext_ram = (char*) 0x1400;
	ext_ram[0] = 0x5;
	_delay_us(40);
	uint8_t y = ext_ram[0];
	return y;
}

unsigned int slide_readR(){
	volatile char* ext_ram = (char*) 0x1400;
	ext_ram[0] = 0x7;
	_delay_us(40);
	uint8_t r = ext_ram[0];
	return r;
}

unsigned int slide_readL(){
	volatile char* ext_ram = (char*) 0x1400;
	ext_ram[0] = 0x6;
	_delay_us(40);
	uint8_t l = ext_ram[0];
	return l;
}