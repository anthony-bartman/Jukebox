/*
 * LCD.H
 *
 * Anthony Bartman
 * 12/13/18
 */

#ifndef LCD_H_
#define LCD_H_

#include <inttypes.h>

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define GPIOAEN 0
#define GPIOCEN 2

#define LCD_TWOLINE 0x38
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY 0x06
#define LCD_DISPON 0x0F

/*
 * lcd_init()
 * This function should:
 * 		1. Enable the GPIOA and C in RCC_AHB1ENR
 * 		2. Initialize LCD screen with correct delays
 */
void lcd_init();

/*
 * lcd_clear()
 * This function should:
 * 		1. Clears the LCD screen
 */
void lcd_clear();

/*
 * lcd_home()
 * This function should:
 * 		1. Moves the cursor to the top left of the LCD screen
 */
void lcd_home();

/*
 * lcd_set_position()
 * This function should:
 * 		1. Sets the position of the LCD cursor
 */
void lcd_set_position();

/*
 * lcd_print_string()
 * This function should:
 * 		1. This will print an ascii string to the LCD screen
 */
void lcd_print_string();

/*
 * lcd_print_num()
 * This function should:
 * 		1. Print a number to the LCD screen
 */
void lcd_print_num();

#endif
