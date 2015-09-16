/*
 * joystick_driver.h
 *
 * Created: 11.09.2015 15:20:30
 *  Author: haakoneh
 */ 


#ifndef JOYSTICK_DRIVER_H_
#define JOYSTICK_DRIVER_H_

typedef enum { JOYSTICK_X, JOYSTICK_Y, SLIDE_L, SLIDE_R} joy_channel;

typedef struct {
	unsigned int joyX;
	unsigned int joyY;
	unsigned int slideL;
	unsigned int slideR;
} JOY_data;

void joystick_init();

//Conver functions
signed int convert_X(unsigned int input);
signed int convert_Y(unsigned int input);

//Simple reading of joystick
unsigned int joystick_read(joy_channel channel);

#endif /* JOYSTICK_DRIVER_H_ */