/*
 * timer.c
 *
 * Created: 06.11.2015 16:57:01
 *  Author: ottokl
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "ir.h"
#include "Drivers/adc_driver.h"
#include "Drivers/uart_driver.h"
#include "Drivers/can_driver.h"
#include "Drivers/solenoid_driver.h"
#include "Controller/pi.h"

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

void timer_enable() {
	TCCR5B |=  (1<<CS52) | (1<<CS50);
	TCCR5B &= ~(1<<CS51);
}

void timer_disable() {
	//prescaler = 0, no clock source
	TCCR5B &= ~((1<<CS52) | (1<<CS51) | (1<<CS50));
}

ISR(TIMER5_COMPA_vect){
	//printf("read: %d\n",adc_read(0));
	//printf("edge: %d\n\n",ir_edge_detected());

	solenoid_in(); //retracts solenoid after 
	
	if(ir_edge_detected()){
		can_ir_transmit();	
	}
	
	can_handle_message();
	pi_update();
}