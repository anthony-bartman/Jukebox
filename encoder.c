/* Anthony Bartman
 * 2/15/19
 *
 * encoder.c
 *
 *  Runs encoder functions
 */

#include "encoder.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"

//Initializes the encoder
void encoder_init() {
	//Enables GPIOA with RCC
	*(RCC_AHB1ENR ) |= (1 << GPIOAEN);

	//Set the MODER pins to:
	//PA0-1: 10 (ALTFUNCT)  PA12: 00 (INPUT)
	GPIOA->MODER &= ~0x0300000F;
	GPIOA->MODER |= 0x0000000A;

	//Set pins to pull up
	GPIOA->PUPDR |= 0x01000005;

	//Sets Pins PA0-1 to correct Alt function
	// -- ALTF 1 is TIMER for channel 1 and 2
	GPIOA->AFRL &= ~0x000000FF;
	GPIOA->AFRL |= 0x00000011;

	/* --------------------------
	 *  Interrupt driven pins
	 * --------------------------
	 */
	//Enable SYSCFG
	*(RCC_APB2ENR ) |= (1 << SYSCFGEN);
	//Enable EXTI PA0-1, and PA12
	*(SYSCFG_EXTICR1) &= ~(0x000000FF);
	*(SYSCFG_EXTICR4) &= ~(0x0000000F);
	//EXTI settings
	*(EXTI_IMR) |= 0x00001003;  //external mask settings
	*(EXTI_RTSR) |= 0x0001003;  //Rising edge detection
	*(EXTI_FTSR) |= 0x0001003;  //Falling edge detection
	//NVIC enable
	*(NVIC_ISER0) |= 0x000000C0;
	*(NVIC_ISER1) |= 0x00000100;

	/* ----------------------
	 * Encoder Interface Mode
	 * ----------------------
	 */
	//Enable TIM2 in RCC
	*(RCC_APB1ENR ) |= (1 << TIM2_EN);
	//CCnS - CCMR1 - MAKE CC1S and CC2S '01' for input mode
	TIM2->CCMR1 &= ~(0x00000303);
	TIM2->CCMR1 |= 0x00000101;

	//Set TIM2_SMCR to "011", to count on both edges
	TIM2->SMCR &= ~0x00000007;
	TIM2->SMCR |= 0x00000003;

	//Enables the TIMER to start measuring encoder
	TIM2->CR1 |= 0x00000001;

}

//This method returns a number based on the position of the encoder
int getRotation(){
	return TIM2->CNT;
}
