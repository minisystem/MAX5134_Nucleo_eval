/*
 * hardware.h
 *
 *  Created on: Feb 17, 2017
 *      Author: jeff
 */

#ifndef INCLUDE_HARDWARE_H_
#define INCLUDE_HARDWARE_H_

#include "main.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"


#define LED1_PORT	RCC_AHB1Periph_GPIOA //PORTA
#define LED2_PORT	RCC_AHB1Periph_GPIOB //PORTB
#define LED3_PORT	RCC_AHB1Periph_GPIOC //PORTC
#define LED4_PORT	RCC_AHB1Periph_GPIOA //PORTA

#define SWITCH_PORT 2
#define SWITCH_PIN	7


#define LED1	4 //PA4
#define LED2	0 //PB0
#define LED3	0 //PC0
#define LED4	10 //PA10

#define MIDI_RX_PIN	3

#endif /* INCLUDE_HARDWARE_H_ */
