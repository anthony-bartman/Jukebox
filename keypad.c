/* Anthony Bartman
 * 12/20/18
 *
 * keypad.c
 *
 *  Runs keypad functions
 */
#include "gpio.h"
#include "keypad.h"
#include "delay.h";

static volatile GPIO* GPIOC = 0x40020800;
static volatile int key = 0;
static char keycode[17] = { '\0', '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8',
			'9', 'C', '*', '0', '#', 'D' };

//Initializes the keypad by manipulating bits
void key_init() {
	//Enable GPIOC with RCC
	*(RCC_AHB1ENR ) = *(RCC_AHB1ENR ) | (1 << GPIOCEN);
	//Rows PC(4-7) to input 0x00
	//Columns PC(0-3) to output 0x55
	GPIOC->MODER &= ~0x0000FFFF;
	GPIOC->MODER |= 0x00000055;
	//Set rows and columns PUPDR to pull up
	GPIOC->PUPDR &= ~0x0000FFFF;
	GPIOC->PUPDR |= 0x00005555;
}

//This method will determine if a key is pressed from the keypad
int key_getkey_noblock() {
	//Used to map binary value to decimal values
	int rowMap[] = { 0, 0, 4, 0, 8, 0, 0, 0, 12 };
	int columnMap[] = { 0, 1, 2, 0, 3, 0, 0, 0, 4 };

	//Checks if a key was pressed regarding which row it was in
	int row = (0xF & (~(GPIOC->IDR & 0x000000F0) >> 4));
	if (row == 0b0000) {
		return 0; //no key was pressed
	} else {
		//SWITCH IDR AND ODR - Make columns inputs and the rows outputs
		GPIOC->MODER &= ~0x0000FFFF;
		GPIOC->MODER |= 0x00005500;
		//delay to update the MODER correctly
		delay_1us(1);
		//Determine which column the key is in
		int column = (0xF & (~(GPIOC->IDR & 0x0000000F)));

		//Use binary value to change to decimal value
		//Rows: change b(1,2,4,8) -> d(0,4,8,12)
		//Columns: change b(1,2,4,8) -> d(1,2,3,4)
		row = rowMap[row];
		column = columnMap[column];

		//SWITCH BACK IDR AND ODR - Make columns outputs and the rows inputs
		GPIOC->MODER &= ~0x0000FFFF;
		GPIOC->MODER |= 0x00000055;

		//returns the decimal row value + decimal column value
		return row + column;
	}
}

//This method will get key value(1-16) if a key is pressed
int key_getkey() {
	key = 0; //reset key value
	int pressedKey = 0;

	while (key == 0) { //checks if a key is pressed
		key = key_getkey_noblock();
	}
	pressedKey = key; //stores a copy of key pressed
	while (key != 0) { //waits until key is released
		key = key_getkey_noblock();
	}
	return pressedKey;
}

//Converts the key pressed's position number to ascii to be displayed to LCD
char key_getchar() {
	key = 0; //reset key value
	key = key_getkey(); //gets "key" number
	//array of characters used on keypad
	return pos_toascii(key);
}

//Helper method used to return a keys position number to Ascii
char pos_toascii(int num){
	return keycode[key];
}

