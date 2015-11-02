/*
 * adc_driver.h
 *
 * Created: 01.11.2015 15:00:08
 *  Author: haakoneh
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

void adc_init();
uint16_t adc_read(uint8_t channel);

#endif /* ADC_DRIVER_H_ */