/*
 * pwm_driver.h
 *
 * Created: 30.10.2015 12:57:48
 *  Author: haakoneh
 */ 


#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

void pwm_init();
void pwm_set_value(uint16_t val);
void pwm_set_servo();

void pwm_test();

#endif /* PWM_DRIVER_H_ */