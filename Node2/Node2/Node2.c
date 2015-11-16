/*
 * Node2.c
 *
 * Created: 28.10.2015 17:02:58
 *  Author: haakoneh
 */ 

#define UART_BAUD 9600
#define MYUBRR F_CPU/16/UART_BAUD-1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Drivers/Can_driver/can_driver.h"
#include "Drivers/Uart_driver/uart_driver.h"
#include "Drivers/PWM_driver/pwm_driver.h"
#include "Drivers/adc_driver.h"
#include "Drivers/motor_driver.h"
#include "Drivers/solenoid_driver.h"
#include "Drivers/IR_driver/ir_driver.h"
#include "Controller/pi.h"
#include "Tests/test_node_2.h"
#include "timer.h"
#include "Drivers/music_driver.h"

//	Calls all init functions, the timer interrupt vector
//	takes care of everything else in the background 
int main(void){
	uart_init(MYUBRR);
	can_init();
	pwm_init();
	adc_init();
	ir_filter_init();
	solenoid_init();
	motor_init();
	pi_init();
	timer_init(); //should be last, starts interrupts

	while(1){ _delay_ms(500); }
}
