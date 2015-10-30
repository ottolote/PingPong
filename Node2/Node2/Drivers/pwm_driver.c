/*
 * servo.c
 *
 * Created: 30.10.2015 12:57:35
 *  Author: haakoneh
 */ 
#include <avr/io.h>

#define MAX_SERVO 32
#define MIN_SERVO 13
#define MID_SERVO (MAX_SERVO + MIN_SERVO) / 2

void pwm_init(){
	//PWM output active (pin5)
	DDRE   |=  (1<<PE3);
	
	//CS3{2:0}   = 101  prescaler set to 64
	TCCR3B |=  (1<<CS32) | (1 << CS30);
	TCCR3B &= ~(1<<CS31);
	
	//WGM3{3:0}  = 1110 mode set to fast PWM
	TCCR3A |=  (1<<WGM31);
	TCCR3A &= ~(1<<WGM30);
	TCCR3B |=  (1<<WGM33) | (1<<WGM32);
	
	//COM3A{1:0} = 10   clear bit on ICR3 compare
	TCCR3A |=  (1<<COM3A1);
	TCCR3A &= ~(1<<COM3A0);
	
	//Setting ICR3 to 312 (20ms)
	ICR3 = 312;
}

void pwm_set_value(unsigned int val) {
	OCR3A = val;
}

void pwm_set_servo(int val){
	val += 128;
	val = (val*18) / 255 + MIN_SERVO;
	
	if (val <= (MID_SERVO + 1) && (val>= (MID_SERVO - 1))) {
		pwm_set_value(MID_SERVO);
	} else if (val >= MIN_SERVO && val <= MAX_SERVO) {
		pwm_set_value(val);
	} else {
		pwm_set_value(MIN_SERVO);
	}
}


void pwm_test(){
	pwm_set_value(150);
}