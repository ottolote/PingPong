/*
 * can_driver.h
 *
 * Created: 16.10.2015 11:26:24
 *  Author: haakoneh
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#define test_bit(reg, bit) (reg & (1 << bit))

uint8_t rx_flag;

typedef struct {
	int id;
	uint8_t length;
	int8_t data[8];
} can_message_t;

void can_init();
void can_message_send(can_message_t* message);
int can_error();
int can_transmit_complete();
void can_interrupt_vector();
can_message_t can_data_receive();
void can_print_message(const can_message_t *message);

void can_test();
void can_joy_test();
void can_joystick_transmit();

#endif /* CAN_DRIVER_H_ */