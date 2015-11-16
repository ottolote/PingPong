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
#define K_P 0.1
#define K_I 0.02


uint8_t direction;
int16_t position;
float integral;

int16_t posref;

uint16_t max_position;

uint8_t printvar;

//	Calibrates the controller by driving to the end of the track in 
//	both directions
void pi_calibrate() {
	
	motor_speed(120);
	
	position = motor_encoder_read();
	int16_t lastpos;
	do {	
		_delay_ms(40);
		lastpos = position;
		position = motor_encoder_read();
	} while (abs(position - lastpos) > 5);
		
	motor_speed(0);
	motor_encoder_reset();
	motor_speed(-100);
	_delay_ms(200);
	
	do {
		_delay_ms(40);
		lastpos = position;
		position = motor_encoder_read();
	} while (abs(position - lastpos) > 300);
	
	motor_speed(0);
	max_position = motor_encoder_read();
}

//	Call the calibration function and placed the servo in the middle
void pi_init() {
	direction = MOTOR_LEFT;
	
	integral = 0;

	printvar = 0;

	pi_calibrate();
	position = max_position;
	pi_update_posref(128);
}

//	Update the reference point for the controller
void pi_update_posref(uint8_t newref) {
	posref = newref*(max_position/OVERFLOW_FIX)/255 * OVERFLOW_FIX;
}

//	Updates the speed input for the motor as a PI controller
void pi_update() {
	position = motor_encoder_read();
	
	static float error;
	error = ((float)position - (float)posref);
	
	integral += (error * (float)TIMER5_OCRA / (float)F_CPU)*1024;  

	int16_t speed = (float)K_P * error + integral * K_I;
	 
	motor_speed(speed);
}
