/*
 * spi_driver.h
 *
 * Created: 16.10.2015 11:28:50
 *  Author: haakoneh
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

void spi_init();

uint8_t spi_read();
void spi_send(char data);

void spi_select();
void spi_deselect();

#endif /* SPI_DRIVER_H_ */