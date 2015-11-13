/*
 * credits.c
 *
 * Created: 13.11.2015 18:26:55
 *  Author: haakoneh
 */ 

#include <stdio.h>

#include "../Drivers/oled_driver.h"

void credits_print(){
	oled_clear_screen();
	oled_pos(0, 4);
	oled_printf("Made by:");
	oled_pos(1,4);
	oled_printf("ottolote");
	oled_pos(2,4);
	oled_printf("and");
	oled_pos(3,4);
	oled_printf("haakoneh");
}
