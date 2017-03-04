/*
 * switches.h
 *
 *  Created on: Feb 18, 2017
 *      Author: jeff
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include "stm32f4xx.h"

void initialize_switches(uint32_t port, uint32_t pin);

uint32_t read_switch(GPIO_TypeDef *port, uint32_t pin);

#endif /* SWITCHES_H_ */
