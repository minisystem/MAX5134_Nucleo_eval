/*
 * system.h
 *
 *  Created on: May 29, 2017
 *      Author: jeff
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define NUM_CHANNELS 4
#define NUM_OCTAVES 11
#define POT_INTERVAL 3792
#define CODE_INTERVAL 4748 //number of DAC codes between octaves - there will be 4 intervals, one for each channel
#define CODE_OFFSET 3124 // there will be 4 absolute offsets, one for each channel

enum mode {

	NORMAL,
	CALIBRATE


};

__IO enum mode mode;

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
	int offset[NUM_OCTAVES];
	uint8_t octave_index:4;
};

extern volatile struct channel channel[NUM_CHANNELS];

__IO uint8_t current_channel;
__IO uint8_t octave;

extern volatile uint16_t DAC_value;

void setup_system_clock(void);
void gpio_init_output(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);
void gpio_init_input(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);
void gpio_init_switch(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);

void init_channels(void);

#endif /* SYSTEM_H_ */
