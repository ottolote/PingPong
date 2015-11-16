/*
 * can_driver.h
 *
 * Created: 16.10.2015 11:26:24
 *  Author: haakoneh
 */ 


#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

#define test_bit(reg, bit) (reg & (1 << bit))

#define JOY_CAN_ID 0
#define IR_CAN_ID 1
#define BUTTON_CAN_ID 2
#define SLIDER_CAN_ID 3
#define MUSIC_PLAY_CAN_ID 4

#define CAN_DATA_JOY_X 0
#define CAN_DATA_JOY_Y 1
#define CAN_DATA_SLIDER_L 2
#define CAN_DATA_SLIDER_R 3
#define CAN_DATA_BUTTON 4


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
void can_handle_message();
int can_buffer_empty();

void can_ir_transmit();

void can_test();

#endif /* CAN_DRIVER_H_ */