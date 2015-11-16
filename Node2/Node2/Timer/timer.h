/*
 * timer.h
 *
 * Created: 06.11.2015 16:57:10
 *  Author: ottokl
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#define TIMER5_OCRA 160 //223 for ca 70 Hz, 7812 for 2 Hz

void timer_init();
void timer_enable();
void timer_disable();


#endif /* TIMER_H_ */