/*
 * game.c
 *
 * Created: 02.11.2015 13:35:06
 *  Author: ottokl
 */ 

#include <util/delay.h>
#include "game.h"
#include "../Timer/timer.h"
#include "../Drivers/oled_driver.h"
#include "../Drivers/can_driver.h"

uint16_t game_score;
uint8_t game_score_counter;

//	Called on a 70Hz frequency. Increases the score at a 10Hz frequency
void game_keep_score(){
	if (ball_rolling)  { game_score_counter++; }

	if (game_score_counter > 7) {
		game_score++;
		game_score_counter = 0;
		oled_pos(3, 11);
		oled_printf("%d", game_score);
	}
}

//	Resets score
void game_reset_score(){
	game_score = 0;
	game_score_counter = 0;
}

//	Makes the game ready for player, prints appropriate content to OLED
//	and enable all communication with node 2
void game_start(){
	game_reset_score();
	game_not_lost = 1;
/*
	oled_clear_screen();
	oled_pos(3,2);
	oled_printf("remove ball");
	_delay_ms(1500);*/
	oled_clear_screen();
	oled_pos(3, 2);
	oled_printf("Score: ");

	//flush pending game_lost messages
	can_data_receive();
	can_data_receive();
	timer_enable();
	
	ball_rolling = 0;
}

//	Ends the game, and terminates all communication with node 2
void game_end(){
	//turns off all communication with Node2
	timer_disable();
	//can_play_music();
	oled_clear_screen();

	//print GAME OVER
	oled_pos(3, 4);
	oled_printf("GAME OVER");
	_delay_ms(1500);
	oled_clear_screen();


	oled_pos(2, 2);
	oled_printf("Final score:");
	oled_pos(4, 7);
	oled_printf("%d", game_score);
	printf("Final score: %d\n", game_score);

	oled_pos(7, 0);
	oled_printf("press any button");
	while ((!button_read(2) && !button_read(1) && button_read(3)) ); //wait for button press
	_delay_ms(100);
	oled_clear_screen();
}

//Function the program stays in while game is running
void game_main(){
	game_start();
	while(game_not_lost){
		//Write data to SRAM to be used in OLED later
	}
	game_end();
}
