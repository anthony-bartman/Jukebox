/* Anthony Bartman
 * 2/14/19
 *
 * speaker.c
 *
 *  Runs speaker functions
 */

#include "gpio.h"
#include "speaker.h"
#include "delay.h";
#include "timer.h";

static volatile GPIO* GPIOB = 0x40020400;
static volatile TIM* TIM3 = 0x40000400;
static volatile NOTE pacmanTheme[] = { { B4, sixteen }, { B5, sixteen }, { FS5,
sixteen }, { DS5, sixteen }, { B5, thirtytwo }, { FS5, thirtytwo }, {
FS5, sixteen }, { E5, eigth }, { C5, sixteen }, { C6, sixteen }, { G5,
sixteen }, { E5, sixteen }, { C6, thirtytwo }, { G5, thirtytwo }, { G5,
sixteen }, { E5, eigth }, { B4, sixteen }, { B5, sixteen }, { FS5,
sixteen }, { DS5, sixteen }, { B5, thirtytwo }, { FS5, thirtytwo }, {
FS5, sixteen }, { E5, eigth }, { DS5, thirtytwo }, { E5, thirtytwo }, {
F5, sixteen }, { F5, thirtytwo }, { FS5, thirtytwo }, { G5, sixteen }, {
G5, thirtytwo }, { GS5, thirtytwo }, { A5, sixteen }, { B5, eigth }, {
REST, NOTE_DELAY } };
static volatile NOTE tetrisTheme[] = { { E5, quarter }, { B4, eigth }, { C5,
eigth }, { D5, eigth }, { E5, sixteen }, { D5, sixteen }, { C5, eigth }, { B4,
eigth }, { A4, quarter }, { A4, eigth }, { C5, eigth }, { E5, quarter }, { D5,
eigth }, { C5, eigth }, { B4, quarter }, { B4, sixteen }, { B4,
sixteen }, { C5, eigth }, { D5, quarter }, { E5, quarter }, {
C5, quarter }, { A4, quarter }, { A4, half }, { D5, quarter }, {
D5, eigth }, { F5, eigth }, { A5, quarter }, { G5, eigth }, {
F5, eigth }, { E5, quarter }, { E5, eigth }, { C5, eigth }, {
E5, quarter }, { D5, eigth }, { C5, eigth }, { B4, quarter }, {
B4, sixteen }, { B4, sixteen }, { C5, eigth }, { D5, quarter }, { E5, quarter },
		{ C5, quarter }, { A4, quarter }, { A4, half }, { REST,
				NOTE_DELAY } };
static volatile NOTE pokemonCenter[] = { { D5, eigth }, { A4, eigth }, { D5,
eigth }, { A5, quarter }, { G5, quarter }, { FS5, eigth }, { E5, quarter }, {
CS5,
quarter }, { CS5, eigth }, { REST, half }, { CS5, eigth }, { A4, eigth }, { CS5,
eigth }, { FS5, quarter }, { E5, quarter }, { CS5,
eigth }, { D5,
eigth }, { FS5, quarter }, { FS5, eigth }, { REST, half }, { FS5,
half }, { A5, half }, { G5, eigth }, { A5, eigth }, { G5, eigth }, {
FS5, eigth }, { E5, half }, { CS5, half }, { E5, half }, { FS5,
eigth }, { G5, eigth }, { FS5, eigth }, { E5, eigth }, { D5,
half }, { FS5, half }, { A5, half }, { G5, eigth }, { FS5, eigth },
		{ G5, eigth }, { A5, eigth }, { B5, half }, { A5,
		quarter }, { G5, eigth }, { FS5, quarter }, { G5, half }, { FS5,
		eigth }, { G5, eigth }, { FS5, eigth }, { E5, eigth }, { D5,
		half }, { REST,
				NOTE_DELAY } };
