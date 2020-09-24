/*
 * KEYPAD.H
 *
 * Anthony Bartman
 * 12/13/18
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <inttypes.h>

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define GPIOCEN 2

/*
 * key_init()
 * This function should:
 * 		1. Sets up the Keypad by enabling GPIOB with RCC
 */
void key_init();

/*
 * key_getkey_noblock()
 * This function should:
 * 		1. Determine when a key is pressed and return a decimal
 * 	value for which key was pressed
 */
int key_getkey_noblock();

/*
 * key_getkey()
 * This function should:
 * 		1. Determine when a key is pressed and released
 */
int key_getkey();

/*
 * key_getchar()
 * This function should:
 * 		1. Change the numerical value returned from the noblock to
 * 	an ascii value to be displayed to the LCD screen
 */
char key_getchar();

/*
 * pos_toascii()
 * This function should:
 * 		1. Change the position key value to
 * 	an ascii value to be displayed to the LCD screen
 */
char pos_toascii();

#endif
