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

//MAX5134 control pins
#define LDAC_PIN 		GPIO_Pin_7
#define DAC_CS_PIN 		GPIO_Pin_4

//12 bit STM32 DAC multiplexing control pins
#define DAC_MUX_PORT 	GPIOB
#define D0				GPIO_Pin_1
#define D1				GPIO_Pin_2
#define MUX_SEL_PORT	GPIOA
#define MUX_SEL			GPIO_Pin_11

#define MIDI_LED	GPIO_Pin_6

//define gate pins
#define GATE_PORT GPIOB
#define GATE_1 GPIO_Pin_8
#define GATE_2 GPIO_Pin_9
#define GATE_3 GPIO_Pin_10
#define GATE_4 GPIO_Pin_11

//define GATE LEDs
#define GATE_LED_PORT GPIOB
#define GATE_LED_MASK 0x0FFF;
#define GATE_LED_1 GPIO_Pin_12
#define GATE_LED_2 GPIO_Pin_13
#define GATE_LED_3 GPIO_Pin_14
#define GATE_LED_4 GPIO_Pin_15

//define ADC inputs
#define ADC_PORT GPIOA
#define SLEW_POT GPIO_Pin_0
#define RATE_POT GPIO_Pin_1

#define NUM_BUTTONS 7

//define channel switches
#define CH_SW_PORT GPIOB
#define CH1_SW	GPIO_Pin_4
#define CH2_SW	GPIO_Pin_5
#define CH3_SW	GPIO_Pin_6
#define CH4_SW	GPIO_Pin_7

//define function switches
#define FUNC_SW_PORT GPIOA
#define MODE_SW	GPIO_Pin_12
#define REC_SW	GPIO_Pin_15
#define PARAM_SW	GPIO_Pin_9

#define MIDI_RX_PIN	3 //PA3

//DAC control bit defines for Write. Requires min 33ns HIGH to LOW LDAC pulse to latch all 4 DACs simultaneously. Can also configure for Write-through which doesn't require LDAC pulsing
#define DAC_CHAN_0 0b00010001
#define DAC_CHAN_1 0b00010010
#define DAC_CHAN_2 0b00010100
#define DAC_CHAN_3 0b00011000

#define DAC_CHAN_NUM 4 //there are 4 DAC channels on MAX5134 0, 1, 2, 3

#endif /* INCLUDE_HARDWARE_H_ */
