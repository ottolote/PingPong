/*
 * button_driver.h
 *
 * Created: 04.11.2015 16:56:25
 *  Author: ottokl
 */ 


#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

void button_init();
uint8_t button_read(uint8_t channel);
uint8_t button_rising_edge_detect(uint8_t channel);



#endif /* BUTTON_DRIVER_H_ */