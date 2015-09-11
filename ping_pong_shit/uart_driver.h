/*
 * uart_driver.h
 *
 * Created: 28.08.2015 14:26:30
 *  Author: haakoneh
 */ 


#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

void uart_init(unsigned int ubrr);

void uart_sendChar(unsigned char data);

unsigned int uart_getChar();

#endif /* UART_DRIVER_H_ */