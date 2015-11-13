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
#include "max520_driver.h"

void motor_output_enable(uint8_t enable){
	if(enable) {
		PORTC &= ~(1<<MOTOR_OE);
		_delay_us(20);
	} else {
		PORTC |= (1<<MOTOR_OE);
	}
}

void motor_encoder_reset(){
	PORTC &= ~(1<<MOTOR_RST);
	_delay_us(20);
	PORTC |= (1<<MOTOR_RST);
}

void motor_encoder_select_byte(uint8_t sel) {
	if(sel) {
		PORTC |= (1<<MOTOR_SEL);
	} else {
		PORTC &= ~(1<<MOTOR_SEL);
	}
	_delay_us(20);
}

void motor_enable(uint8_t enable) {
	if(enable) {
		PORTC |= (1<<MOTOR_EN);
	} else {
		PORTC &= ~(1<<MOTOR_EN);
	}
}

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
	if (speed > 0){
		motor_direction(MOTOR_LEFT);
	} else {
		motor_direction(MOTOR_RIGHT);
	}
	speed = abs(speed);
	if (speed > MOTOR_MAX_SPEED)	{ speed = MOTOR_MAX_SPEED; }
		
	max520_write(MAX520_CHANNEL_MOTOR, (uint8_t)speed);
}

void motor_direction(uint8_t dir){
	if(dir == MOTOR_LEFT){
		PORTC &= ~(1<<MOTOR_DIR);
	} else {
		PORTC |= (1<<MOTOR_DIR);
	}
}

void motor_velocity(float velocity){
	//do something useful
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
	//motor_encoder_reset();
	//return reverse_bits(PINK); //temporary
	return -((msb << 8) | lsb);
}


