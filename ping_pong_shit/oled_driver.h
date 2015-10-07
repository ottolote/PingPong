/*
 * oled_driver.h
 *
 * Created: 18.09.2015 10:29:05
 *  Author: ottokl
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

/*
#define oled_command_addr ESS 0x1200
#define OLED_DATA_ADDRESS 0x1200
*/
void oled_init();

void oled_write_command(uint8_t command);
void oled_write_data(uint8_t data);
void oled_goto_line();
void oled_clear_line();
void oled_clear_screen();
void oled_print_char(char ch);
void oled_printf(char* fmt, ...);
void oled_print_arrow(uint8_t row, uint8_t pos);
void oled_goto_page(unsigned int page);
void oled_move_arrow(signed int joystick_Y, unsigned int menu_min, unsigned int menu_max);

#endif /* OLED_DRIVER_H_ */