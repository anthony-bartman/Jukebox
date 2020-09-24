/* Anthony Bartman
 * 2/14/19
 *
 * main.c
 *
 *  Main file that creates a UI where a user can play songs through
 *  piezo speaker by using the rotary encoder
 */
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "keypad.h"
#include "speaker.h"
#include "encoder.h"
#include "delay.h"

static int songSel = 0; //Used to determine which song to play
static int currentRotation = -4; //Tracks rotation of encoder
static int playingSong = 0; //determines if the system is playing a song or not
static int totalNumSongs = 6; //Total number of songs

//Main function that runs the program
int main() {
	//init things needed for the lab
	key_init();
	lcd_init();
	speaker_init();
	encoder_init();

	//Sets up main menu for the jukebox
	mainMenu();

	//Infinite loop to run this program mostly on interrupts
	while (1) {
		if (playingSong == 1) { //If the button was pressed, play the song
			playSong(songSel);
			//resets rotation value on encoder
			(TIM2->CNT) &= ~(0xFFFFFFFF);
			//resets other flags and display
			playingSong = 0;
			mainMenu();
		}
	}
	return 0;
}

//Sets up the main menu display for the jukebox
void mainMenu() {
	lcd_clear();
	lcd_print_string("|Song Selection|");
	lcd_set_position(0, 1);
	lcd_print_string("-");
	lcd_set_position(15, 1);
	lcd_print_string("-");
	songSelection();
}

//Sets up the "song playing" menu for the jukebox
void playingMenu() {
	lcd_clear();
	lcd_print_string("==| Listenin |==");
	lcd_set_position(0, 1);
	lcd_print_string("->");
	songSelection();
}

//This updates the song selection parts of the playing or main menu's
void songSelection() {
	lcd_set_position(1, 1);
	if (songSel == 0) {
		lcd_print_string("Super Mario 64");
		lcd_set_position(16, 0); //move cursor off screen
	} else if (songSel == 1) {
		lcd_print_string(" Zelda Secret ");
		lcd_set_position(16, 0); //move cursor off screen
	} else if (songSel == 2) {
		lcd_print_string("Pokemon Center");
		lcd_set_position(16, 0); //move cursor off screen
	} else if (songSel == 3) {
		lcd_print_string(" Tetris Theme ");
		lcd_set_position(16, 0); //move cursor off screen
	} else if (songSel == 4) {
		lcd_print_string(" Pacman Theme ");
		lcd_set_position(16, 0); //move cursor off screen
	} else if (songSel == 5) {
		lcd_print_string(" Zelda Theme  ");
		lcd_set_position(16, 0); //move cursor off screen
	}
}

//If encoder is turned counter clockwise, this IRQ will update LCD display
//to the next song in the list
void EXTI0_IRQHandler() {
	if (*(EXTI_PR ) == 1 << 0) {
		int rotation = getRotation();
		if (rotation >= (currentRotation + 4)) {
			currentRotation = rotation;
			if (songSel < (totalNumSongs - 1)) {
				songSel++;
			} else { //Reset to first song
				songSel = 0;
			}
			songSelection();
		}
		delay_1ms(50); //used so that user does not spin too quickly
	}
	//Disable Interrupt
	*(EXTI_PR ) &= 0x1;
}

//If encoder is turned counter clockwise, this IRQ will update LCD display
//to the next song in the list
void EXTI1_IRQHandler() {
	if (*(EXTI_PR ) == 1 << 1) {
		int rotation = getRotation();
		if (rotation <= (currentRotation - 4)) {
			currentRotation = rotation;
			if (songSel > 0) {
				songSel--;
			} else { //Reset to last song
				songSel = totalNumSongs - 1;
			}
			songSelection();
		}
		delay_1ms(50); //used so that user does not spin too quickly
	}
	//Disable Interrupt
	*(EXTI_PR ) &= 0x2;
}

//Interrupt handler for the button being pressed. This IRQ will start the song
//or pause the song once its playing
void EXTI15_10_IRQHandler() {
	if (playingSong == 0) { //Song is not playing
		playingSong = 1;
		playingMenu();
	} else { //Song is playing
		lcd_clear();
		playSong(10); //Turns off music temporarily
		lcd_print_string("=|:| PAUSED |:|=");
		lcd_set_position(0, 1);
		lcd_print_string("Re-push to start");

		delay_1ms(1000); //delays so it does not double press
		//begin checking for another press
		int temp = GPIOA->IDR;
		temp &= (0x1000);
		while (temp >> 12 == 1) {
			temp = GPIOA->IDR;
			temp &= (0x1000);
		}
		//user repressed button, update displays
		lcd_clear();
		playingMenu();
	}
	delay_1ms(1000); //used so that user does not double hit button
	//Disables Interrupt
	*(EXTI_PR ) &= (1 << 12);
}
