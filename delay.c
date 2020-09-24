/* Anthony Bartman
 * Embedded 2
 * 12/6/18
 * Function:
 * This is a busy wait delay in ms or us that takes a delay as an argument
 */

#include <inttypes.h>
#include <stdio.h>

#include "delay.h"  //include declaration header file

//This function will loop n times
void delay_1ms(uint32_t n){
//Loop N times
	for(int i = 0; i < n; i++){
	//Load ticks to make 1ms: 16000
		//* this tells compiler to write to memory, it like [r1] in assembly
		*(STK_LOAD) = 16000;
		//Start counter
		*(STK_CTRL) = (1<<EN) | (1<<CLKSOURCE);
	//Check the flag
		//if its 1, end
		// else continue counting

		uint32_t currFlag = 0;
		uint8_t done = 0;
		while (done == 0){
			currFlag = *(STK_CTRL) & (1<<COUNTFLAG);
			if(currFlag == (1<<COUNTFLAG)){
				done = 1;
			}
		}
	}
}

//This function will loop n times
void delay_1us(uint32_t n){
//Loop N times
	for(int i = 0; i < n; i++){
	//Load ticks to make 1ms: 16000
		//* this tells compiler to write to memory, it like [r1] in assembly
		*(STK_LOAD) = 16;
		//Start counter
		*(STK_CTRL) = (1<<EN) | (1<<CLKSOURCE);
	//Check the flag
		//if its 1, end
		// else continue counting

		uint32_t currFlag = 0;
		uint8_t done = 0;
		while (done == 0){
			currFlag = *(STK_CTRL) & (1<<COUNTFLAG);
			if(currFlag == (1<<COUNTFLAG)){
				done = 1;
			}
		}
	}
}
