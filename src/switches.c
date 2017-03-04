/*
 * switches.c
 *
 *  Created on: Feb 18, 2017
 *      Author: jeff
 */
#include "switches.h"
#include "hardware.h"
#include <leds.h>


void initialize_switches(uint32_t port, uint32_t pin) {

	  RCC->AHB1ENR |= RCC_MASKx(port);
/*
	  GPIO_InitTypeDef GPIO_InitStructure;

	  // Configure pin in output push/pull mode
	  //GPIO_InitStructure.Pin = BLINK_PIN_MASK(BLINK_PIN_NUMBER);
	  GPIO_InitStructure.Pin = PIN_MASK(pin);
	  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	  GPIO_InitStructure.Pull = GPIO_NOPULL; //no pull because of external pull up resistor, but could get rid of pull up resistor and use internul pull up
	  //int blink_gpio = BLINK_GPIOx(BLINK_PORT_NUMBER);
	  int gpio = GPIOx(port);
	  HAL_GPIO_Init(gpio, &GPIO_InitStructure);
*/
}

uint32_t read_switch(GPIO_TypeDef *port, uint32_t pin) {

	if (port->IDR & (1<<pin)) {

		return 1;
	} else {
		return 0;
	}
}

