/*
 * ENCODER.H
 *
 * Anthony Bartman
 * 2/14/19
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <inttypes.h>
#include "gpio.h"
#include "timer.h"

//RCC enables used to set encoder to interface mode
#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define RCC_APB1ENR (volatile uint32_t*) 0x40023840
#define RCC_APB2ENR (volatile uint32_t*) 0x40023844
#define GPIOAEN 0
#define TIM2_EN 0
#define SYSCFGEN 14

//Interrupt addresses
#define SYSCFG_EXTICR1 (volatile uint32_t*) 0x40013808
#define SYSCFG_EXTICR4 (volatile uint32_t*) 0x40013814
#define EXTI_IMR (volatile uint32_t*) 0x40013C00
#define EXTI_RTSR (volatile uint32_t*) 0x40013C08
#define EXTI_FTSR (volatile uint32_t*) 0x40013C0C
#define EXTI_PR (volatile uint32_t*) 0x40013C14
#define NVIC_ISER0 (volatile uint32_t*) 0xE000E100
#define NVIC_ISER1 (volatile uint32_t*) 0xE000E104

//Used to set PA0 and PA1 to encoder mode; need CH1 and CH2
static volatile GPIO* GPIOA = 0x40020000;
static volatile TIM* TIM2 = 0x40000000;

/*
 * encoder_init()
 * This function should:
 * 		1. Sets up the keypad by enabling GPIOA with RCC
 * 		2. Enable PA0,PA1 - Turn dials   PA12 - is button
 * 		3. Set encoder into encoder interface mode
 * 		4. Make encoder interrput driven
 */
void encoder_init();
/*
 *  getRotation()
 *  Function:
 * 		Simple getter method that gets the rotation of the encoder from
 * 		the timer register.
 */
int getRotation();

#endif
