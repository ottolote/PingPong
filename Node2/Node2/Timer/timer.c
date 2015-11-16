/*
 * timer.c
 *
 * Created: 06.11.2015 16:57:01
 *  Author: ottokl
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "../Drivers/IR_driver/ir_driver.h"
#include "../Drivers/adc_driver.h"
#include "../Drivers/Uart_driver/uart_driver.h"
#include "../Drivers/Can_driver/can_driver.h"
#include "../Drivers/solenoid_driver.h"
#include "../Controller/pi.h"

//	Init the timer and interrupts
void timer_init() {
	//CS3{2:0}   = 101  prescaler set to 1024
	timer_enable();
	
	TCCR5A &= ~(1<<WGM50);
	TCCR5A &= ~(1<<WGM51);
	TCCR5B |=  (1<<WGM52);
	TCCR5B &= ~(1<<WGM53);
	//TCCR3A |=  (1<<COM3A1) | (1<<COM3A0);
	
	OCR5A = TIMER5_OCRA; // 223 for 70 HZ, 7812 for 2 Hz
	
	//enable interrupt on OCR3A compare
	TIMSK5 |= (1<<OCIE5A);
	TIFR5 |= (1<<ICF1);	

	sei();
	
}

//	Enable timer
void timer_enable() {
	TCCR5B |=  (1<<CS52) | (1<<CS50);
	TCCR5B &= ~(1<<CS51);
}

//	Disable timer
void timer_disable() {
	//prescaler = 0, no clock source
	TCCR5B &= ~((1<<CS52) | (1<<CS51) | (1<<CS50));
}

//	Calles at a 100Hz frequency. Checks if IR is obstructed, handles any  
//	messages an updates the controller
ISR(TIMER5_COMPA_vect){
	solenoid_in(); //retracts solenoid after 
	
	if(ir_edge_detected()){
		can_ir_transmit();	
	}
	
	can_handle_message();
	pi_update();
}