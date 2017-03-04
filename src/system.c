/*
 * system.c
 *
 *  Created on: Mar 1, 2017
 *      Author: jeff
 */

#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx_it.h"

//forward declarations

void setup_system_clock(void);

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


