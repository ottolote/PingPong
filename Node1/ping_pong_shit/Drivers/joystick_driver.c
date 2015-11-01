/*
 * joystick_driver.c
 *
 * Created: 11.09.2015 15:20:45
 *  Author: haakoneh
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <math.h>
#include "joystick_driver.h"

int center_val_X;
int center_val_Y;


void joystick_init(){ 
	center_val_X = joystick_read(JOYSTICK_X);
	center_val_Y = joystick_read(JOYSTICK_Y);
}

signed int read_converted(joy_channel channel){

	int v = (joystick_read(channel));

	if      (channel == JOYSTICK_X)  {v -= center_val_X;}
	else if (channel == JOYSTICK_Y)  {v -= center_val_Y;}

	return  (v > 127)	? 127 :
			(v < -128)	? -128 :
			 v;
}

unsigned int joystick_read(joy_channel channel){
	volatile char* ext_ram = (char*) 0x1400;
	ext_ram[0] = 0x4 + channel;
	_delay_us(40);
	return (uint8_t) ext_ram[0];
}

signed int joystick_direction_Y(){
	signed int joy_Y = read_converted(JOYSTICK_Y);
	if(joy_Y >= 100){
		return 1;
	} else if (joy_Y <= -100) {
		return -1;
	} 
	return 0;
}

signed int joystick_direction_X(){
	signed int joy_X = read_converted(JOYSTICK_X);
	if(joy_X >= 100){
		return 1;
	} else if (joy_X <= -100) {
		return -1;
	}
	return 0;
}
