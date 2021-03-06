/*
 * motor.c
 *
 * Created: 06.11.2015 10:39:42
 *  Author: ottokl
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include "motor_driver.h"
#include "../MAX520_driver/max520_driver.h"

//	Enable active low motor output 
void motor_output_enable(uint8_t enable){
	if(enable) {
		PORTC &= ~(1<<MOTOR_OE);
		_delay_us(20);
	} else {
		PORTC |= (1<<MOTOR_OE);
	}
}

//	Resets the encoder
void motor_encoder_reset(){
	PORTC &= ~(1<<MOTOR_RST);
	_delay_us(20);
	PORTC |= (1<<MOTOR_RST);
}

//	Toggles the encoder select byte
void motor_encoder_select_byte(uint8_t sel) {
	if(sel) {
		PORTC |= (1<<MOTOR_SEL);
	} else {
		PORTC &= ~(1<<MOTOR_SEL);
	}
	_delay_us(20);
}

//	Enables the motor on port C
void motor_enable(uint8_t enable) {
	if(enable) {
		PORTC |= (1<<MOTOR_EN);
	} else {
		PORTC &= ~(1<<MOTOR_EN);
	}
}

//	Initializes the motor in port C
void motor_init() {
	max520_init(MAX520_TWI_ADDRESS);
	
	DDRC |= (1<<PC3) | (1<<PC4) | (1<<PC5) | (1<<PC6) | (1<<PC7);
	
	DDRK = 0;
	
	motor_output_enable(ENABLE);
	motor_encoder_reset();
	motor_encoder_select_byte(ENCODER_HIGH);
	motor_speed(0);
	motor_enable(ENABLE);
}


void motor_speed(int16_t speed){
	if (speed > 0) {	motor_direction(MOTOR_LEFT);  }
	else		   {	motor_direction(MOTOR_RIGHT); }
		
	speed = abs(speed);
	
	if		(speed > MOTOR_MAX_SPEED)	{ speed = MOTOR_MAX_SPEED; }
	//else if (speed < MOTOR_MIN_SPEED)	{ speed =				1; }
		
	max520_write(MAX520_CHANNEL_MOTOR, (uint8_t)speed);
}

void motor_direction(uint8_t dir){
	if(dir == MOTOR_LEFT){
		PORTC &= ~(1<<MOTOR_DIR);
	} else {
		PORTC |= (1<<MOTOR_DIR);
	}
}

uint8_t reverse_bits(uint8_t val) {
	val = ((val & 0x55) << 1) | ((val & 0xaa) >> 1);
	val = ((val & 0x33) << 2) | ((val & 0xcc) >> 2);
	val = ((val & 0x0f) << 4) | ((val & 0xf0) >> 4);
	return val;
}

int16_t motor_encoder_read(){
	motor_output_enable(ENABLE);
	
	motor_encoder_select_byte(ENCODER_HIGH);
	uint8_t msb = reverse_bits(PINK);
	motor_encoder_select_byte(ENCODER_LOW);
	uint8_t lsb = reverse_bits(PINK);	
	
	motor_output_enable(DISABLE);
	return -((msb << 8) | lsb);
}


