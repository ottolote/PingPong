/*
 * motor_driver.h
 *
 * Created: 06.11.2015 10:39:59
 *  Author: ottokl
 */ 


#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#define MOTOR_DIR	3
#define MOTOR_EN	4
#define MOTOR_SEL	5
#define MOTOR_RST	6
#define MOTOR_OE	7

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 0

#define ENABLE 1
#define DISABLE 0

#define ENCODER_LOW 1
#define ENCODER_HIGH 0

#define MOTOR_MAX_SPEED 180


#include <stdint.h>

void motor_init();
void motor_speed(int16_t speed);
void motor_direction(uint8_t dir);
int16_t motor_encoder_read();
void motor_output_enable(uint8_t enable);
void motor_encoder_reset();

#endif /* MOTOR_DRIVER_H_ */