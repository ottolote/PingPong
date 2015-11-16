/*
 * servo.c
 *
 * Created: 30.10.2015 12:57:35
 *  Author: haakoneh
 */ 
#include <avr/io.h>
#include "pwm_driver.h"

//prescaler 64
/*
#define MAX_SERVO 525
#define MIN_SERVO 225
#define DUTY_CYCLE_STEPS 5000*/
//precaler 8
#define MAX_SERVO 4200
#define MIN_SERVO 1800
#define DUTY_CYCLE_STEPS 40000

#define MID_SERVO (MAX_SERVO + MIN_SERVO) / 2
#define UNDERFLOW_FIX 32
#define VAL_SCALE (MAX_SERVO - MIN_SERVO) 

void pwm_init(){
	//PWM output active (pin5)
	DDRE   |=  (1<<PE3);
	
	//CS3{2:0}   = 011  prescaler set to 64
/*
	TCCR3B |=  (1<<CS30) | (1 << CS31);
	TCCR3B &= ~(1<<CS32);*/

	//CS3{2:0}   = 010  prescaler set to 8
	TCCR3B |=   (1<<CS31);
	TCCR3B &= ~((1<<CS32) | (1<<CS30));
	
	//WGM3{3:0}  = 1110 mode set to fast PWM
	TCCR3A |=  (1<<WGM31);
	TCCR3A &= ~(1<<WGM30);
	TCCR3B |=  (1<<WGM33) | (1<<WGM32);
	
	//COM3A{1:0} = 10   clear bit on ICR3 compare
	TCCR3A |=  (1<<COM3A1);
	TCCR3A &= ~(1<<COM3A0);
	
	//Setting ICR3 to DUTY_CYCLE_STEPS (20ms)
	ICR3 = DUTY_CYCLE_STEPS;
	pwm_set_value(MID_SERVO);
}

void pwm_set_value(uint16_t val) {
	OCR3A = val;
}

void pwm_set_servo(int16_t val){
	val += 128;
	//printf("%d\n", val);
	val = (val*(VAL_SCALE/UNDERFLOW_FIX)) / 255 * UNDERFLOW_FIX + MIN_SERVO;
	pwm_set_value(val);
}


void pwm_test(){
	pwm_set_value(150);
}