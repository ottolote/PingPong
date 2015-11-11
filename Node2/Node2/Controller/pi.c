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
#include "../timer.h"

#define MAX_SPEED 120
#define MIN_SPEED 60
#define MIN_THRESHOLD 20
#define K_P 0.024
#define K_I 0


uint8_t direction;
float position;
float integral;
float derivate;

int16_t posref;

uint16_t max_position;

uint8_t printvar;

void pi_calibrate() {
	motor_direction(MOTOR_LEFT);
	motor_speed(76);
	_delay_ms(2000);
	motor_speed(0);
	_delay_ms(50);
	motor_encoder_reset();

	motor_direction(MOTOR_RIGHT);
	motor_speed(70);
	_delay_ms(2000);
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
}

void pi_update_posref(uint8_t newref) {
	posref = newref*32;
}

void pi_update() {
	//PORTC |= (1<<MOTOR_RST);
	position = motor_encoder_read();
	
	int16_t error = (position - posref);
	
	integral += ((float)error * (float)TIMER5_OCRA / (float)F_CPU)*1024; //unit of integral is seconds  

	uint8_t speed = (float)K_P * (float)error;//+ fabs(integral * K_I);

	if (speed > MAX_SPEED)	{ speed = MAX_SPEED; } 
	//else if (speed < MIN_SPEED && speed ){ }

	if(position < posref) {
		motor_direction(MOTOR_RIGHT);
	} else {
		motor_direction(MOTOR_LEFT);
	}

	motor_speed(speed);
 

	//printf("speed: %d\nerror: %d\n\n", speed, error);

}