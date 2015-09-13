/*
 * joystick_driver.h
 *
 * Created: 11.09.2015 15:20:30
 *  Author: haakoneh
 */ 


#ifndef JOYSTICK_DRIVER_H_
#define JOYSTICK_DRIVER_H_

void joystick_init();

//Conver functions
signed int convert_X(unsigned int input);
signed int convert_Y(unsigned int input);

//Simple reading of joystick
unsigned int joystick_readX();
unsigned int joystick_readY();
unsigned int slide_readR();
unsigned int slide_readL();
#endif /* JOYSTICK_DRIVER_H_ */