/*
 * switches.c
 *
 *  Created on: Feb 18, 2017
 *      Author: jeff
 */
#include "switches.h"
#include "hardware.h"
#include <leds.h>

struct button button[NUM_BUTTONS] = {

		{CH1_SW, CH_SW_PORT, 0, 0},
		{CH2_SW, CH_SW_PORT, 0, 0},
		{CH3_SW, CH_SW_PORT, 0, 0},
		{CH4_SW, CH_SW_PORT, 0, 0},
		{MODE_SW, FUNC_SW_PORT, 0, 0},
		{REC_SW, FUNC_SW_PORT, 0, 0},
		{PARAM_SW, FUNC_SW_PORT, 0, 0}
};

uint32_t read_switch(GPIO_TypeDef *port, uint32_t pin) {

	if (port->IDR & (1<<pin)) {

		return 1;
	} else {
		return 0;
	}
}

