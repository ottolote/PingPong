/*
 * game.h
 *
 * Created: 02.11.2015 13:35:20
 *  Author: ottokl
 */ 


#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

volatile uint8_t game_not_lost;
uint8_t ball_rolling;

void game_keep_score();
void game_main();

#endif /* GAME_H_ */