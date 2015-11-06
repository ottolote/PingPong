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
#define MOTOR_OE	7b 

#include <stdint.h>

void motor_init();
void motor_twi_address();
void motor_speed(uint8_t speed);
void motor_direction();
int16_t motor_encoder_read();


#endif /* MOTOR_DRIVER_H_ */