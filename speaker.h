/* Anthony Bartman
 * 2/14/19
 *
 * speaker.h
 *
 *  Holds speaker functions
 */

#ifndef SPEAKER_H_
#define SPEAKER_H_

//Speaker Init Values RCC address and bit enables
#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define RCC_APB1ENR (volatile uint32_t*) 0x40023840
#define TIM3_EN 1
#define GPIOBEN 1

//Notes used for the songs programmed into the jukebox
#define G2 98
#define FS3 185
#define G3 196
#define GS3 207
#define A3 220
#define C4 261
#define D4 293
#define DS4 311
#define E4 329
#define FS4 370
#define G4 392
#define GS4 415
#define A4 440
#define AS4 466
#define B4 493
#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define FS5 740
#define G5 784
#define GS5 830
#define A5 880
#define AS5 932
#define B5 987
#define C6 1046
#define D6 1174
//REST (dummy) frequency
#define REST 1

//Note Durations in milli-seconds
#define whole 2000
#define half 1000
#define quarterd 750
#define quarter 500
#define eigth 250
#define sixteen 125
#define thirtytwo 85
#define triplet 166
//Simulates when one would release a key on keyboard
#define NOTE_DELAY 5

#include <inttypes.h>

//Used to initialize a note with a frequency and a duration
typedef struct {
	int frequency;
	int duration;
} NOTE;

//Initializes the speaker
void speaker_init();
//Based on the songChoice, this method will iterate through
//an array that holds the song
void playSong(int songChoice);
//This method will play a note from by sending values to the
//registers needed for the speaker
void speaker_playNote(int duration, int frequency);
//Sets the frequency to get the correct sound for a note
void setFreq(int hz) ;

#endif