static volatile NOTE zeldaSecret[] = { { G5, sixteen }, { FS5, sixteen }, { DS5,
sixteen }, { A4, sixteen }, { GS4, sixteen }, { E5, sixteen }, { GS5,
sixteen }, { C6, sixteen }, { REST, NOTE_DELAY } };
static volatile NOTE zeldaTheme[] = { { A4, half }, { REST, eigth }, { REST,
eigth }, { A4, eigth },{ A4, triplet },  { A4, triplet }, { A4,
triplet }, { A4, quarter }, { G4, eigth },
		{ A4, quarter }, { REST, eigth }, {
		REST, eigth }, { A4, eigth }, { A4, triplet }, {
		A4, triplet }, { A4,
		triplet }, { A4, quarter }, { G4, eigth }, { A4,
		quarter }, { REST,
		eigth }, { REST, eigth }, { A4, eigth }, { A4, triplet },  { A4, triplet }, { A4,
		triplet }, { A4, eigth }, { E4, sixteen },  { E4,
		sixteen }, { E4, eigth }, { E4, sixteen },  { E4,
		sixteen }, {
		E4, eigth },  { E4, sixteen },  { E4, sixteen },  { E4,
		eigth },  {
		E4, eigth },  { A4, quarter }, { E4, quarter }, {
		E4, eigth }, {
		A4, eigth }, { A4, sixteen }, { B4, sixteen }, { C5, sixteen }, { D5,
		sixteen }, { E5, half }, { REST, eigth }, { E5, eigth }, { E5,
		triplet }, { F5, triplet }, { G5, triplet }, { A5, half }, {
		REST, half }, { A5, eigth }, { A5, triplet }, { G5, triplet }, {
		F5, triplet }, { G5, eigth }, { G5, sixteen }, { F5, sixteen }, { E5,
		half }, { E5, quarter }, { D5, eigth }, { D5, sixteen }, { E5,
		sixteen }, { F5, half }, { E5, eigth }, { D5, eigth }, { C5,
		eigth }, { C5, sixteen }, { D5, sixteen }, { E5, half }, { D5,
		eigth }, { C5, eigth }, { B4, eigth }, { B4, sixteen }, { C5,
		sixteen }, { D5, half }, { F5, quarter }, { E5, eigth },  { E4,
		sixteen }, { E4, sixteen },
		 { E4, eigth }, { E4,
		sixteen },  {
		E4, sixteen }, { E4, eigth }, { E4, sixteen }, { E4,
		sixteen }, { E4, eigth },{ E4, eigth },  { A4, quarter }, { E4, quarter }, { E4,
		eigth }, { A4, eigth }, { A4, sixteen }, { B4,
		sixteen }, { C5,
		sixteen }, { D5, sixteen }, { E5, half }, { REST, eigth }, { E5,
		eigth }, { E5, triplet }, { F5, triplet }, { G5, triplet }, {
		A5, half }, { REST, eigth }, { REST, eigth }, { C6, quarter }, {
		B5, quarter }, { G5, half }, { E5, quarter }, { F5, half }, {
		F5, quarter }, { A5, quarter }, { G5, quarter }, { E5, half }, {
		E5, quarter }, { F5, half }, { F5, quarter }, { A5, quarter }, {
		GS5, quarter }, { E5, half }, { CS5, quarter }, { D5, half }, {
		D5, quarter }, { F5, quarter }, { E5, quarter }, { C5, half }, {
		A4, quarter }, { B4, eigth }, { B4, sixteen }, { C5, sixteen }, { D5,
		half }, { F5, quarter }, { E5, eigth },  { E4,
				sixteen },  { E4,
		sixteen },  { E4, eigth },
		 { E4, sixteen },  { E4,
		sixteen }, {
		E4, eigth },  { E4, sixteen },  { E4, sixteen }, { E4,
		eigth },  {
		E4, eigth }, { REST, NOTE_DELAY } };
static volatile NOTE mario64[] = { { C6, quarter }, { A5, quarter }, { C6,
quarter }, { D6, eigth }, { C6, eigth }, { REST, eigth }, { E5, quarterd }, {
F5, quarter }, {
FS5, quarter }, { G5, quarter }, { REST, quarter }, { G5, eigth }, {
G4, eigth }, { REST, eigth }, { G5, eigth }, { REST, half }, { REST, quarter },
		{ D4, eigth }, { DS4, eigth }, { E4, eigth }, { DS4, eigth }, { E4,
		eigth }, { G4, eigth }, { REST, eigth }, { A4, eigth }, { G4,
		quarter }, { C4, quarter }, { G3, quarter }, { G3, quarter }, {
		G3, eigth }, { GS3, eigth }, { A3, eigth }, { GS3, eigth }, {
		A3, eigth }, { C4, eigth }, { REST, eigth }, { D4, eigth }, {
		C4, quarter }, { A3, quarter }, { A3, quarter }, { A3, quarter }, { GS3,
		eigth }, { G3, eigth }, { FS3, eigth }, { G3,
		eigth }, { G4, eigth }, { REST, quarter }, { G3, eigth }, { FS3,
		quarter }, { G3, eigth }, { G4, eigth }, { REST, quarter }, {
		G3, eigth }, { FS3, eigth }, { G3, eigth }, { G4, eigth }, {
		REST, quarter }, { G4, quarter }, { A4, eigth }, { G4, eigth }, { REST,
		eigth }, { D4, eigth }, { REST, quarter }, { G3, quarter }, {
		G3, quarter }, { D4, eigth }, { DS4, eigth }, { E4, eigth }, {
		DS4, eigth }, { E4, eigth }, { G4, eigth }, { REST, eigth }, {
		A4, eigth }, { G4, quarter }, { AS4, quarter }, { G3, quarter }, { G3,
		quarter }, { GS4, quarter }, { A4, eigth }, { GS4, eigth }, {
		A4,
		eigth }, { C5, eigth }, { REST, eigth }, { D5, eigth }, { C5,
		quarter }, { A4, quarter }, { REST, quarter }, { REST, quarter }, { GS4,
		quarter }, { G4, eigth }, { FS4, eigth }, {
		G4, eigth }, { G3, eigth }, { REST, quarter }, { G4, eigth }, {
		FS4, eigth }, { G4, eigth }, { G3, eigth }, { REST, eigth }, {
		G3, eigth }, { DS4, triplet }, { E4, quarter }, { E4, quarter }, { C4,
		whole }, { REST, NOTE_DELAY } };

