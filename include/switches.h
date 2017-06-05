/*
 * switches.h
 *
 *  Created on: Feb 18, 2017
 *      Author: jeff
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include "stm32f4xx.h"
#include <hardware.h>

enum {
	CH1_SW_INDEX = 0,
	CH2_SW_INDEX,
	CH3_SW_INDEX,
	CH4_SW_INDEX,
	MODE_SW_INDEX,
	REC_SW_INDEX,
	PARAM_SW_INDEX

};

struct button {

	uint16_t pin;
	GPIO_TypeDef *port;
	uint8_t state:1;
	uint8_t held:1;


};

extern struct button button[NUM_BUTTONS];

void parse_switch_data(void);

uint8_t read_channel_switches(void);

uint32_t read_switch(GPIO_TypeDef *port, uint32_t pin);

#endif /* SWITCHES_H_ */
