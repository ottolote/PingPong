/*
 * max520.h
 *
 * Created: 06.11.2015 18:33:35
 *  Author: ottokl
 */ 


#ifndef MAX520_DRIVER_H_
#define MAX520_DRIVER_H_

#include <stdint.h>

void max520_init(uint8_t max520_twi_address);
void max520_write(uint8_t channel, uint8_t val);

#endif /* MAX520_DRIVER_H_ */