static NOTE* pointer = &mario64[0]; //Points to correct array/song that user selects
static int songSize = 1; //Used to determine how many notes to play


//Initializes the speaker
void speaker_init() {
	//Enables RCC timer and GPIOB
	*(RCC_AHB1ENR ) |= (1 << GPIOBEN);
	*(RCC_APB1ENR ) |= (1 << TIM3_EN);

	//Sets PB4 pin to alt function
	GPIOB->MODER &= ~0x00003000;
	GPIOB->MODER |= 0x00000200; //Might be wrong

	//Maps PB4 pin to the timer
	GPIOB->AFRL &= ~0x00030000;
	GPIOB->AFRL |= 0x00020000;

	//Set CCMR1 to enable preload and set pwm
	TIM3->CCMR1 &= ~0x00000078;
	TIM3->CCMR1 |= 0x00000078;

	//This will divide the 16Mhz clock rate by 15+1
	//This slows the rate that of the Timer sampling
	//main system clock
	TIM3->PSC = 15;
}

//Based on the user's input, this will play the song note by note
void playSong(int songChoice) {
	if (songChoice == 0) {
		pointer = &mario64[0]; //& =  means address of
		songSize = sizeof(mario64) / sizeof(NOTE);
		for (int i = 0; i < songSize; i++) {
			speaker_playNote(pointer->frequency, pointer->duration);
			pointer++;
			speaker_playNote(0,NOTE_DELAY); //Delay after each note to simulate a key released
		}
	} else if (songChoice == 1) {
		pointer = &zeldaSecret[0]; //& =  means address of
		songSize = sizeof(zeldaSecret) / sizeof(NOTE);
		for (int i = 0; i < songSize; i++) {
			speaker_playNote(pointer->frequency, pointer->duration);
			pointer++;
			speaker_playNote(0,NOTE_DELAY); //Delay after each note to simulate a key released
		}
	} else if (songChoice == 2) {
		pointer = &pokemonCenter[0]; //& =  means address of
		songSize = sizeof(pokemonCenter) / sizeof(NOTE);
		for (int i = 0; i < songSize; i++) {
			speaker_playNote(pointer->frequency, pointer->duration);
			pointer++;
			speaker_playNote(0,NOTE_DELAY); //Delay after each note to simulate a key released
		}
	} else if (songChoice == 3) {
		pointer = &tetrisTheme[0]; //& =  means address of
		songSize = sizeof(tetrisTheme) / sizeof(NOTE);
		for (int i = 0; i < songSize; i++) {
			speaker_playNote(pointer->frequency, pointer->duration);
			pointer++;
			speaker_playNote(0,NOTE_DELAY); //Delay after each note to simulate a key released
		}
	} else if (songChoice == 4) { //Stop music from playing
		pointer = &pacmanTheme[0]; //& =  means address of
		songSize = sizeof(pacmanTheme) / sizeof(NOTE);
		for (int i = 0; i < songSize; i++) {
			speaker_playNote(pointer->frequency, pointer->duration);
			pointer++;
			speaker_playNote(0,NOTE_DELAY); //Delay after each note to simulate a key released
		}
	} else if (songChoice == 5) {
		pointer = &zeldaTheme[0]; //& =  means address of
		songSize = sizeof(zeldaTheme) / sizeof(NOTE);
		for (int i = 0; i < songSize; i++) {
			speaker_playNote(pointer->frequency, pointer->duration);
			pointer++;
			speaker_playNote(0,NOTE_DELAY); //Delay after each note to simulate a key released
		}
	} else { //IF there is no song for that choice, play nothing
		speaker_playNote(0, 0);
	}
}

//Sends the speaker registers what they need to play a specific note and delay
void speaker_playNote(int frequency, int duration) {
//Sets ARR and CR1 what type of sound to output
	setFreq(frequency);
//Start counting
	TIM3->CCER |= 0x1;
	TIM3->CR1 |= 0x1;

//Delay of note played using delay.h
	delay_1ms(duration);

//Stops timer from counting
	TIM3->CCER &= ~(0x1);
	TIM3->CR1 &= ~(0x1);
}

//Sets the correct frequency of the sound to the ARR and CCR1
void setFreq(int hz) {
	int tone = 1000000 * 1 / hz;
	TIM3->ARR = tone;
	tone = tone / 2;
	TIM3->CCR1 = tone;
}
