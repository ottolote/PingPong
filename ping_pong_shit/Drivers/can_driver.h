/*
 * can.h
 *
 * Created: 16.10.2015 11:26:24
 *  Author: haakoneh
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#include <stdint.h>

typedef struct can_message_t can_message_t;
struct can_message_t {
	uint8_t     id;
	uint8_t     length;
	uint8_t     data[8];
};

void can_init();
void can_send(can_message_t message);
can_message_t can_receive();
void can_print_message();

#endif /* CAN_DRIVER_H_ */