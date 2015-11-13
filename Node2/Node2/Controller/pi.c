/*
 * pi.c
 *
 * Created: 11.11.2015 15:09:16
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <util/delay.h>

#include "../Drivers/motor_driver.h"
#include "pi.h"
#include "../timer.h"

#define OVERFLOW_FIX 64
#define K_P 0.06
#define K_I 0.01


uint8_t direction;
int16_t position;
float integral;
float derivate;

int16_t posref;

uint16_t max_position;

uint8_t printvar;

void pi_calibrate() {
	motor_speed(140);
	_delay_ms(1500);
	motor_speed(0);
	_delay_ms(50);
	motor_encoder_reset();

	motor_speed(-120);
	_delay_ms(1500);
	motor_speed(0);
	_delay_ms(50);
	
	max_position = motor_encoder_read();
	//motor_encoder_reset();
	printf("max pos: %d\n\n", max_position);
}

void pi_init() {
	direction = MOTOR_LEFT;
	
	integral = 0;

	printvar = 0;

	pi_calibrate();
	position = max_position;
	pi_update_posref(128);
}

void pi_update_posref(uint8_t newref) {
	posref = newref*(max_position/OVERFLOW_FIX)/255 * OVERFLOW_FIX;
}

void pi_update() {
	//PORTC |= (1<<MOTOR_RST);
	position = motor_encoder_read();
	
	int16_t error = (position/2 - posref/2);
	
	integral += ((float)error * (float)TIMER5_OCRA / (float)F_CPU)*1024; //unit of integral is seconds  

	int16_t speed = 2*(float)K_P * (float)error+ integral * K_I;

	 
	motor_speed(speed);
 

	//printf("speed: %d\nerror: %d\n\n", speed, error);

}