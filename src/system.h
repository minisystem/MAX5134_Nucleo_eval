/*
 * system.h
 *
 *  Created on: May 29, 2017
 *      Author: jeff
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define NUM_CHANNELS 4

struct flag {

	uint8_t sys_tick:1;


};

__IO struct flag flag;

void setup_system_clock(void);
void gpio_init_output(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);
void gpio_init_input(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);
void gpio_init_switch(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);

#endif /* SYSTEM_H_ */
