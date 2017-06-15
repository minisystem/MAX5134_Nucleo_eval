/*
 * system.h
 *
 *  Created on: May 29, 2017
 *      Author: jeff
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define NUM_CHANNELS 4
#define NUM_OCTAVES 10
#define CODE_INTERVAL 4792 //number of DAC codes between octaves
#define CODE_OFFSET 3235 //

struct flag {

	uint8_t sys_tick:1;


};

__IO struct flag flag;

struct channel {

	uint8_t gate:1;
	uint8_t selected:1;
	uint8_t mode;
	uint8_t note;
	uint16_t cv;
	uint16_t pitch_table[NUM_OCTAVES];
	uint8_t octave_index:3;
};

extern volatile struct channel channel[NUM_CHANNELS];



void setup_system_clock(void);
void gpio_init_output(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);
void gpio_init_input(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);
void gpio_init_switch(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);

void init_channels(void);

#endif /* SYSTEM_H_ */
