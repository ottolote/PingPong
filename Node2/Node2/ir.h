/*
 * ir.h
 *
 * Created: 01.11.2015 13:30:20
 *  Author: haakoneh
 */ 


#ifndef IR_H_
#define IR_H_

#define IR_CHANNEL 0

uint8_t last_ir_val;

void ir_filter_init();
uint8_t ir_obstructed();
uint8_t ir_edge_detected();

#endif /* IR_H_ */