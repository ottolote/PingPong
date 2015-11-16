/*
 * max520_driver.c
 *
 * Created: 06.11.2015 18:32:52
 *  Author: ottokl
 */ 
#include <avr/interrupt.h>

#include "max520_driver.h"
//#include <Wire.h>
#include "../../TWI/TWI_Master.h"


#define  MAX520_TWI_ADDR_BASE 0b01010000

static uint8_t twi_address = 0b000;

void max520_init(uint8_t max520_twi_addr){
	TWI_Master_Initialise();
	sei();
	twi_address = max520_twi_addr;
}

void max520_write(uint8_t channel, uint8_t val){
	uint8_t message[3] = {	MAX520_TWI_ADDR_BASE | ((twi_address & 0x07) << 1),
							channel & 0x07,
							val	};
	
	TWI_Start_Transceiver_With_Data(message, 3);
}
