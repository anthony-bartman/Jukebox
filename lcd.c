/* Anthony Bartman
 * 12/20/18
 *
 * lcd.c
 *
 *  Runs LCD functions for LCD screen
 */
#include "gpio.h"
#include "lcd.h"
#include "delay.h"

static volatile GPIO* GPIOC = 0x40020800;
static volatile GPIO* GPIOA = 0x40020000;

//Initailizes the LCD screen
void lcd_init() {
	//Power on delay ~40ms
	delay_1ms(50);
	//Enable GPIOA and C with RCC
	*(RCC_AHB1ENR ) = *(RCC_AHB1ENR ) | (1 << GPIOAEN);
	*(RCC_AHB1ENR ) = *(RCC_AHB1ENR ) | (1 << GPIOCEN);
	//Enable select GPIO pins to output
	GPIOC->MODER &= ~0x003F0000;
	GPIOA->MODER &= ~0x00FFFF00;

	GPIOC->MODER |= 0x00150000;
	GPIOA->MODER |= 0x00555500;

	//Init display 2 line
	lcd_cmd((uint8_t) LCD_TWOLINE);
	//clears LCD display
	lcd_cmd((uint8_t) LCD_CLEAR);
	//Clear needs 1.52 ms
	delay_1ms(2);

	//home the LCD cursor
	lcd_cmd((uint8_t) LCD_HOME);
	//home needs 1.52 ms
	delay_1ms(2);

	//Entry mode for LCD - move right, no shift
	lcd_cmd((uint8_t) LCD_ENTRY);
	//Turn display on for LCD
	lcd_cmd((uint8_t) LCD_DISPON);
}

//Clear lcd screen
void lcd_clear() {
	//clears LCD display
	lcd_cmd((uint8_t) LCD_CLEAR);
	//Clear needs 1.52 ms
	delay_1ms(2);
}

//Homes the cursor in the top left of screen
void lcd_home() {
	//home the LCD cursor
	lcd_cmd((uint8_t) LCD_HOME);
	//home needs 1.52 ms
	delay_1ms(2);
}

//Sets position of cursor on screen
void lcd_set_position(int row, int column) {
	//resets cursor to home
	lcd_home();
	if (row >= 0 && row < 17) {
		//checks send data to the second line
		if (column == 1) {
			row += 0x40;
		}
		row = row | (1 << 7);
		lcd_cmd(row);
		delay_1ms(2);
	} else { //data will be off the screen
	}
}

//Prints a array of characters to LCD
void lcd_print_string(char* string) {
	int arraySize = 16;
	int charDisp = 0;

	for (int i = 0; i < arraySize; i++) {
		if (string[i] != '\0') {
			lcd_data((uint8_t) string[i]);
			charDisp++;
		} else {
			i = arraySize;
			//return i;
		}
	}
	//return charDisp;
}

//Prints a number to the LCD screen
void lcd_print_num(int num) {
	int arraySize = 16;
	char numList[16] = { 0 };
	int numDisp = 0;

	sprintf(numList, "%i", num);
	for (int i = 0; i < arraySize; i++) {
		if (numList[i] > 0) {
			lcd_data((uint8_t) numList[i]);
			numDisp++;
		} else {
			//return i;
		}
	}
	//return numDisp;
}

//Helper Methods
//Begins to get LCD ready to perform a command
void lcd_cmd(uint8_t data) {
	GPIOC->BSRR |= (1 << 9) << 16; //rw clr
	GPIOC->BSRR |= (1 << 8) << 16; //rs clr
	lcd_exec(data);
}

//Gets ready to send data to LCD screen
void lcd_data(uint8_t data) {
	GPIOC->BSRR |= (1 << 9) << 16; //rw clr
	GPIOC->BSRR |= (1 << 8); //rs is high
	lcd_exec(data);
}

//Begins to execute LCD command to LCD screen
void lcd_exec(uint8_t data) {
	lcd_set(data);
	lcd_latch();
	//40 uS delay
	delay_1us(100);
}

//Sets data to output bus to be sent to LCD screen
void lcd_set(uint8_t data) {
	GPIOA->ODR &= ~0xFF0;
	GPIOA->ODR |= data << 4;
}

//Pulses enable bit to send data to LCD
void lcd_latch() {
	GPIOC->BSRR |= (1 << 10); //Bring E high
	//Need a 1us delay
	delay_1us(10);
	GPIOC->BSRR |= (1 << 10) << 16; //Bring E low
	//Need  1us delay
	delay_1us(10);
}
