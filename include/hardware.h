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

#define LDAC_PIN GPIO_Pin_9
#define DAC_CS_PIN GPIO_Pin_8

#define SWITCH_PORT 2
#define SWITCH_PIN	7


#define LED1	4 //PA4
#define LED2	0 //PB0
#define LED3	0 //PC0
#define LED4	10 //PA10

#define MIDI_RX_PIN	3 //PA3

//DAC control bit defines for Write. Requires min 33ns HIGH to LOW LDAC pulse to latch all 4 DACs simultaneously. Can also configure for Write-through which doesn't require LDAC pulsing
#define DAC_CHAN_0 0b00010001
#define DAC_CHAN_1 0b00010010
#define DAC_CHAN_2 0b00010100
#define DAC_CHAN_3 0b00011000

#endif /* INCLUDE_HARDWARE_H_ */
