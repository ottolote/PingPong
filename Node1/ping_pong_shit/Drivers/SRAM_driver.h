/*
 * SRAM_driver.h
 *
 * Created: 11.09.2015 12:32:20
 *  Author: haakoneh
 */ 


#ifndef SRAM_DRIVER_H_
#define SRAM_DRIVER_H_

void SRAM_init();
void SRAM_test(void);

void SRAM_write(unsigned int address_offset, unsigned int data);
unsigned int SRAM_read(unsigned int address_offset);

#endif /* SRAM_DRIVER_H_ */