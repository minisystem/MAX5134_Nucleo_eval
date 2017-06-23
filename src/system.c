/*
 * system.c
 *
 *  Created on: Mar 1, 2017
 *      Author: jeff
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_it.h"
#include "system.h"


//void setup_system_clock(void);
//void gpio_init_output(GPIO_TypeDef * gpio_port, uint32_t gpio_pin);

__IO enum mode mode = NORMAL;

__IO struct flag flag;

__IO struct channel channel[NUM_CHANNELS];



__IO uint8_t current_channel = 0;

__IO uint16_t DAC_value = 0;

void setup_system_clock(void) {

	RCC_HSEConfig(RCC_HSE_ON);
	while(!RCC_WaitForHSEStartUp());
	FLASH_SetLatency(FLASH_Latency_4); //for 3.3V FLASH latency needs to be 4 for 90 < HCLK < 120 MHz
	/* HCLK = SYSCLK */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);

	/* PCLK2 = HCLK */
	RCC_PCLK2Config(RCC_HCLK_Div2);

	 /* PCLK1 = HCLK */
	RCC_PCLK1Config(RCC_HCLK_Div2);

	//PLL CLOCK uses HSE, PLLM = /8, PLLN = X400, PLLP = /4
	RCC_PLLConfig(RCC_PLLSource_HSE, 8, 400, 4, 4, 2); //settings for 100 MHz from 8 MHz crystal

	/* Enable PLL */
	RCC_PLLCmd(ENABLE);

	/* Wait till PLL is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08);
	SystemCoreClockUpdate(); //need to call this to update system clock after changes made.


}

void gpio_init_output(GPIO_TypeDef * gpio_port, uint32_t gpio_pin) {

	//clock is started elsewhere
	  GPIO_InitTypeDef gpio_init;

	  gpio_init.GPIO_Pin = gpio_pin;
	  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	  gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	  gpio_init.GPIO_OType = GPIO_OType_PP;
	  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(gpio_port, &gpio_init);
}

void gpio_init_switch(GPIO_TypeDef * gpio_port, uint32_t gpio_pin) {

	GPIO_InitTypeDef gpio_init;

	gpio_init.GPIO_Pin = gpio_pin;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Fast_Speed;
	//gpio_init.GPIO_OType = GPIO_OType
	gpio_init.GPIO_PuPd = GPIO_PuPd_UP; //enable pull up
	GPIO_Init(gpio_port, &gpio_init);
}

void init_channels(void) {

	uint16_t init_pitch_table[NUM_OCTAVES];

	for (int i = 0; i < NUM_OCTAVES; i++) {

		init_pitch_table[i] = i*CODE_INTERVAL + CODE_OFFSET;
		//channel[0].pitch_table[i] = i*CODE_INTERVAL;
	}

	for (int i = 0; i < NUM_CHANNELS; i++) {

		memcpy((void*)channel[i].pitch_table, (const void*)init_pitch_table, (size_t)sizeof(init_pitch_table));
	}

	for (int i = 0; i < NUM_OCTAVES; i++) {

		channel[0].pitch_table[i] += channel[0].offset[i];
	}



}
